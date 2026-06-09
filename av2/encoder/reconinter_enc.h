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

#ifndef AV2_AV2_ENCODER_RECONINTER_ENC_H_
#define AV2_AV2_ENCODER_RECONINTER_ENC_H_

#include "aom/aom_integer.h"
#include "av2/common/av2_common_int.h"
#include "av2/common/blockd.h"
#include "av2/common/convolve.h"
#include "av2/common/filter.h"
#include "config/av2_dsp_rtcd.h"
#include "config/av1_rtcd.h"
#include "av2/common/reconinter.h"
#include "av2/common/warped_motion.h"
#include "av2/common/scale.h"

#ifdef __cplusplus
extern "C" {
#endif

// Build single or compound reference inter predictors for all planes.
// Can build inter-intra predictors, masked predictors etc as well.
int av2_enc_build_inter_predictor(const AV2_COMMON *cm, MACROBLOCKD *xd,
                                  int mi_row, int mi_col,
                                  const BUFFER_SET *ctx, BLOCK_SIZE bsize,
                                  int plane_from, int plane_to);

void enc_build_inter_predictors(const AV2_COMMON *cm, MACROBLOCKD *xd,
                                int plane, MB_MODE_INFO *mi,
                                const BUFFER_SET *ctx,
                                int build_for_refine_mv_only, int bw, int bh,
                                int mi_x, int mi_y);

// Build one inter predictor. It is called for building predictor for single
// reference case, or just the 1st or 2nd reference in compound reference case.
// Can build both regular and masked predictors.
void av2_enc_build_one_inter_predictor(uint16_t *dst, int dst_stride,
                                       const MV *src_mv,
                                       InterPredParams *inter_pred_params);

void av2_build_inter_predictor_single_buf_y(MACROBLOCKD *xd, BLOCK_SIZE bsize,
                                            int ref, uint16_t *ext_dst,
                                            int ext_dst_stride);

void av2_build_wedge_inter_predictor_from_buf_y(
    MACROBLOCKD *xd, BLOCK_SIZE bsize, uint16_t *ext_dst0, int ext_dst_stride0,
    uint16_t *ext_dst1, int ext_dst_stride1);

// Forward declarations for standard AV1 upsampled and mask prediction routines
struct AV1Common;
void aom_highbd_upsampled_pred(MACROBLOCKD *xd,
                               const struct AV1Common *const cm, int mi_row,
                               int mi_col, const MV *const mv,
                               uint8_t *comp_pred8, int width, int height,
                               int subpel_x_q3, int subpel_y_q3,
                               const uint8_t *ref8, int ref_stride, int bd,
                               int subpel_search);

void aom_highbd_comp_avg_upsampled_pred(
    MACROBLOCKD *xd, const struct AV1Common *const cm, int mi_row, int mi_col,
    const MV *const mv, uint8_t *comp_pred8, const uint8_t *pred8, int width,
    int height, int subpel_x_q3, int subpel_y_q3, const uint8_t *ref8,
    int ref_stride, int bd, int subpel_search);

void aom_highbd_comp_mask_upsampled_pred(
    MACROBLOCKD *xd, const struct AV1Common *const cm, int mi_row, int mi_col,
    const MV *const mv, uint8_t *comp_pred8, const uint8_t *pred8, int width,
    int height, int subpel_x_q3, int subpel_y_q3, const uint8_t *ref8,
    int ref_stride, const uint8_t *mask, int mask_stride, int invert_mask,
    int bd, int subpel_search);

static inline void av2_highbd_comp_mask_pred(uint16_t *dst,
                                             const uint16_t *pred, int width,
                                             int height, const uint16_t *ref,
                                             int ref_stride,
                                             const uint8_t *mask,
                                             int mask_stride, int invert_mask) {
  aom_highbd_comp_mask_pred((uint8_t *)dst, (const uint8_t *)pred, width,
                            height, (const uint8_t *)ref, ref_stride, mask,
                            mask_stride, invert_mask);
}

static inline void av2_highbd_comp_avg_pred(uint16_t *dst, const uint16_t *pred,
                                            int width, int height,
                                            const uint16_t *ref,
                                            int ref_stride) {
  aom_highbd_comp_avg_pred((uint8_t *)dst, (const uint8_t *)pred, width, height,
                           (const uint8_t *)ref, ref_stride);
}

#include "aom_ports/mem.h"

// Inline wrappers for AV2 HBD compatibility
static inline void av2_highbd_upsampled_pred(
    MACROBLOCKD *xd, const AV2_COMMON *const cm, int mi_row, int mi_col,
    const MV *const mv, uint16_t *comp_pred, int width, int height,
    int subpel_x_q3, int subpel_y_q3, const uint16_t *ref, int ref_stride,
    int bd, int subpel_search, int is_scaled) {
  (void)xd;
  (void)is_scaled;
  aom_highbd_upsampled_pred(NULL, (const struct AV1Common *)cm, mi_row, mi_col,
                            mv, CONVERT_TO_BYTEPTR(comp_pred), width, height,
                            subpel_x_q3, subpel_y_q3, CONVERT_TO_BYTEPTR(ref),
                            ref_stride, bd, subpel_search);
}

static inline void av2_highbd_comp_avg_upsampled_pred(
    MACROBLOCKD *xd, const AV2_COMMON *const cm, int mi_row, int mi_col,
    const MV *const mv, uint16_t *comp_pred, const uint16_t *pred, int width,
    int height, int subpel_x_q3, int subpel_y_q3, const uint16_t *ref,
    int ref_stride, int bd, int subpel_search, int is_scaled) {
  (void)xd;
  (void)is_scaled;
  aom_highbd_comp_avg_upsampled_pred(
      NULL, (const struct AV1Common *)cm, mi_row, mi_col, mv,
      CONVERT_TO_BYTEPTR(comp_pred), CONVERT_TO_BYTEPTR(pred), width, height, subpel_x_q3,
      subpel_y_q3, CONVERT_TO_BYTEPTR(ref), ref_stride, bd, subpel_search);
}

static inline void av2_highbd_comp_mask_upsampled_pred(
    MACROBLOCKD *xd, const AV2_COMMON *const cm, int mi_row, int mi_col,
    const MV *const mv, uint16_t *comp_pred, const uint16_t *pred, int width,
    int height, int subpel_x_q3, int subpel_y_q3, const uint16_t *ref,
    int ref_stride, const uint8_t *mask, int mask_stride, int invert_mask,
    int bd, int subpel_search, int is_scaled) {
  (void)xd;
  (void)is_scaled;
  aom_highbd_comp_mask_upsampled_pred(
      NULL, (const struct AV1Common *)cm, mi_row, mi_col, mv,
      CONVERT_TO_BYTEPTR(comp_pred), CONVERT_TO_BYTEPTR(pred), width, height, subpel_x_q3,
      subpel_y_q3, CONVERT_TO_BYTEPTR(ref), ref_stride, mask, mask_stride,
      invert_mask, bd, subpel_search);
}

static inline void av2_highbd_dist_wtd_comp_avg_upsampled_pred(
    MACROBLOCKD *xd, const AV2_COMMON *const cm, int mi_row, int mi_col,
    const MV *const mv, uint16_t *comp_pred, const uint16_t *pred, int width,
    int height, int subpel_x_q3, int subpel_y_q3, const uint16_t *ref,
    int ref_stride, int bd, const DIST_WTD_COMP_PARAMS *jcp_param,
    int subpel_search, int is_scaled) {
  (void)xd;
  (void)is_scaled;
  aom_highbd_upsampled_pred(NULL, (const struct AV1Common *)cm, mi_row, mi_col,
                            mv, CONVERT_TO_BYTEPTR(comp_pred), width, height,
                            subpel_x_q3, subpel_y_q3, CONVERT_TO_BYTEPTR(ref),
                            ref_stride, bd, subpel_search);
  const int w0 = jcp_param->fwd_offset;
  const int w1 = jcp_param->bck_offset;
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      comp_pred[j] = ROUND_POWER_OF_TWO(w0 * pred[j] + w1 * comp_pred[j], 4);
    }
    comp_pred += width;
    pred += width;
  }
}



#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // AV2_AV2_ENCODER_RECONINTER_ENC_H_
