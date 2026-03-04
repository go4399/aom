/*
 * Copyright (c) 2020, Alliance for Open Media. All rights reserved.
 *
 * This source code is subject to the terms of the BSD 2 Clause License and
 * the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
 * was not distributed with this source code in the LICENSE file, you can
 * obtain it at www.aomedia.org/license/software. If the Alliance for Open
 * Media Patent License 1.0 was not distributed with this source code in the
 * PATENTS file, you can obtain it at www.aomedia.org/license/patent.
 */

#ifndef AOM_AV1_ENCODER_PARTITION_SEARCH_H_
#define AOM_AV1_ENCODER_PARTITION_SEARCH_H_

#include "config/aom_config.h"

#include "av1/encoder/block.h"
#include "av1/encoder/encoder.h"
#include "av1/encoder/encodeframe.h"
#include "av1/encoder/tokenize.h"

void av1_set_offsets_without_segment_id(const AV1_COMP *const cpi,
                                        const TileInfo *const tile,
                                        MACROBLOCK *const x, int mi_row,
                                        int mi_col, BLOCK_SIZE bsize);
#if !CONFIG_HW_ML_PART
void av1_set_offsets(const AV1_COMP *const cpi, const TileInfo *const tile,
                     MACROBLOCK *const x, int mi_row, int mi_col,
                     BLOCK_SIZE bsize);
#endif  // CONFIG_HW_ML_PART
void av1_rd_use_partition(AV1_COMP *cpi, ThreadData *td, TileDataEnc *tile_data,
                          MB_MODE_INFO **mib, TokenExtra **tp, int mi_row,
                          int mi_col, BLOCK_SIZE bsize, int *rate,
                          int64_t *dist, int do_recon, PC_TREE *pc_tree);
void av1_nonrd_use_partition(AV1_COMP *cpi, ThreadData *td,
                             TileDataEnc *tile_data, MB_MODE_INFO **mib,
                             TokenExtra **tp, int mi_row, int mi_col,
                             BLOCK_SIZE bsize, PC_TREE *pc_tree);
#if CONFIG_RT_ML_PARTITIONING
void av1_nonrd_pick_partition(AV1_COMP *cpi, ThreadData *td,
                              TileDataEnc *tile_data, TokenExtra **tp,
                              int mi_row, int mi_col, BLOCK_SIZE bsize,
                              RD_STATS *rd_cost, int do_recon, int64_t best_rd,
                              PC_TREE *pc_tree);
#endif

#if CONFIG_PARTITION_SEARCH_ORDER
void av1_reset_part_sf(PARTITION_SPEED_FEATURES *part_sf);
void av1_reset_sf_for_ext_part(AV1_COMP *const cpi);
bool av1_rd_partition_search(AV1_COMP *const cpi, ThreadData *td,
                             TileDataEnc *tile_data, TokenExtra **tp,
                             SIMPLE_MOTION_DATA_TREE *sms_root, int mi_row,
                             int mi_col, BLOCK_SIZE bsize,
                             RD_STATS *best_rd_cost);
#endif

bool av1_rd_pick_partition(AV1_COMP *const cpi, ThreadData *td,
                           TileDataEnc *tile_data, TokenExtra **tp, int mi_row,
                           int mi_col, BLOCK_SIZE bsize, RD_STATS *rd_cost,
                           RD_STATS best_rdc, PC_TREE *pc_tree,
                           SIMPLE_MOTION_DATA_TREE *sms_tree, int64_t *none_rd,
                           SB_MULTI_PASS_MODE multi_pass_mode,
                           RD_RECT_PART_WIN_INFO *rect_part_win_info);

static inline void set_cb_offsets(uint16_t *cb_offset,
                                  const uint16_t cb_offset_y,
                                  const uint16_t cb_offset_uv) {
  cb_offset[PLANE_TYPE_Y] = cb_offset_y;
  cb_offset[PLANE_TYPE_UV] = cb_offset_uv;
}

static inline void update_cb_offsets(MACROBLOCK *x, const BLOCK_SIZE bsize,
                                     const int subsampling_x,
                                     const int subsampling_y) {
  x->cb_offset[PLANE_TYPE_Y] += block_size_wide[bsize] * block_size_high[bsize];
  if (x->e_mbd.is_chroma_ref) {
    const BLOCK_SIZE plane_bsize =
        get_plane_block_size(bsize, subsampling_x, subsampling_y);
    assert(plane_bsize != BLOCK_INVALID);
    x->cb_offset[PLANE_TYPE_UV] +=
        block_size_wide[plane_bsize] * block_size_high[plane_bsize];
  }
}

#if CONFIG_HW_ML_PART
enum {
  FEATURE_INTER_RD_MULT = 0,
  FEATURE_INTER_FULL_PSNR,
  FEATURE_INTER_FULL_Q_COEFF_MAX,
  FEATURE_INTER_FULL_Q_COEFF_NONZ,
  FEATURE_INTER_SQ_0_PSNR,
  FEATURE_INTER_SQ_0_Q_COEFF_MAX,
  FEATURE_INTER_SQ_0_Q_COEFF_NONZ,
  FEATURE_INTER_SQ_1_PSNR,
  FEATURE_INTER_SQ_1_Q_COEFF_MAX,
  FEATURE_INTER_SQ_1_Q_COEFF_NONZ,
  FEATURE_INTER_SQ_2_PSNR,
  FEATURE_INTER_SQ_2_Q_COEFF_MAX,
  FEATURE_INTER_SQ_2_Q_COEFF_NONZ,
  FEATURE_INTER_SQ_3_PSNR,
  FEATURE_INTER_SQ_3_Q_COEFF_MAX,
  FEATURE_INTER_SQ_3_Q_COEFF_NONZ,
  FEATURE_INTER_FULL_LOG_MAG,
  FEATURE_INTER_FULL_ANGLE_RAD,
  FEATURE_INTER_SQ_0_LOG_MAG,
  FEATURE_INTER_SQ_0_ANGLE_RAD,
  FEATURE_INTER_SQ_1_LOG_MAG,
  FEATURE_INTER_SQ_1_ANGLE_RAD,
  FEATURE_INTER_SQ_2_LOG_MAG,
  FEATURE_INTER_SQ_2_ANGLE_RAD,
  FEATURE_INTER_SQ_3_LOG_MAG,
  FEATURE_INTER_SQ_3_ANGLE_RAD,
  FEATURE_INTER_FULL_LOG_SATDQ,
  FEATURE_INTER_SQ_0_LOG_SATDQ,
  FEATURE_INTER_SQ_1_LOG_SATDQ,
  FEATURE_INTER_SQ_2_LOG_SATDQ,
  FEATURE_INTER_SQ_3_LOG_SATDQ,
  FEATURE_INTER_FULL_LOG_SATD,
  FEATURE_INTER_SQ_0_LOG_SATD,
  FEATURE_INTER_SQ_1_LOG_SATD,
  FEATURE_INTER_SQ_2_LOG_SATD,
  FEATURE_INTER_SQ_3_LOG_SATD,
  FEATURE_INTER_HOR_0_PSNR,
  FEATURE_INTER_HOR_0_Q_COEFF_MAX,
  FEATURE_INTER_HOR_0_Q_COEFF_NONZ,
  FEATURE_INTER_HOR_0_LOG_MAG,
  FEATURE_INTER_HOR_0_ANGLE_RAD,
  FEATURE_INTER_HOR_0_LOG_SATDQ,
  FEATURE_INTER_HOR_0_LOG_SATD,
  FEATURE_INTER_HOR_1_PSNR,
  FEATURE_INTER_HOR_1_Q_COEFF_MAX,
  FEATURE_INTER_HOR_1_Q_COEFF_NONZ,
  FEATURE_INTER_HOR_1_LOG_MAG,
  FEATURE_INTER_HOR_1_ANGLE_RAD,
  FEATURE_INTER_HOR_1_LOG_SATDQ,
  FEATURE_INTER_HOR_1_LOG_SATD,
  FEATURE_INTER_VER_0_PSNR,
  FEATURE_INTER_VER_0_Q_COEFF_MAX,
  FEATURE_INTER_VER_0_Q_COEFF_NONZ,
  FEATURE_INTER_VER_0_LOG_MAG,
  FEATURE_INTER_VER_0_ANGLE_RAD,
  FEATURE_INTER_VER_0_LOG_SATDQ,
  FEATURE_INTER_VER_0_LOG_SATD,
  FEATURE_INTER_VER_1_PSNR,
  FEATURE_INTER_VER_1_Q_COEFF_MAX,
  FEATURE_INTER_VER_1_Q_COEFF_NONZ,
  FEATURE_INTER_VER_1_LOG_MAG,
  FEATURE_INTER_VER_1_ANGLE_RAD,
  FEATURE_INTER_VER_1_LOG_SATDQ,
  FEATURE_INTER_VER_1_LOG_SATD,

  // V1 features
  // NONE w RECT switch, search_none_after_rect
  FEATURE_INTER_SWITCH,
  // partition type: mixed, luma, chroma
  FEATURE_INTER_PART_T,

  FEATURE_INTER_MAX
};

enum {
  FEATURE_INTRA_LOG_QP_SQUARED = 0,
  FEATURE_INTRA_HAS_ABOVE,
  FEATURE_INTRA_LOG_ABOVE_WIDTH,
  FEATURE_INTRA_LOG_ABOVE_HEIGHT,
  FEATURE_INTRA_HAS_LEFT,
  FEATURE_INTRA_LOG_LEFT_WIDTH,
  FEATURE_INTRA_LOG_LEFT_HEIGHT,
  FEATURE_INTRA_NORM_BEST_0_SSE,
  FEATURE_INTRA_NORM_BEST_0_VAR,
  FEATURE_INTRA_NORM_BEST_1_SSE,
  FEATURE_INTRA_NORM_BEST_1_VAR,
  FEATURE_INTRA_NORM_BEST_2_SSE,
  FEATURE_INTRA_NORM_BEST_2_VAR,
  FEATURE_INTRA_NORM_BEST_SSE_0_00,
  FEATURE_INTRA_NORM_BEST_VAR_0_00,
  FEATURE_INTRA_NORM_BEST_SSE_0_01,
  FEATURE_INTRA_NORM_BEST_VAR_0_01,
  FEATURE_INTRA_NORM_BEST_SSE_0_10,
  FEATURE_INTRA_NORM_BEST_VAR_0_10,
  FEATURE_INTRA_NORM_BEST_SSE_0_11,
  FEATURE_INTRA_NORM_BEST_VAR_0_11,
  FEATURE_INTRA_NORM_BEST_SSE_1_00,
  FEATURE_INTRA_NORM_BEST_VAR_1_00,
  FEATURE_INTRA_NORM_BEST_SSE_1_01,
  FEATURE_INTRA_NORM_BEST_VAR_1_01,
  FEATURE_INTRA_NORM_BEST_SSE_1_10,
  FEATURE_INTRA_NORM_BEST_VAR_1_10,
  FEATURE_INTRA_NORM_BEST_SSE_1_11,
  FEATURE_INTRA_NORM_BEST_VAR_1_11,
  FEATURE_INTRA_NORM_BEST_SSE_2_00,
  FEATURE_INTRA_NORM_BEST_VAR_2_00,
  FEATURE_INTRA_NORM_BEST_SSE_2_01,
  FEATURE_INTRA_NORM_BEST_VAR_2_01,
  FEATURE_INTRA_NORM_BEST_SSE_2_10,
  FEATURE_INTRA_NORM_BEST_VAR_2_10,
  FEATURE_INTRA_NORM_BEST_SSE_2_11,
  FEATURE_INTRA_NORM_BEST_VAR_2_11,
  FEATURE_INTRA_HOR_NORM_BEST_SSE_0_0,
  FEATURE_INTRA_HOR_NORM_BEST_VAR_0_0,
  FEATURE_INTRA_HOR_NORM_BEST_SSE_0_1,
  FEATURE_INTRA_HOR_NORM_BEST_VAR_0_1,
  FEATURE_INTRA_HOR_NORM_BEST_SSE_1_0,
  FEATURE_INTRA_HOR_NORM_BEST_VAR_1_0,
  FEATURE_INTRA_HOR_NORM_BEST_SSE_1_1,
  FEATURE_INTRA_HOR_NORM_BEST_VAR_1_1,
  FEATURE_INTRA_HOR_NORM_BEST_SSE_2_0,
  FEATURE_INTRA_HOR_NORM_BEST_VAR_2_0,
  FEATURE_INTRA_HOR_NORM_BEST_SSE_2_1,
  FEATURE_INTRA_HOR_NORM_BEST_VAR_2_1,

  FEATURE_INTRA_VER_NORM_BEST_SSE_0_0,
  FEATURE_INTRA_VER_NORM_BEST_VAR_0_0,
  FEATURE_INTRA_VER_NORM_BEST_SSE_0_1,
  FEATURE_INTRA_VER_NORM_BEST_VAR_0_1,
  FEATURE_INTRA_VER_NORM_BEST_SSE_1_0,
  FEATURE_INTRA_VER_NORM_BEST_VAR_1_0,
  FEATURE_INTRA_VER_NORM_BEST_SSE_1_1,
  FEATURE_INTRA_VER_NORM_BEST_VAR_1_1,
  FEATURE_INTRA_VER_NORM_BEST_SSE_2_0,
  FEATURE_INTRA_VER_NORM_BEST_VAR_2_0,
  FEATURE_INTRA_VER_NORM_BEST_SSE_2_1,
  FEATURE_INTRA_VER_NORM_BEST_VAR_2_1,

  FEATURE_INTRA_MAX
};

SimpleMotionData *av1_get_sms_data_entry(SimpleMotionDataBufs *sms_bufs,
                                         int mi_row, int mi_col,
                                         BLOCK_SIZE bsize, BLOCK_SIZE sb_size,
                                         int8_t region_type);
SimpleMotionData *av1_get_sms_data(AV1_COMP *const cpi,
                                   const TileInfo *const tile, MACROBLOCK *x,
                                   int mi_row, int mi_col, BLOCK_SIZE bsize,
                                   ThreadData *td, bool need_residual_stats,
                                   int8_t region_type);
#endif  // CONFIG_HW_ML_PART

#endif  // AOM_AV1_ENCODER_PARTITION_SEARCH_H_
