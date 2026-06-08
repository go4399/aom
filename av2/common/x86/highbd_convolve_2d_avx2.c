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

#include <immintrin.h>
#include <assert.h>

#include "config/av2_dsp_rtcd.h"

#include "av2/common/x86/av2_convolve_avx2.h"
#include "aom_dsp/x86/synonyms.h"
#include "aom_dsp/aom_dsp_common.h"
#include "aom_dsp/aom_filter.h"
#include "av2/common/convolve.h"

// Public RTCD entry point for the AVX2 highbd separable 2D convolve. The actual
// kernel lives as a static-inline in av2_convolve_avx2.h; the specialized
// variant already dispatches on the per-axis tap count (8/6/4/2-tap, the 2-tap
// branch being the bilinear case), so it is correct for every filter type.
void av2_highbd_convolve_2d_sr_avx2(
    const uint16_t *src, int src_stride, uint16_t *dst, int dst_stride, int w,
    int h, const InterpFilterParams *filter_params_x,
    const InterpFilterParams *filter_params_y, const int subpel_x_qn,
    const int subpel_y_qn, ConvolveParams *conv_params, int bd) {
  av2_highbd_convolve_2d_sr_specialized_avx2(
      src, src_stride, dst, dst_stride, w, h, filter_params_x, filter_params_y,
      subpel_x_qn, subpel_y_qn, conv_params, bd);
}
