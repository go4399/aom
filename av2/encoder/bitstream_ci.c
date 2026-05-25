/*
 * Copyright (c) 2025, Alliance for Open Media. All rights reserved
 *
 * This source code is subject to the terms of the BSD 3-Clause Clear License
 * and the Alliance for Open Media Patent License 1.0. If the BSD 3-Clause Clear
 * License was not distributed with this source code in the LICENSE file, you
 * can obtain it at aomedia.org/license/software-license/bsd-3-c-c/.  If the
 * Alliance for Open Media Patent License 1.0 was not distributed with this
 * source code in the PATENTS file, you can obtain it at
 * aomedia.org/license/patent-license/.
 */
#include <assert.h>
#include <limits.h>
#include <stdio.h>

#include "aom/aom_encoder.h"
#include "aom_dsp/aom_dsp_common.h"
#include "aom_dsp/binary_codes_writer.h"
#include "aom_dsp/bitwriter_buffer.h"
#include "aom_mem/aom_mem.h"
#include "aom_ports/bitops.h"
#include "aom_ports/mem_ops.h"
#include "aom_ports/system_state.h"
#include "av2/encoder/bitstream.h"
#include "av2/encoder/ci_syntax.h"

// Custom type-erased mirror mappings to completely isolate compiling unit
typedef struct color_infoci {
  int color_description_idc;
  int color_primaries;
  int transfer_characteristics;
  int matrix_coefficients;
  int full_range_flag;
} ColorInfoci;

typedef struct sar_infoci {
  int sar_aspect_ratio_idc;
  int sar_width;
  int sar_height;
} SarInfoci;

typedef struct ContentInterpretationci {
  int ci_scan_type_idc;
  int ci_color_description_present_flag;
  int ci_chroma_sample_position_present_flag;
  int ci_aspect_ratio_info_present_flag;
  int ci_timing_info_present_flag;
  int ci_extension_present_flag;
  int ci_chroma_sample_position[2];

  ColorInfoci color_info;
  SarInfoci sar_info;
  avm_timing_info_t timing_info;
  bool ci_from_leading;
} ContentInterpretationci;

void av2_write_color_info(const struct ContentInterpretation *ci_params,
                          struct aom_write_bit_buffer *wb) {
  const ContentInterpretationci *params =
      (const ContentInterpretationci *)ci_params;
  const ColorInfoci *col_info = &params->color_info;
  avm_wb_write_rice_golomb(wb, col_info->color_description_idc, 2);
  if (col_info->color_description_idc == 0) {
    avm_wb_write_literal(wb, col_info->color_primaries, 8);
    avm_wb_write_literal(wb, col_info->transfer_characteristics, 8);
    avm_wb_write_literal(wb, col_info->matrix_coefficients, 8);
  }
  avm_wb_write_bit(wb, col_info->full_range_flag);
}

void av2_write_sar_info(const struct ContentInterpretation *ci_params,
                        struct aom_write_bit_buffer *wb) {
  const ContentInterpretationci *params =
      (const ContentInterpretationci *)ci_params;
  const SarInfoci *sar_info = &params->sar_info;
  avm_wb_write_literal(wb, sar_info->sar_aspect_ratio_idc, 8);
  if (sar_info->sar_aspect_ratio_idc == AVM_SAR_IDC_255) {
    avm_wb_write_uvlc(wb, sar_info->sar_width);
    avm_wb_write_uvlc(wb, sar_info->sar_height);
  }
}

int av2_write_ci_info(const struct ContentInterpretation *ci_params,
                      struct aom_write_bit_buffer *wb) {
  const ContentInterpretationci *params =
      (const ContentInterpretationci *)ci_params;
  avm_wb_write_literal(wb, params->ci_scan_type_idc, 2);
  avm_wb_write_bit(wb, params->ci_color_description_present_flag);
  avm_wb_write_bit(wb, params->ci_chroma_sample_position_present_flag);
  avm_wb_write_bit(wb, params->ci_aspect_ratio_info_present_flag);
  avm_wb_write_bit(wb, params->ci_timing_info_present_flag);
  avm_wb_write_literal(wb, 0, 2);

  if (params->ci_color_description_present_flag) {
    av2_write_color_info(ci_params, wb);
  }

  if (params->ci_chroma_sample_position_present_flag) {
    avm_wb_write_uvlc(wb, params->ci_chroma_sample_position[0]);
    if (params->ci_scan_type_idc != 1) {
      avm_wb_write_uvlc(wb, params->ci_chroma_sample_position[1]);
    }
  }
  if (params->ci_aspect_ratio_info_present_flag) {
    av2_write_sar_info(ci_params, wb);
  }

  if (params->ci_timing_info_present_flag) {
    av2_write_timing_info_header(&params->timing_info, wb);
  }
  return 0;
}

uint32_t av2_write_content_interpretation_obu(
    const struct ContentInterpretation *ci_params, uint8_t *const dst) {
  struct aom_write_bit_buffer wb = { dst, 0 };
  const ContentInterpretationci *params =
      (const ContentInterpretationci *)ci_params;

  av2_write_ci_info(ci_params, &wb);

  avm_wb_write_bit(&wb, params->ci_extension_present_flag);
  assert(!params->ci_extension_present_flag);
  av2_add_trailing_bits(&wb);
  return avm_wb_bytes_written(&wb);
}
