/*
 * Copyright (c) 2021, Alliance for Open Media. All rights reserved
 *
 * This source code is subject to the terms of the BSD 3-Clause Clear License
 * and the Alliance for Open Media Patent License 1.0. If the BSD 3-Clause Clear
 * License was not distributed with this source code in the LICENSE file, you
 * can obtain it at aomedia.org/license/software-license/bsd-3-c-c/.  If the
 * Alliance for Open Media Patent License 1.0 was not distributed with this
 * source code in the PATENTS file, you can obtain it at
 * aomedia.org/license/patent-license/.
 */

#ifndef AV2_AV2_ENCODER_BITSTREAM_H_
#define AV2_AV2_ENCODER_BITSTREAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "aom_dsp/bitwriter_buffer.h"
#include "aom_dsp/binary_codes_writer.h"
#include "av2/common/enums.h"
#include "av2/common/obu_util.h"
#include "av2/common/timing.h"

#define av2_wb_write_literal aom_wb_write_literal
#define av2_wb_write_bit aom_wb_write_bit
#define av2_wb_overwrite_literal aom_wb_overwrite_literal
#define av2_wb_overwrite_bit aom_wb_overwrite_bit
#define av2_write_bit_buffer aom_write_bit_buffer
#define av2_wb_write_uvlc aom_wb_write_uvlc
#define av2_wb_bytes_written aom_wb_bytes_written

#define av2_wb_write_inv_signed_literal aom_wb_write_inv_signed_literal
#define av2_wb_write_unsigned_literal aom_wb_write_unsigned_literal
#define av2_uleb_encode aom_uleb_encode
#define aom_uleb_size_in_bytes aom_uleb_size_in_bytes
#define av2_wb_is_byte_aligned aom_wb_is_byte_aligned
#define av2_wb_write_uleb av2_wb_write_uleb
#define av2_img_alloc aom_img_alloc
#define av2_img_free aom_img_free

static INLINE void av2_wb_write_uleb(struct aom_write_bit_buffer *wb,
                                     uint32_t value) {
  do {
    uint8_t byte = value & 0x7f;
    value >>= 7;
    if (value != 0) byte |= 0x80;
    aom_wb_write_literal(wb, byte, 8);
  } while (value != 0);
}

#define AV2_MIF_KEY_FRAME AOM_MIF_KEY_FRAME
#define AV2_MIF_NON_KEY_FRAME AOM_MIF_NON_KEY_FRAME

static INLINE void av2_wb_write_rice_golomb(struct aom_write_bit_buffer *wb,
                                            uint32_t data, int k) {
  assert(k <= 26);
  uint32_t quotient = data >> k;
  assert(quotient < 32);
  uint32_t mask = (1 << k) - 1;
  uint32_t remainder = data & mask;
  for (uint32_t i = 0; i < quotient; i++) {
    aom_wb_write_bit(wb, 1);
  }
  aom_wb_write_bit(wb, 0);
  aom_wb_write_literal(wb, remainder, k);
}

static INLINE void av2_wb_write_svlc(struct aom_write_bit_buffer *wb,
                                     int32_t value) {
  uint32_t uvlc;
  if (value > 0) {
    uvlc = (uint32_t)(value * 2 - 1);
  } else {
    uvlc = (uint32_t)(abs(value) * 2);
  }
  aom_wb_write_uvlc(wb, uvlc);
}
#ifndef av2_write_primitive_quniform_defined
#define av2_write_primitive_quniform_defined
static INLINE void av2_write_primitive_quniform(aom_writer *w, uint16_t n,
                                                uint16_t v) {
  if (n <= 1) return;
  const int l = get_msb(n - 1) + 1;
  const int m = (1 << l) - n;
  if (v < m) {
    aom_write_literal(w, v, l - 1);
  } else {
    aom_write_literal(w, m + ((v - m) >> 1), l - 1);
    aom_write_literal(w, (v - m) & 1, 1);
  }
}
#endif

#ifndef av2_wb_write_primitive_quniform_defined
#define av2_wb_write_primitive_quniform_defined
static INLINE void av2_wb_write_primitive_quniform(
    struct aom_write_bit_buffer *wb, uint16_t n, uint16_t v) {
  if (n <= 1) return;
  assert(v < n);
  const int l = get_msb(n) + 1;
  const int m = (1 << l) - n;
  if (v < m) {
    aom_wb_write_literal(wb, v, l - 1);
  } else {
    aom_wb_write_literal(wb, m + ((v - m) >> 1), l - 1);
    aom_wb_write_bit(wb, (v - m) & 1);
  }
}
#endif

static INLINE void av2_wb_write_primitive_ref_quniform(
    struct aom_write_bit_buffer *wb, uint16_t n, uint16_t r, uint16_t v) {
  if (v == r) {
    aom_wb_write_bit(wb, 0);
  } else {
    aom_wb_write_bit(wb, 1);
    av2_wb_write_primitive_quniform(wb, n - 1, v - (v > r));
  }
}

struct aom_write_bit_buffer;
struct quantization_matrix_set;
struct film_grain_model;
struct RestorationUnitInfo;
struct TileInfo;
struct BufferRemovalTimingInfo;
struct OperatingPointSet;
struct AtlasSegmentInfo;
struct ContentInterpretation;
struct color_info;
struct sar_info;

// Standard AV2 typedef forward-declarations to align pointer parameters
// perfectly
typedef struct AV2_COMP AV2_COMP;
typedef struct AV2Common AV2Common;
typedef struct AV2Common AV2_COMMON;
typedef struct macroblockd MACROBLOCKD;
typedef struct macroblock MACROBLOCK;
typedef struct aom_writer aom_writer;
typedef struct SequenceHeader SequenceHeader;

void av2_write_conformance_window(const SequenceHeader *seq_params,
                                  struct aom_write_bit_buffer *wb);

void setup_cm_qmindex_list(AV2Common *const cm);
bool add_userqm_in_qmobulist(AV2_COMP *cpi);
uint32_t write_qm_obu(AV2_COMP *cpi, int signalled_obu_pos, uint8_t *const dst);
int write_qm_data(AV2_COMP *cpi, struct quantization_matrix_set *qm_list,
                  int qm_pos, const int num_planes,
                  struct aom_write_bit_buffer *wb);
void set_film_grain_model(const AV2_COMP *const cpi,
                          struct film_grain_model *fgm_current);
int film_grain_model_decision(int fgm_pos, struct film_grain_model *fgm_in_list,
                              struct film_grain_model *fgm);
int write_fgm_obu(AV2_COMP *cpi, struct film_grain_model *fgm,
                  uint8_t *const dst);

// Writes only the OBU Sequence Header payload, and returns the size of the
// payload written to 'dst'. This function does not write the OBU header, the
// optional extension, or the OBU size to 'dst'.
uint32_t av2_write_sequence_header_obu(const SequenceHeader *seq_params,
                                       uint8_t *const dst);

// Writes the OBU header byte, and the OBU header extension byte when
// obu_type is not OBU_MULTI_STREAM_DECODER_OPERATION and obu_layer is non-zero.
// Returns number of bytes written to 'dst'.
uint32_t av2_write_obu_header(OBU_TYPE obu_type, int obu_temporal,
                              int obu_layer, uint8_t *const dst);

int av2_write_uleb_obu_size(size_t obu_header_size, size_t obu_payload_size,
                            uint8_t *dest);

void av2_add_trailing_bits(struct aom_write_bit_buffer *wb);

uint32_t av2_write_layer_configuration_record_obu(AV2_COMP *cpi, int xlayer_id,
                                                  uint8_t *const dst);
uint32_t av2_write_atlas_segment_info_obu(AV2_COMP *cpi, uint8_t *const dst);
uint32_t av2_write_operating_point_set_obu(AV2_COMP *cpi, int xlayer_id,
                                           int obu_xlayer_id,
                                           uint8_t *const dst);

int av2_set_atlas_segment_info_params(AV2_COMP *cpi,
                                      struct AtlasSegmentInfo *atlas,
                                      int xlayer_id);

void av2_set_ops_params(struct OperatingPointSet *ops, int xlayer_id,
                        int ops_id, int ops_cnt);

uint32_t av2_write_buffer_removal_timing_obu(
    const struct BufferRemovalTimingInfo *brt_info, uint8_t *const dst);

void av2_set_buffer_removal_timing_params(AV2_COMP *const cpi);

/*!\brief Pack the bitstream for one frame
 *
 * \ingroup high_level_algo
 * \callgraph
 */
int av2_pack_bitstream(AV2_COMP *cpi, uint8_t *dst, size_t *size,
                       int *const largest_tile_id);

void av2_write_sec_tx_type(const AV2_COMMON *const cm, const MACROBLOCKD *xd,
                           av2_tx_type tx_type, TX_SIZE tx_size, uint16_t eob,
                           aom_writer *w);

void av2_write_tx_type(const AV2_COMMON *const cm, const MACROBLOCKD *xd,
                       av2_tx_type tx_type, TX_SIZE tx_size, aom_writer *w,
                       const int plane, const int eob, const int dc_skip);

void av2_write_cctx_type(const AV2_COMMON *const cm, const MACROBLOCKD *xd,
                         CctxType cctx_type, TX_SIZE tx_size, aom_writer *w);

void av2_write_timing_info_header(const av2_timing_info_t *const timing_info,
                                  struct aom_write_bit_buffer *wb);
uint32_t av2_write_content_interpretation_obu(
    const struct ContentInterpretation *ci_params, uint8_t *const dst);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // AV2_AV2_ENCODER_BITSTREAM_H_
