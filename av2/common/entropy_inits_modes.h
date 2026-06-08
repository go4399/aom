/*
 * Copyright (c) 2024, Alliance for Open Media. All rights reserved
 *
 * This source code is subject to the terms of the BSD 3-Clause Clear License
 * and the Alliance for Open Media Patent License 1.0. If the BSD 3-Clause Clear
 * License was not distributed with this source code in the LICENSE file, you
 * can obtain it at aomedia.org/license/software-license/bsd-3-c-c/.  If the
 * Alliance for Open Media Patent License 1.0 was not distributed with this
 * source code in the PATENTS file, you can obtain it at
 * aomedia.org/license/patent-license/.
 */

#ifndef AV2_AV2_COMMON_ENTROPY_INITS_MODES_H_
#define AV2_AV2_COMMON_ENTROPY_INITS_MODES_H_

#include "config/aom_config.h"

#include "av2/common/entropymode.h"

#ifdef __cplusplus
extern "C" {
#endif

static const aom_cdf_prob
    default_inter_single_mode_cdf[INTER_MODE_CONTEXTS][AV2_CDF_SIZE(3)] = {
      { AV2_CDF3(10043, 11100), AV2_PARA3(0, -1, -1) },
      { AV2_CDF3(21561, 21758), AV2_PARA3(0, 0, -1) },
      { AV2_CDF3(25411, 25714), AV2_PARA3(0, 0, 0) },
      { AV2_CDF3(14117, 14341), AV2_PARA3(0, 0, 0) },
      { AV2_CDF3(18288, 18577), AV2_PARA3(0, 0, 0) },
    };

static const aom_cdf_prob
    default_inter_warp_mode_cdf[WARPMV_MODE_CONTEXT][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(25999), AV2_PARA2(0, 0, -1) },
      { AV2_CDF2(14478), AV2_PARA2(0, -1, -2) },
      { AV2_CDF2(10868), AV2_PARA2(0, -1, -1) },
      { AV2_CDF2(5256), AV2_PARA2(-1, -1, -1) },
      { AV2_CDF2(2722), AV2_PARA2(-1, -1, -1) },
    };

static const aom_cdf_prob default_is_warpmv_or_warp_newmv_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(15095), AV2_PARA2(0, 0, -1)
};

static const aom_cdf_prob
    default_refinemv_flag_cdf[NUM_REFINEMV_CTX][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(20204), AV2_PARA2(-1, 0, 0) },
      { AV2_CDF2(17614), AV2_PARA2(-1, 1, 1) },
      { AV2_CDF2(24443), AV2_PARA2(-1, -1, -2) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
    };

static const aom_cdf_prob
    default_drl_cdf[DRL_CDF_TYPES][DRL_MODE_CONTEXTS][AV2_CDF_SIZE(2)] = {
      {
          { AV2_CDF2(15721), AV2_PARA2(1, 1, 0) },
          { AV2_CDF2(21115), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(19567), AV2_PARA2(0, 0, -1) },
          { AV2_CDF2(17602), AV2_PARA2(1, 1, 1) },
          { AV2_CDF2(13319), AV2_PARA2(1, 1, 1) },
      },
      {
          { AV2_CDF2(18692), AV2_PARA2(1, 1, 1) },
          { AV2_CDF2(19343), AV2_PARA2(1, 1, 0) },
          { AV2_CDF2(18207), AV2_PARA2(1, 1, 0) },
          { AV2_CDF2(17908), AV2_PARA2(1, 1, 1) },
          { AV2_CDF2(18304), AV2_PARA2(1, 1, 1) },
      },
      {
          { AV2_CDF2(22157), AV2_PARA2(1, 1, 0) },
          { AV2_CDF2(23233), AV2_PARA2(1, 1, 0) },
          { AV2_CDF2(22782), AV2_PARA2(1, 1, 0) },
          { AV2_CDF2(22353), AV2_PARA2(1, 1, 1) },
          { AV2_CDF2(22457), AV2_PARA2(1, 1, 1) },
      },
    };

static const aom_cdf_prob default_tip_drl_cdf[DRL_CDF_TYPES][AV2_CDF_SIZE(2)] = {
  { AV2_CDF2(30662), AV2_PARA2(0, 0, 0) },
  { AV2_CDF2(23823), AV2_PARA2(0, -1, -1) },
  { AV2_CDF2(21676), AV2_PARA2(0, -1, -1) },
};

static const aom_cdf_prob default_skip_drl_cdf[DRL_CDF_TYPES][AV2_CDF_SIZE(2)] = {
  { AV2_CDF2(21634), AV2_PARA2(0, 0, 0) },
  { AV2_CDF2(17376), AV2_PARA2(0, 0, 0) },
  { AV2_CDF2(18432), AV2_PARA2(1, 0, 0) },
};

static const aom_cdf_prob
    default_use_optflow_cdf[OPFL_MODE_CONTEXTS][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(11582), AV2_PARA2(-1, -1, -1) },
      { AV2_CDF2(24076), AV2_PARA2(-1, 0, -1) },
    };

static const aom_cdf_prob
    default_inter_compound_mode_is_joint_cdf[NUM_CTX_IS_JOINT][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(24720), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(32764), AV2_PARA2(0, 0, 0) },
    };

static const aom_cdf_prob default_inter_compound_mode_non_joint_type_cdf
    [NUM_CTX_NON_JOINT_TYPE][AV2_CDF_SIZE(5)] = {
      { AV2_CDF5(12177, 20001, 23193, 24448), AV2_PARA5(-1, 0, -1) },
      { AV2_CDF5(21192, 25117, 27806, 27948), AV2_PARA5(-1, -1, -1) },
      { AV2_CDF5(26779, 28724, 30192, 30249), AV2_PARA5(-1, 0, 0) },
      { AV2_CDF5(12506, 17871, 21295, 21389), AV2_PARA5(-1, -1, -1) },
      { AV2_CDF5(16948, 20335, 22582, 22617), AV2_PARA5(-1, -1, -1) },
    };

static const aom_cdf_prob
    default_inter_compound_mode_same_refs_cdf[INTER_MODE_CONTEXTS][AV2_CDF_SIZE(
        4)] = {
      { AV2_CDF4(6386, 23344, 23348), AV2_PARA4(-1, -1, -1) },
      { AV2_CDF4(10945, 24709, 24714), AV2_PARA4(-1, -1, -1) },
      { AV2_CDF4(11517, 25230, 25258), AV2_PARA4(-1, -1, -1) },
      { AV2_CDF4(7563, 22176, 22180), AV2_PARA4(-1, -1, -1) },
      { AV2_CDF4(6629, 20955, 20966), AV2_PARA4(0, -1, -1) },
    };

static const aom_cdf_prob
    default_cwp_idx_cdf[MAX_CWP_CONTEXTS][MAX_CWP_NUM - 1][AV2_CDF_SIZE(2)] = {
      {
          { AV2_CDF2(21704), AV2_PARA2(-2, -1, -1) },
          { AV2_CDF2(15990), AV2_PARA2(-1, -1, -1) },
          { AV2_CDF2(12544), AV2_PARA2(-2, -1, -2) },
          { AV2_CDF2(25638), AV2_PARA2(-2, -2, -2) },
      },
      {
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      },
    };

static const aom_cdf_prob default_jmvd_scale_mode_cdf[AV2_CDF_SIZE(5)] = {
  AV2_CDF5(23180, 24894, 26548, 29872), AV2_PARA5(0, 0, -1)
};

static const aom_cdf_prob default_jmvd_amvd_scale_mode_cdf[AV2_CDF_SIZE(3)] = {
  AV2_CDF3(23178, 26812), AV2_PARA3(0, 0, 0)
};

static const aom_cdf_prob default_compound_type_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(16894), AV2_PARA2(-1, -1, -1)
};

static const aom_cdf_prob
    default_amvd_mode_cdf[NUM_AMVD_MODES][AMVD_MODE_CONTEXTS][AV2_CDF_SIZE(2)] = {
      {
          { AV2_CDF2(5980), AV2_PARA2(0, 1, 0) },
          { AV2_CDF2(6091), AV2_PARA2(0, 1, 0) },
          { AV2_CDF2(6237), AV2_PARA2(1, 1, 1) },
      },
      {
          { AV2_CDF2(861), AV2_PARA2(-1, -1, 0) },
          { AV2_CDF2(847), AV2_PARA2(-2, 0, 0) },
          { AV2_CDF2(1198), AV2_PARA2(-2, 1, -1) },
      },
      {
          { AV2_CDF2(456), AV2_PARA2(-2, 0, 1) },
          { AV2_CDF2(431), AV2_PARA2(-2, 1, 1) },
          { AV2_CDF2(849), AV2_PARA2(-2, 1, 1) },
      },
      {
          { AV2_CDF2(409), AV2_PARA2(-2, 1, 1) },
          { AV2_CDF2(385), AV2_PARA2(-2, 1, 1) },
          { AV2_CDF2(581), AV2_PARA2(-2, 1, 1) },
      },
      {
          { AV2_CDF2(16246), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(9696), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(8791), AV2_PARA2(0, 0, 0) },
      },
      {
          { AV2_CDF2(13199), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(10624), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(8586), AV2_PARA2(0, 0, -1) },
      },
      {
          { AV2_CDF2(5112), AV2_PARA2(0, 0, -1) },
          { AV2_CDF2(3920), AV2_PARA2(0, 0, 1) },
          { AV2_CDF2(3668), AV2_PARA2(0, 0, 1) },
      },
      {
          { AV2_CDF2(12017), AV2_PARA2(1, 0, -1) },
          { AV2_CDF2(10177), AV2_PARA2(1, 0, 0) },
          { AV2_CDF2(9184), AV2_PARA2(1, 0, 0) },
      },
      {
          { AV2_CDF2(12111), AV2_PARA2(0, -1, -1) },
          { AV2_CDF2(8056), AV2_PARA2(0, -1, 0) },
          { AV2_CDF2(6641), AV2_PARA2(-1, 0, -2) },
      },
    };

static const aom_cdf_prob default_wedge_quad_cdf[AV2_CDF_SIZE(4)] = {
  AV2_CDF4(6511, 18144, 27374), AV2_PARA4(0, 0, -1)
};

static const aom_cdf_prob default_wedge_angle_cdf[WEDGE_QUADS][AV2_CDF_SIZE(5)] = {
  { AV2_CDF5(10258, 15276, 19997, 26561), AV2_PARA5(0, -1, -1) },
  { AV2_CDF5(14039, 19183, 26143, 30047), AV2_PARA5(0, -1, -1) },
  { AV2_CDF5(19564, 22099, 25104, 29960), AV2_PARA5(0, 0, -1) },
  { AV2_CDF5(13808, 17950, 25715, 29008), AV2_PARA5(0, -1, -2) },
};

static const aom_cdf_prob default_wedge_dist_cdf[AV2_CDF_SIZE(4)] = {
  AV2_CDF4(8203, 16994, 21032), AV2_PARA4(1, 0, 0)
};

static const aom_cdf_prob default_wedge_dist_cdf2[AV2_CDF_SIZE(3)] = {
  AV2_CDF3(14463, 19115), AV2_PARA3(1, 0, 0)
};

static const aom_cdf_prob
    default_warp_interintra_cdf[BLOCK_SIZE_GROUPS][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(27980), AV2_PARA2(-2, -1, -1) },
      { AV2_CDF2(29163), AV2_PARA2(-2, -1, -1) },
      { AV2_CDF2(30658), AV2_PARA2(-2, -2, -2) },
    };

static const aom_cdf_prob
    default_interintra_cdf[BLOCK_SIZE_GROUPS][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(20569), AV2_PARA2(0, 0, -1) },
      { AV2_CDF2(17106), AV2_PARA2(0, 0, -1) },
      { AV2_CDF2(20948), AV2_PARA2(0, 0, -1) },
      { AV2_CDF2(25796), AV2_PARA2(-1, -1, -2) },
    };

static const aom_cdf_prob default_wedge_interintra_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(16758), AV2_PARA2(0, 0, 0)
};

static const aom_cdf_prob
    default_interintra_mode_cdf[BLOCK_SIZE_GROUPS][AV2_CDF_SIZE(4)] = {
      { AV2_CDF4(1819, 16131, 26802), AV2_PARA4(-1, -1, -2) },
      { AV2_CDF4(1442, 15840, 28441), AV2_PARA4(0, 0, -1) },
      { AV2_CDF4(1995, 15814, 28221), AV2_PARA4(0, -1, -2) },
      { AV2_CDF4(3564, 15440, 28048), AV2_PARA4(-1, -1, -2) },
    };

static const aom_cdf_prob
    default_warp_ref_idx0_cdf[WARP_REF_CONTEXTS][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(18903), AV2_PARA2(0, 0, 0) },
    };

static const aom_cdf_prob
    default_warp_ref_idx1_cdf[WARP_REF_CONTEXTS][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(24500), AV2_PARA2(1, 1, 0) },
    };

static const aom_cdf_prob
    default_warp_ref_idx2_cdf[WARP_REF_CONTEXTS][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(25360), AV2_PARA2(1, 0, 0) },
    };

static const aom_cdf_prob
    default_warp_causal_cdf[WARP_CAUSAL_MODE_CTX][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(14877), AV2_PARA2(0, 0, -1) },
      { AV2_CDF2(12801), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(6885), AV2_PARA2(0, -1, -1) },
      { AV2_CDF2(2987), AV2_PARA2(-1, -1, 0) },
    };

static const aom_cdf_prob
    default_warp_precision_idx_cdf[BLOCK_SIZES_ALL][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(31107), AV2_PARA2(-2, -1, -1) },
      { AV2_CDF2(27357), AV2_PARA2(-1, -1, -1) },
      { AV2_CDF2(26435), AV2_PARA2(-1, 0, -1) },
      { AV2_CDF2(22585), AV2_PARA2(-1, 0, -1) },
      { AV2_CDF2(20146), AV2_PARA2(-1, 0, -2) },
      { AV2_CDF2(18671), AV2_PARA2(-1, -1, -2) },
      { AV2_CDF2(18210), AV2_PARA2(-1, -1, -2) },
      { AV2_CDF2(17968), AV2_PARA2(-1, 0, 0) },
      { AV2_CDF2(17451), AV2_PARA2(-1, 1, 0) },
      { AV2_CDF2(16736), AV2_PARA2(-1, 0, 0) },
      { AV2_CDF2(16040), AV2_PARA2(-1, 0, 0) },
      { AV2_CDF2(15633), AV2_PARA2(-1, 0, 0) },
      { AV2_CDF2(13952), AV2_PARA2(-1, 0, 0) },
      { AV2_CDF2(14893), AV2_PARA2(-1, 0, 0) },
      { AV2_CDF2(13323), AV2_PARA2(-1, 0, 0) },
      { AV2_CDF2(11830), AV2_PARA2(-2, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(21783), AV2_PARA2(-1, 0, 1) },
      { AV2_CDF2(20345), AV2_PARA2(0, 1, 0) },
      { AV2_CDF2(19398), AV2_PARA2(-1, 0, 0) },
      { AV2_CDF2(17823), AV2_PARA2(-1, -2, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(20066), AV2_PARA2(-1, 0, 0) },
      { AV2_CDF2(18893), AV2_PARA2(0, -1, 0) },
    };

static const aom_cdf_prob
    default_warp_delta_param_cdf[WARP_DELTA_PARAM_TYPES][AV2_CDF_SIZE(8)] = {
      { AV2_CDF8(8995, 22970, 25406, 29673, 30295, 31670, 31863),
        AV2_PARA8(0, 0, 0) },
      { AV2_CDF8(13333, 24012, 26545, 30183, 30839, 31958, 32139),
        AV2_PARA8(0, 0, 0) },
    };

static const aom_cdf_prob
    default_warp_delta_param_high_cdf[WARP_DELTA_PARAM_TYPES][AV2_CDF_SIZE(8)] = {
      { AV2_CDF8(8959, 14388, 19825, 21810, 25035, 28077, 29469),
        AV2_PARA8(0, 1, 0) },
      { AV2_CDF8(9199, 14146, 19484, 21591, 24614, 28015, 29538),
        AV2_PARA8(0, 0, 0) },
    };

static const aom_cdf_prob default_warp_param_sign_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(14285), AV2_PARA2(1, 1, 1)
};

static const aom_cdf_prob
    default_warp_extend_cdf[WARP_EXTEND_CTX][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(20359), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(20310), AV2_PARA2(1, 0, 0) },
      { AV2_CDF2(21759), AV2_PARA2(1, 0, 0) },
    };

static const aom_cdf_prob default_warpmv_with_mvd_flag_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(18452), AV2_PARA2(0, 0, -1)
};

static const aom_cdf_prob default_bawp_cdf[BAWP_FLAG_CTXS][AV2_CDF_SIZE(2)] = {
  { AV2_CDF2(26456), AV2_PARA2(-1, -1, -1) },
  { AV2_CDF2(5121), AV2_PARA2(-1, -1, -1) },
};

static const aom_cdf_prob
    default_explicit_bawp_cdf[BAWP_SCALES_CTX_COUNT][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(26966), AV2_PARA2(-1, -1, -1) },
      { AV2_CDF2(15275), AV2_PARA2(0, -1, -1) },
      { AV2_CDF2(14613), AV2_PARA2(-1, -1, -1) },
    };

static const aom_cdf_prob default_explicit_bawp_scale_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(21998), AV2_PARA2(-1, -1, -2)
};

static const aom_cdf_prob default_tip_cdf[TIP_CONTEXTS][AV2_CDF_SIZE(2)] = {
  { AV2_CDF2(30898), AV2_PARA2(1, 1, 1) },
  { AV2_CDF2(19665), AV2_PARA2(0, 0, 0) },
  { AV2_CDF2(9477), AV2_PARA2(0, 1, 0) },
};

static const aom_cdf_prob default_tip_pred_mode_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(22129), AV2_PARA2(-1, -1, -1)
};

static const aom_cdf_prob default_palette_y_size_cdf[AV2_CDF_SIZE(7)] = {
  AV2_CDF7(8779, 15095, 20777, 24903, 27923, 30403), AV2_PARA7(-1, -1, -2)
};

static const aom_cdf_prob default_palette_uv_size_cdf[AV2_CDF_SIZE(7)] = {
  AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087), AV2_PARA7(0, 0, 0)
};

static const aom_cdf_prob
    default_identity_row_cdf_y[PALETTE_ROW_FLAG_CONTEXTS][AV2_CDF_SIZE(3)] = {
      { AV2_CDF3(22515, 25751), AV2_PARA3(-1, 0, 0) },
      { AV2_CDF3(4014, 5233), AV2_PARA3(-1, -1, -1) },
      { AV2_CDF3(3548, 4163), AV2_PARA3(-1, -1, 1) },
      { AV2_CDF3(12999, 32756), AV2_PARA3(-2, -1, -1) },
    };

static const aom_cdf_prob
    default_identity_row_cdf_uv[PALETTE_ROW_FLAG_CONTEXTS][AV2_CDF_SIZE(3)] = {
      { AV2_CDF3(10923, 21845), AV2_PARA3(0, 0, 0) },
      { AV2_CDF3(10923, 21845), AV2_PARA3(0, 0, 0) },
      { AV2_CDF3(10923, 21845), AV2_PARA3(0, 0, 0) },
      { AV2_CDF3(10923, 21845), AV2_PARA3(0, 0, 0) },
    };

static const aom_cdf_prob default_palette_y_color_index_cdf
    [PALETTE_SIZES][PALETTE_COLOR_INDEX_CONTEXTS][AV2_CDF_SIZE(8)] = {
      {
          { AV2_CDF2(28140), AV2_PARA2(1, 1, 0) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(8582), AV2_PARA2(0, -1, -1) },
          { AV2_CDF2(27413), AV2_PARA2(-1, -1, -2) },
          { AV2_CDF2(30429), AV2_PARA2(1, 1, 1) },
      },
      {
          { AV2_CDF3(25350, 29026), AV2_PARA3(1, 1, 0) },
          { AV2_CDF3(11363, 25273), AV2_PARA3(0, -1, -2) },
          { AV2_CDF3(6841, 28579), AV2_PARA3(0, 0, -1) },
          { AV2_CDF3(21350, 26012), AV2_PARA3(0, -1, -1) },
          { AV2_CDF3(30573, 31646), AV2_PARA3(1, 1, 1) },
      },
      {
          { AV2_CDF4(23706, 26962, 29060), AV2_PARA4(0, 0, 0) },
          { AV2_CDF4(9976, 22516, 27382), AV2_PARA4(0, 0, -1) },
          { AV2_CDF4(6691, 25460, 29234), AV2_PARA4(0, -1, -1) },
          { AV2_CDF4(18909, 23925, 28403), AV2_PARA4(-1, -1, -1) },
          { AV2_CDF4(30308, 31076, 31818), AV2_PARA4(1, 1, 1) },
      },
      {
          { AV2_CDF5(24116, 26957, 28486, 29941), AV2_PARA5(0, 0, 0) },
          { AV2_CDF5(9568, 20472, 24294, 28942), AV2_PARA5(1, -1, -1) },
          { AV2_CDF5(5706, 25243, 28040, 30406), AV2_PARA5(1, 0, -1) },
          { AV2_CDF5(20105, 22982, 27024, 28911), AV2_PARA5(-1, -1, -1) },
          { AV2_CDF5(30897, 31342, 31766, 32199), AV2_PARA5(1, 1, 1) },
      },
      {
          { AV2_CDF6(20824, 24227, 25926, 27459, 29266), AV2_PARA6(1, 0, 0) },
          { AV2_CDF6(8141, 18989, 21599, 26182, 28576), AV2_PARA6(1, 0, 0) },
          { AV2_CDF6(5252, 24340, 26450, 28438, 30625), AV2_PARA6(1, 0, 0) },
          { AV2_CDF6(19519, 22695, 25587, 26972, 28423), AV2_PARA6(0, -1, -1) },
          { AV2_CDF6(30383, 30890, 31247, 31653, 32150), AV2_PARA6(1, 0, 1) },
      },
      {
          { AV2_CDF7(21628, 24512, 25873, 27054, 28131, 29539),
            AV2_PARA7(1, -1, 0) },
          { AV2_CDF7(8028, 18264, 20613, 25424, 27112, 28906),
            AV2_PARA7(1, 1, 0) },
          { AV2_CDF7(6489, 22242, 24461, 26394, 28350, 30510),
            AV2_PARA7(1, 0, 0) },
          { AV2_CDF7(22048, 24429, 26990, 27944, 28417, 29574),
            AV2_PARA7(1, 0, -1) },
          { AV2_CDF7(30801, 31205, 31472, 31728, 32005, 32305),
            AV2_PARA7(1, 1, 1) },
      },
      {
          { AV2_CDF8(22471, 25083, 25984, 26893, 27654, 28750, 29903),
            AV2_PARA8(1, 1, 1) },
          { AV2_CDF8(7542, 17057, 19151, 23550, 25459, 27066, 28804),
            AV2_PARA8(1, 1, 0) },
          { AV2_CDF8(7582, 20437, 22728, 24622, 26515, 28579, 30632),
            AV2_PARA8(1, 1, 0) },
          { AV2_CDF8(22102, 24144, 26916, 28151, 28846, 29212, 30153),
            AV2_PARA8(0, 0, 0) },
          { AV2_CDF8(30524, 30887, 31156, 31393, 31626, 31911, 32281),
            AV2_PARA8(1, 1, 1) },
      },
    };

static const aom_cdf_prob default_palette_y_mode_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(30045), AV2_PARA2(-2, -2, -2)
};

static const aom_cdf_prob default_palette_uv_mode_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(16384), AV2_PARA2(0, 0, 0)
};

static const aom_cdf_prob
    default_comp_inter_cdf[COMP_INTER_CONTEXTS][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(26924), AV2_PARA2(0, 1, 0) },
      { AV2_CDF2(25000), AV2_PARA2(0, 0, -1) },
      { AV2_CDF2(17949), AV2_PARA2(0, 0, -1) },
      { AV2_CDF2(13581), AV2_PARA2(0, -1, -1) },
      { AV2_CDF2(7034), AV2_PARA2(0, 0, 0) },
    };

static const aom_cdf_prob
    default_single_ref_cdf[REF_CONTEXTS][INTER_REFS_PER_FRAME - 1]
                          [AV2_CDF_SIZE(2)] = {
                            {
                                { AV2_CDF2(26469), AV2_PARA2(0, 0, 0) },
                                { AV2_CDF2(28870), AV2_PARA2(-1, -1, 0) },
                                { AV2_CDF2(29662), AV2_PARA2(0, 0, -1) },
                                { AV2_CDF2(29867), AV2_PARA2(0, -1, -1) },
                                { AV2_CDF2(29772), AV2_PARA2(0, -1, -1) },
                                { AV2_CDF2(29776), AV2_PARA2(-1, 0, -1) },
                            },
                            {
                                { AV2_CDF2(13631), AV2_PARA2(0, -1, -1) },
                                { AV2_CDF2(18185), AV2_PARA2(-1, -2, -2) },
                                { AV2_CDF2(19992), AV2_PARA2(-1, -1, -2) },
                                { AV2_CDF2(18462), AV2_PARA2(-2, -2, -2) },
                                { AV2_CDF2(17451), AV2_PARA2(-1, -2, -2) },
                                { AV2_CDF2(11578), AV2_PARA2(-2, -2, -2) },
                            },
                            {
                                { AV2_CDF2(2599), AV2_PARA2(0, 0, 0) },
                                { AV2_CDF2(5203), AV2_PARA2(-1, -1, -1) },
                                { AV2_CDF2(5185), AV2_PARA2(-1, -1, -1) },
                                { AV2_CDF2(3671), AV2_PARA2(-1, -1, -1) },
                                { AV2_CDF2(3954), AV2_PARA2(0, -1, -1) },
                                { AV2_CDF2(1633), AV2_PARA2(0, -1, 0) },
                            },
                          };

static const aom_cdf_prob
    default_comp_ref0_cdf[REF_CONTEXTS][INTER_REFS_PER_FRAME - 1]
                         [AV2_CDF_SIZE(2)] = {
                           {
                               { AV2_CDF2(9272), AV2_PARA2(-1, -1, -2) },
                               { AV2_CDF2(17175), AV2_PARA2(-2, -2, -2) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                           },
                           {
                               { AV2_CDF2(1385), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(4439), AV2_PARA2(-1, -1, -1) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                           },
                           {
                               { AV2_CDF2(521), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(1854), AV2_PARA2(-1, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                           },
                         };

static const aom_cdf_prob
    default_comp_ref1_cdf[REF_CONTEXTS][COMPREF_BIT_TYPES]
                         [INTER_REFS_PER_FRAME - 1][AV2_CDF_SIZE(2)] = {
                           {
                               {
                                   { AV2_CDF2(30729), AV2_PARA2(1, 0, 0) },
                                   { AV2_CDF2(29403), AV2_PARA2(0, -1, 0) },
                                   { AV2_CDF2(29037), AV2_PARA2(0, -1, -1) },
                                   { AV2_CDF2(29355), AV2_PARA2(-1, -1, -1) },
                                   { AV2_CDF2(28573), AV2_PARA2(0, -1, 0) },
                                   { AV2_CDF2(27396), AV2_PARA2(0, -1, -2) },
                               },
                               {
                                   { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                                   { AV2_CDF2(19315), AV2_PARA2(-1, -1, -1) },
                                   { AV2_CDF2(27821), AV2_PARA2(0, -1, -1) },
                                   { AV2_CDF2(27892), AV2_PARA2(-1, -1, -1) },
                                   { AV2_CDF2(28695), AV2_PARA2(-1, -1, 0) },
                                   { AV2_CDF2(29637), AV2_PARA2(-2, 0, -1) },
                               },
                           },
                           {
                               {
                                   { AV2_CDF2(30432), AV2_PARA2(0, 0, 0) },
                                   { AV2_CDF2(20290), AV2_PARA2(-1, -1, -1) },
                                   { AV2_CDF2(19855), AV2_PARA2(-1, -2, -2) },
                                   { AV2_CDF2(18567), AV2_PARA2(-2, -2, -2) },
                                   { AV2_CDF2(18331), AV2_PARA2(-1, -2, -2) },
                                   { AV2_CDF2(14241), AV2_PARA2(-2, -2, -2) },
                               },
                               {
                                   { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                                   { AV2_CDF2(5725), AV2_PARA2(-1, -1, -1) },
                                   { AV2_CDF2(13420), AV2_PARA2(-1, -1, -1) },
                                   { AV2_CDF2(12780), AV2_PARA2(-1, -1, -2) },
                                   { AV2_CDF2(10781), AV2_PARA2(-2, -2, -2) },
                                   { AV2_CDF2(6424), AV2_PARA2(-2, -2, -2) },
                               },
                           },
                           {
                               {
                                   { AV2_CDF2(11634), AV2_PARA2(-1, -1, -1) },
                                   { AV2_CDF2(10093), AV2_PARA2(-1, -1, -1) },
                                   { AV2_CDF2(6065), AV2_PARA2(-1, -1, -1) },
                                   { AV2_CDF2(5408), AV2_PARA2(-1, -1, -1) },
                                   { AV2_CDF2(6411), AV2_PARA2(-1, -1, -1) },
                                   { AV2_CDF2(4075), AV2_PARA2(-1, -1, 0) },
                               },
                               {
                                   { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                                   { AV2_CDF2(898), AV2_PARA2(0, 1, 0) },
                                   { AV2_CDF2(3127), AV2_PARA2(0, 0, -1) },
                                   { AV2_CDF2(1775), AV2_PARA2(0, -1, 0) },
                                   { AV2_CDF2(1217), AV2_PARA2(0, -1, -1) },
                                   { AV2_CDF2(591), AV2_PARA2(0, -1, 0) },
                               },
                           },
                         };

static const aom_cdf_prob
    default_comp_group_idx_cdfs[COMP_GROUP_IDX_CONTEXTS][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(17510), AV2_PARA2(-1, 0, -1) },
      { AV2_CDF2(10382), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(8336), AV2_PARA2(-1, -1, -1) },
      { AV2_CDF2(6054), AV2_PARA2(0, 0, -1) },
      { AV2_CDF2(5764), AV2_PARA2(0, -1, 0) },
      { AV2_CDF2(7154), AV2_PARA2(0, -2, 0) },
      { AV2_CDF2(15013), AV2_PARA2(-1, 0, -1) },
      { AV2_CDF2(8426), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(8278), AV2_PARA2(0, 0, -1) },
      { AV2_CDF2(4856), AV2_PARA2(0, 0, -1) },
      { AV2_CDF2(3464), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(5295), AV2_PARA2(0, 0, 0) },
    };

static const aom_cdf_prob
    default_skip_mode_cdfs[SKIP_MODE_CONTEXTS][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(30964), AV2_PARA2(1, 1, 1) },
      { AV2_CDF2(21769), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(12484), AV2_PARA2(0, 1, 0) },
    };

static const aom_cdf_prob
    default_lossless_tx_size_cdf[BLOCK_SIZE_GROUPS][LOSSLESS_TX_SIZE_PLANE_CTXS]
                                [AV2_CDF_SIZE(2)] = {
                                  {
                                      { AV2_CDF2(16384), AV2_PARA2(0, 0, -1) },
                                      { AV2_CDF2(16384), AV2_PARA2(0, 0, -1) },
                                  },
                                  {
                                      { AV2_CDF2(16384), AV2_PARA2(1, 0, 0) },
                                      { AV2_CDF2(16384), AV2_PARA2(1, 0, 0) },
                                  },
                                  {
                                      { AV2_CDF2(16384), AV2_PARA2(1, 0, 0) },
                                      { AV2_CDF2(16384), AV2_PARA2(1, 0, 0) },
                                  },
                                  {
                                      { AV2_CDF2(16384), AV2_PARA2(1, 0, 0) },
                                      { AV2_CDF2(16384), AV2_PARA2(1, 0, 0) },
                                  },
                                };

static const aom_cdf_prob default_lossless_inter_tx_type_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(16384), AV2_PARA2(0, 0, 0)
};

static const aom_cdf_prob default_skip_txfm_cdfs[SKIP_CONTEXTS][AV2_CDF_SIZE(2)] = {
  { AV2_CDF2(25865), AV2_PARA2(-1, 0, 0) },
  { AV2_CDF2(14316), AV2_PARA2(0, 0, 0) },
  { AV2_CDF2(4598), AV2_PARA2(0, 0, 0) },
  { AV2_CDF2(25612), AV2_PARA2(0, -1, -1) },
  { AV2_CDF2(12366), AV2_PARA2(0, 0, -1) },
  { AV2_CDF2(3320), AV2_PARA2(1, 1, 0) },
};

static const aom_cdf_prob default_bru_mode_cdf[AV2_CDF_SIZE(3)] = {
  AV2_CDF3(4124, 16615), AV2_PARA3(0, 0, 0)
};

static const aom_cdf_prob
    default_intra_inter_cdf[INTRA_INTER_CONTEXTS][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(1522), AV2_PARA2(0, 0, -1) },
      { AV2_CDF2(14381), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(10455), AV2_PARA2(-1, 0, 0) },
      { AV2_CDF2(27796), AV2_PARA2(0, 0, 0) },
    };

static const aom_cdf_prob default_intrabc_cdf[INTRABC_CONTEXTS][AV2_CDF_SIZE(2)] = {
  { AV2_CDF2(32085), AV2_PARA2(0, -1, 0) },
  { AV2_CDF2(15172), AV2_PARA2(-1, -1, 0) },
  { AV2_CDF2(4503), AV2_PARA2(0, 0, 0) },
};

static const aom_cdf_prob default_intrabc_mode_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(29993), AV2_PARA2(0, -1, -1)
};

static const aom_cdf_prob
    default_intrabc_bv_precision_cdf[NUM_BV_PRECISION_CONTEXTS][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(19778), AV2_PARA2(-1, -1, -1) },
    };

static const aom_cdf_prob default_switchable_flex_restore_cdf
    [MAX_LR_FLEX_SWITCHABLE_BITS][MAX_LR_FLEX_MB_PLANE][AV2_CDF_SIZE(2)] = {
      {
          { AV2_CDF2(25542), AV2_PARA2(-2, -2, -2) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      },
      {
          { AV2_CDF2(25550), AV2_PARA2(-1, -2, -2) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      },
      {
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      },
    };

static const aom_cdf_prob default_wienerns_restore_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(9051), AV2_PARA2(-1, -1, -2)
};

static const aom_cdf_prob
    default_wienerns_length_cdf[WIENERNS_LENGTH_CTXS][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(4898), AV2_PARA2(-2, -1, -1) },
      { AV2_CDF2(6088), AV2_PARA2(-1, 0, -1) },
    };

static const aom_cdf_prob default_wienerns_uv_sym_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(29286), AV2_PARA2(-2, 1, 0)
};

static const aom_cdf_prob
    default_wienerns_4part_cdf[WIENERNS_4PART_CTX_MAX][AV2_CDF_SIZE(4)] = {
      { AV2_CDF4(16557, 24352, 29677), AV2_PARA4(0, -1, -1) },
    };

static const aom_cdf_prob default_pc_wiener_restore_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(12799), AV2_PARA2(-1, 0, 0)
};

static const aom_cdf_prob
    default_ccso_cdf[CCSO_PLANES][CCSO_CONTEXT][AV2_CDF_SIZE(2)] = {
      {
          { AV2_CDF2(18469), AV2_PARA2(-2, -2, -2) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(4949), AV2_PARA2(-1, -2, -2) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      },
      {
          { AV2_CDF2(23470), AV2_PARA2(-1, -2, -2) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(6666), AV2_PARA2(-1, -2, -2) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      },
      {
          { AV2_CDF2(22914), AV2_PARA2(-1, -2, -2) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(6993), AV2_PARA2(-1, -2, -2) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      },
    };

static const aom_cdf_prob
    default_cdef_strength_index0_cdf[CDEF_STRENGTH_INDEX0_CTX][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(29034), AV2_PARA2(-1, -1, -2) },
      { AV2_CDF2(16472), AV2_PARA2(-1, -1, -2) },
      { AV2_CDF2(5751), AV2_PARA2(-1, -1, -2) },
      { AV2_CDF2(3115), AV2_PARA2(-1, -1, -1) },
    };

static const aom_cdf_prob default_cdef_cdf[CDEF_STRENGTHS_NUM -
                                           1][AV2_CDF_SIZE(7)] = {
  { AV2_CDF2(17708), AV2_PARA2(-1, -1, -2) },
  { AV2_CDF3(13413, 24899), AV2_PARA3(-1, -1, -2) },
  { AV2_CDF4(10588, 19866, 26664), AV2_PARA4(-1, -2, -2) },
  { AV2_CDF5(10131, 17874, 23876, 28766), AV2_PARA5(-1, -2, -2) },
  { AV2_CDF6(8363, 15451, 20811, 25453, 29393), AV2_PARA6(-1, -1, -2) },
  { AV2_CDF7(7372, 13867, 18969, 23278, 26977, 30156), AV2_PARA7(-1, -1, -2) },
};

static const aom_cdf_prob default_gdf_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(14593), AV2_PARA2(-1, -1, -2)
};

static const aom_cdf_prob
    default_mrl_index_cdf[MRL_INDEX_CONTEXTS][AV2_CDF_SIZE(4)] = {
      { AV2_CDF4(29573, 31193, 32023), AV2_PARA4(1, 0, 1) },
      { AV2_CDF4(21812, 27066, 30279), AV2_PARA4(1, 0, 0) },
      { AV2_CDF4(16076, 23806, 28762), AV2_PARA4(0, 0, -1) },
    };

static const aom_cdf_prob
    default_multi_line_mrl_cdf[MRL_INDEX_CONTEXTS][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(19678), AV2_PARA2(0, -1, -1) },
      { AV2_CDF2(12287), AV2_PARA2(0, -1, -1) },
      { AV2_CDF2(9574), AV2_PARA2(0, -1, -1) },
    };

static const aom_cdf_prob default_dpcm_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(16384), AV2_PARA2(0, 0, 0)
};

static const aom_cdf_prob default_dpcm_vert_horz_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(16384), AV2_PARA2(0, 0, 0)
};

static const aom_cdf_prob default_dpcm_uv_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(16384), AV2_PARA2(0, 0, 0)
};

static const aom_cdf_prob default_dpcm_uv_vert_horz_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(16384), AV2_PARA2(0, 0, 0)
};

static const aom_cdf_prob
    default_fsc_mode_cdf[FSC_MODE_CONTEXTS][FSC_BSIZE_CONTEXTS][AV2_CDF_SIZE(2)] = {
      {
          { AV2_CDF2(30503), AV2_PARA2(0, 0, 1) },
          { AV2_CDF2(31244), AV2_PARA2(0, 0, 1) },
          { AV2_CDF2(32254), AV2_PARA2(1, 0, 1) },
          { AV2_CDF2(32324), AV2_PARA2(1, 1, 1) },
          { AV2_CDF2(32582), AV2_PARA2(1, 1, 1) },
          { AV2_CDF2(32691), AV2_PARA2(1, 1, 1) },
      },
      {
          { AV2_CDF2(27437), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(27242), AV2_PARA2(1, 1, 0) },
          { AV2_CDF2(28040), AV2_PARA2(1, 0, -1) },
          { AV2_CDF2(27589), AV2_PARA2(1, 0, -1) },
          { AV2_CDF2(27234), AV2_PARA2(0, -1, -2) },
          { AV2_CDF2(23583), AV2_PARA2(-2, -2, -2) },
      },
      {
          { AV2_CDF2(26068), AV2_PARA2(1, 0, 0) },
          { AV2_CDF2(22635), AV2_PARA2(1, 0, 0) },
          { AV2_CDF2(22069), AV2_PARA2(0, -1, -1) },
          { AV2_CDF2(19218), AV2_PARA2(-1, -1, -2) },
          { AV2_CDF2(13701), AV2_PARA2(-1, -1, -1) },
          { AV2_CDF2(4636), AV2_PARA2(-1, -2, 1) },
      },
      {
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(32016), AV2_PARA2(0, 1, 0) },
          { AV2_CDF2(32403), AV2_PARA2(1, 1, 1) },
          { AV2_CDF2(32583), AV2_PARA2(0, 1, 0) },
          { AV2_CDF2(32683), AV2_PARA2(1, 0, -1) },
      },
    };

static const aom_cdf_prob default_cfl_index_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(12507), AV2_PARA2(-1, -1, 0)
};

static const aom_cdf_prob default_cfl_mhccp_switch_cdf[AV2_CDF_SIZE(2)] = {
  AV2_CDF2(15499), AV2_PARA2(-1, -1, 0)
};

static const aom_cdf_prob default_cfl_sign_cdf[AV2_CDF_SIZE(8)] = {
  AV2_CDF8(2421, 4332, 11256, 12766, 21386, 28725, 32087), AV2_PARA8(-2, -2, -2)
};

static const aom_cdf_prob
    default_cfl_alpha_cdf[CFL_ALPHA_CONTEXTS][AV2_CDF_SIZE(8)] = {
      { AV2_CDF8(21679, 25305, 30646, 31512, 32537, 32646, 32696),
        AV2_PARA8(-2, -2, -2) },
      { AV2_CDF8(8262, 16302, 24082, 29422, 31398, 32286, 32525),
        AV2_PARA8(-2, -2, -2) },
      { AV2_CDF8(17235, 26166, 30378, 31305, 32373, 32549, 32668),
        AV2_PARA8(-2, -2, -2) },
      { AV2_CDF8(17618, 25732, 27865, 30338, 31125, 31522, 32238),
        AV2_PARA8(-2, -2, -2) },
      { AV2_CDF8(17542, 23066, 27907, 28728, 30702, 31165, 31435),
        AV2_PARA8(-2, -2, -2) },
      { AV2_CDF8(17675, 24802, 30468, 30783, 31841, 32264, 32422),
        AV2_PARA8(-2, -2, -2) },
    };

static const aom_cdf_prob default_y_mode_set_cdf[AV2_CDF_SIZE(4)] = {
  AV2_CDF4(28863, 31022, 31724), AV2_PARA4(1, 1, 1)
};

static const aom_cdf_prob
    default_y_mode_idx_cdf[Y_MODE_CONTEXTS][AV2_CDF_SIZE(8)] = {
      { AV2_CDF8(15175, 20075, 21728, 24098, 26405, 27655, 28860),
        AV2_PARA8(0, -1, 0) },
      { AV2_CDF8(10114, 14957, 16815, 19127, 20147, 25583, 27169),
        AV2_PARA8(0, 0, 0) },
      { AV2_CDF8(5636, 9004, 10456, 12122, 12744, 20325, 25607),
        AV2_PARA8(0, 0, 0) },
    };

static const aom_cdf_prob
    default_y_mode_idx_offset_cdf[Y_MODE_CONTEXTS][AV2_CDF_SIZE(6)] = {
      { AV2_CDF6(12743, 18172, 20194, 23648, 26419), AV2_PARA6(0, -1, -1) },
      { AV2_CDF6(8976, 16084, 20827, 24595, 28496), AV2_PARA6(1, 0, 0) },
      { AV2_CDF6(8784, 14556, 19710, 24903, 28724), AV2_PARA6(1, 0, 0) },
    };

static const aom_cdf_prob default_uv_mode_cdf[UV_MODE_CONTEXTS][AV2_CDF_SIZE(8)] = {
  { AV2_CDF8(9363, 20957, 22865, 24753, 26411, 27983, 30428),
    AV2_PARA8(-1, -1, -1) },
  { AV2_CDF8(21282, 23610, 28208, 29311, 30348, 31158, 31491),
    AV2_PARA8(-1, -1, 0) },
};

static const aom_cdf_prob
    default_switchable_interp_cdf[SWITCHABLE_FILTER_CONTEXTS][AV2_CDF_SIZE(3)] = {
      { AV2_CDF3(29975, 32748), AV2_PARA3(-1, 0, 0) },
      { AV2_CDF3(2076, 32703), AV2_PARA3(1, 0, 0) },
      { AV2_CDF3(19, 1768), AV2_PARA3(0, 1, 0) },
      { AV2_CDF3(17314, 27415), AV2_PARA3(-1, -1, -1) },
      { AV2_CDF3(31286, 31994), AV2_PARA3(0, 0, 0) },
      { AV2_CDF3(9581, 32608), AV2_PARA3(-1, -1, -1) },
      { AV2_CDF3(535, 1036), AV2_PARA3(0, 0, 0) },
      { AV2_CDF3(24819, 27722), AV2_PARA3(-1, -1, -1) },
      { AV2_CDF3(10923, 21845), AV2_PARA3(0, 0, 0) },
      { AV2_CDF3(10923, 21845), AV2_PARA3(0, 0, 0) },
      { AV2_CDF3(10923, 21845), AV2_PARA3(0, 0, 0) },
      { AV2_CDF3(10923, 21845), AV2_PARA3(0, 0, 0) },
      { AV2_CDF3(10923, 21845), AV2_PARA3(0, 0, 0) },
      { AV2_CDF3(10923, 21845), AV2_PARA3(0, 0, 0) },
      { AV2_CDF3(10923, 21845), AV2_PARA3(0, 0, 0) },
      { AV2_CDF3(10923, 21845), AV2_PARA3(0, 0, 0) },
    };

static const aom_cdf_prob default_delta_q_cdf[AV2_CDF_SIZE(8)] = {
  AV2_CDF8(16594, 23325, 26424, 28225, 29358, 30099, 30613),
  AV2_PARA8(-2, -1, -1)
};

static const aom_cdf_prob
    default_intra_ext_tx_cdf[EXT_TX_SETS_INTRA][EXT_TX_SIZES][AV2_CDF_SIZE(16)] = {
      {
          { 0 },
          { 0 },
          { 0 },
          { 0 },
      },
      {
          { AV2_CDF7(5026, 16816, 19974, 23404, 26845, 30499),
            AV2_PARA7(0, -1, 1) },
          { AV2_CDF7(14910, 25257, 26964, 29323, 30237, 31535),
            AV2_PARA7(0, 0, 0) },
          { AV2_CDF7(13759, 26108, 27688, 29793, 30265, 31576),
            AV2_PARA7(-1, -2, 0) },
          { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
            AV2_PARA7(0, 0, 0) },
      },
      {
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      },
    };

static const aom_cdf_prob
    default_intra_ext_tx_short_side_cdf[EXT_TX_SIZES][AV2_CDF_SIZE(4)] = {
      { AV2_CDF4(10692, 26586, 29231), AV2_PARA4(-1, -1, -1) },
      { AV2_CDF4(26700, 32160, 32748), AV2_PARA4(0, -1, -1) },
      { AV2_CDF4(26915, 32411, 32748), AV2_PARA4(0, -1, -1) },
      { AV2_CDF4(8192, 16384, 24576), AV2_PARA4(0, 0, 0) },
    };

static const aom_cdf_prob default_inter_tx_type_set_cdf
    [INTER_TX_TYPE_SIGNALING_SETS][EOB_TX_CTXS][EXT_TX_SIZES][AV2_CDF_SIZE(2)] = {
      {
          {
              { AV2_CDF2(14122), AV2_PARA2(-1, 0, 0) },
              { AV2_CDF2(8962), AV2_PARA2(-1, -1, -1) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          },
          {
              { AV2_CDF2(16423), AV2_PARA2(-1, 0, -1) },
              { AV2_CDF2(23446), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          },
          {
              { AV2_CDF2(23352), AV2_PARA2(-1, 0, 0) },
              { AV2_CDF2(17069), AV2_PARA2(-2, -1, -1) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          },
      },
      {
          {
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(20835), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          },
          {
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(30720), AV2_PARA2(0, 0, 1) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          },
          {
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(28857), AV2_PARA2(0, 1, -2) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          },
      },
    };

static const aom_cdf_prob default_inter_tx_type_idx_cdf
    [INTER_TX_TYPE_SIGNALING_SETS][EOB_TX_CTXS][AV2_CDF_SIZE(8)] = {
      {
          { AV2_CDF8(8914, 10732, 12270, 14822, 17128, 19586, 21964),
            AV2_PARA8(-1, 0, 0) },
          { AV2_CDF8(1160, 1555, 1972, 3414, 3962, 5151, 5908),
            AV2_PARA8(-1, -1, -1) },
          { AV2_CDF8(22819, 24338, 25592, 27001, 28395, 29648, 30990),
            AV2_PARA8(-1, 0, 0) },
      },
      {
          { AV2_CDF8(1140, 1725, 2324, 14653, 19072, 23618, 28109),
            AV2_PARA8(-1, -1, -1) },
          { AV2_CDF8(58, 261, 587, 32317, 32556, 32626, 32708),
            AV2_PARA8(0, 0, -2) },
          { AV2_CDF8(17404, 17669, 18403, 24052, 26393, 28506, 30676),
            AV2_PARA8(-2, -1, -2) },
      },
    };

static const aom_cdf_prob
    default_inter_tx_type_offset_1_cdf[EOB_TX_CTXS][AV2_CDF_SIZE(8)] = {
      { AV2_CDF8(3121, 6470, 9191, 12280, 17811, 22588, 27697),
        AV2_PARA8(1, 1, 1) },
      { AV2_CDF8(338, 377, 571, 743, 7932, 11860, 17524), AV2_PARA8(0, 0, -1) },
      { AV2_CDF8(3314, 7625, 10639, 14206, 19363, 23456, 28033),
        AV2_PARA8(1, 1, 1) },
    };

static const aom_cdf_prob
    default_inter_tx_type_offset_2_cdf[EOB_TX_CTXS][AV2_CDF_SIZE(4)] = {
      { AV2_CDF4(8669, 16533, 24855), AV2_PARA4(1, 1, 1) },
      { AV2_CDF4(9441, 16413, 25276), AV2_PARA4(1, 1, 0) },
      { AV2_CDF4(8767, 17611, 24876), AV2_PARA4(0, -2, 0) },
    };

static const aom_cdf_prob default_inter_ext_tx_cdf
    [EXT_TX_SETS_INTER][EOB_TX_CTXS][EXT_TX_SIZES][AV2_CDF_SIZE(16)] = {
      {
          {
              { 0 },
              { 0 },
              { 0 },
              { 0 },
          },
          {
              { 0 },
              { 0 },
              { 0 },
              { 0 },
          },
          {
              { 0 },
              { 0 },
              { 0 },
              { 0 },
          },
      },
      {
          {
              { 0 },
              { 0 },
              { 0 },
              { 0 },
          },
          {
              { 0 },
              { 0 },
              { 0 },
              { 0 },
          },
          {
              { 0 },
              { 0 },
              { 0 },
              { 0 },
          },
      },
      {
          {
              { 0 },
              { 0 },
              { 0 },
              { 0 },
          },
          {
              { 0 },
              { 0 },
              { 0 },
              { 0 },
          },
          {
              { 0 },
              { 0 },
              { 0 },
              { 0 },
          },
      },
      {
          {
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          },
          {
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          },
          {
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          },
      },
      {
          {
              { AV2_CDF4(8192, 16384, 24576), AV2_PARA4(0, 0, 0) },
              { AV2_CDF4(8192, 16384, 24576), AV2_PARA4(0, 0, 0) },
              { AV2_CDF4(8192, 16384, 24576), AV2_PARA4(0, 0, 0) },
              { AV2_CDF4(8192, 16384, 24576), AV2_PARA4(0, 0, 0) },
          },
          {
              { AV2_CDF4(8192, 16384, 24576), AV2_PARA4(0, 0, 0) },
              { AV2_CDF4(8192, 16384, 24576), AV2_PARA4(0, 0, 0) },
              { AV2_CDF4(8192, 16384, 24576), AV2_PARA4(0, 0, 0) },
              { AV2_CDF4(8192, 16384, 24576), AV2_PARA4(0, 0, 0) },
          },
          {
              { AV2_CDF4(8192, 16384, 24576), AV2_PARA4(0, 0, 0) },
              { AV2_CDF4(8192, 16384, 24576), AV2_PARA4(0, 0, 0) },
              { AV2_CDF4(8192, 16384, 24576), AV2_PARA4(0, 0, 0) },
              { AV2_CDF4(8192, 16384, 24576), AV2_PARA4(0, 0, 0) },
          },
      },
    };

static const aom_cdf_prob
    default_inter_ext_tx_short_side_cdf[EOB_TX_CTXS][EXT_TX_SIZES][AV2_CDF_SIZE(
        4)] = {
      {
          { AV2_CDF4(8347, 20254, 24536), AV2_PARA4(-1, -1, -1) },
          { AV2_CDF4(15994, 26294, 32748), AV2_PARA4(0, 0, -1) },
          { AV2_CDF4(21212, 27810, 32748), AV2_PARA4(0, 0, -1) },
          { AV2_CDF4(8192, 16384, 24576), AV2_PARA4(0, 0, 0) },
      },
      {
          { AV2_CDF4(21815, 25084, 26230), AV2_PARA4(-2, -2, -2) },
          { AV2_CDF4(29354, 31747, 32748), AV2_PARA4(-1, -2, -2) },
          { AV2_CDF4(31614, 32529, 32748), AV2_PARA4(-1, -2, -1) },
          { AV2_CDF4(8192, 16384, 24576), AV2_PARA4(0, 0, 0) },
      },
      {
          { AV2_CDF4(10501, 20590, 24181), AV2_PARA4(-1, -1, -1) },
          { AV2_CDF4(17596, 26388, 32748), AV2_PARA4(-1, -2, -2) },
          { AV2_CDF4(15407, 26475, 32732), AV2_PARA4(-2, -2, 0) },
          { AV2_CDF4(8192, 16384, 24576), AV2_PARA4(0, 0, 0) },
      },
    };

static const aom_cdf_prob default_tx_ext_32_cdf[TX_EXT_32_CTXS][AV2_CDF_SIZE(2)] = {
  { AV2_CDF2(36), AV2_PARA2(0, 0, 0) },
  { AV2_CDF2(128), AV2_PARA2(-1, -1, -1) },
};

static const aom_cdf_prob
    default_stx_cdf[STX_PRED_CTXS][TX_SIZES][AV2_CDF_SIZE(4)] = {
      {
          { AV2_CDF4(4486, 15589, 26440), AV2_PARA4(1, 0, 0) },
          { AV2_CDF4(2357, 9504, 16641), AV2_PARA4(0, -1, 0) },
          { AV2_CDF4(1364, 8034, 14431), AV2_PARA4(0, 0, 0) },
          { AV2_CDF4(2472, 8725, 13853), AV2_PARA4(1, 0, -1) },
          { AV2_CDF4(7523, 11681, 14783), AV2_PARA4(0, 0, -1) },
      },
      {
          { AV2_CDF4(8192, 16384, 24576), AV2_PARA4(0, 0, 0) },
          { AV2_CDF4(8192, 16384, 24576), AV2_PARA4(0, 0, 0) },
          { AV2_CDF4(10490, 15394, 22206), AV2_PARA4(-1, -1, -1) },
          { AV2_CDF4(13967, 16953, 23109), AV2_PARA4(-1, -1, -1) },
          { AV2_CDF4(20191, 21727, 25818), AV2_PARA4(-1, -1, -2) },
      },
    };

static const aom_cdf_prob default_cctx_type_cdf[AV2_CDF_SIZE(7)] = {
  AV2_CDF7(14350, 14836, 16054, 17075, 19408, 28530), AV2_PARA7(-2, -2, -2)
};

static const aom_cdf_prob
    default_pb_mv_most_probable_precision_cdf[NUM_MV_PREC_MPP_CONTEXT][AV2_CDF_SIZE(
        2)] = {
      { AV2_CDF2(20650), AV2_PARA2(-1, -1, -1) },
      { AV2_CDF2(15758), AV2_PARA2(-1, -1, -1) },
      { AV2_CDF2(4571), AV2_PARA2(-1, -1, -1) },
    };

static const aom_cdf_prob default_pb_mv_precision_cdf
    [MV_PREC_DOWN_CONTEXTS][NUM_PB_FLEX_QUALIFIED_MAX_PREC][AV2_CDF_SIZE(3)] = {
      {
          { AV2_CDF3(10923, 21845), AV2_PARA3(0, 0, 0) },
          { AV2_CDF3(31340, 32505), AV2_PARA3(1, 0, 1) },
          { AV2_CDF3(26039, 32175), AV2_PARA3(0, 0, 0) },
      },
      {
          { AV2_CDF3(10923, 21845), AV2_PARA3(0, 0, 0) },
          { AV2_CDF3(32197, 32676), AV2_PARA3(1, 0, 0) },
          { AV2_CDF3(28679, 32626), AV2_PARA3(0, 0, 0) },
      },
    };

static const aom_cdf_prob
    default_do_split_cdf[PARTITION_STRUCTURE_NUM][PARTITION_CONTEXTS]
                        [AV2_CDF_SIZE(2)] = {
                          {
                              { AV2_CDF2(28084), AV2_PARA2(0, 0, 1) },
                              { AV2_CDF2(23755), AV2_PARA2(1, 1, 1) },
                              { AV2_CDF2(23634), AV2_PARA2(1, 1, 1) },
                              { AV2_CDF2(19368), AV2_PARA2(0, 0, 1) },
                              { AV2_CDF2(24961), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(14941), AV2_PARA2(0, 0, -1) },
                              { AV2_CDF2(16154), AV2_PARA2(0, 0, -1) },
                              { AV2_CDF2(5905), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(21934), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(10440), AV2_PARA2(-1, 0, -1) },
                              { AV2_CDF2(11984), AV2_PARA2(-1, -1, -1) },
                              { AV2_CDF2(3474), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(20492), AV2_PARA2(0, 1, -1) },
                              { AV2_CDF2(6963), AV2_PARA2(0, -1, -1) },
                              { AV2_CDF2(8099), AV2_PARA2(-1, 0, -1) },
                              { AV2_CDF2(1529), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(24117), AV2_PARA2(1, 1, -2) },
                              { AV2_CDF2(7871), AV2_PARA2(0, -2, 0) },
                              { AV2_CDF2(23604), AV2_PARA2(0, 0, -2) },
                              { AV2_CDF2(8429), AV2_PARA2(-1, -1, 0) },
                              { AV2_CDF2(27356), AV2_PARA2(0, 0, -2) },
                              { AV2_CDF2(22441), AV2_PARA2(0, -1, -2) },
                              { AV2_CDF2(8897), AV2_PARA2(-1, -1, -1) },
                              { AV2_CDF2(6811), AV2_PARA2(-2, -2, -1) },
                              { AV2_CDF2(17592), AV2_PARA2(0, 1, -1) },
                              { AV2_CDF2(5648), AV2_PARA2(-1, -1, -2) },
                              { AV2_CDF2(5339), AV2_PARA2(-1, 0, -1) },
                              { AV2_CDF2(1082), AV2_PARA2(-1, 0, -1) },
                              { AV2_CDF2(26143), AV2_PARA2(1, 0, -2) },
                              { AV2_CDF2(11379), AV2_PARA2(1, -2, 0) },
                              { AV2_CDF2(20142), AV2_PARA2(1, 1, 1) },
                              { AV2_CDF2(7401), AV2_PARA2(0, -1, 1) },
                              { AV2_CDF2(26235), AV2_PARA2(1, -1, -2) },
                              { AV2_CDF2(23674), AV2_PARA2(1, 0, 1) },
                              { AV2_CDF2(12441), AV2_PARA2(1, 0, -2) },
                              { AV2_CDF2(10482), AV2_PARA2(1, 0, 0) },
                              { AV2_CDF2(20663), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(4192), AV2_PARA2(-1, 0, -2) },
                              { AV2_CDF2(5274), AV2_PARA2(-1, -1, 1) },
                              { AV2_CDF2(713), AV2_PARA2(0, 0, -1) },
                              { AV2_CDF2(28255), AV2_PARA2(1, 0, 0) },
                              { AV2_CDF2(27370), AV2_PARA2(1, 0, 0) },
                              { AV2_CDF2(23527), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(20990), AV2_PARA2(0, 0, -1) },
                              { AV2_CDF2(26727), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(21187), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(25324), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(17838), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(26136), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16591), AV2_PARA2(0, -1, -1) },
                              { AV2_CDF2(19838), AV2_PARA2(0, 0, -1) },
                              { AV2_CDF2(10605), AV2_PARA2(-1, -1, -1) },
                              { AV2_CDF2(22914), AV2_PARA2(0, 0, -1) },
                              { AV2_CDF2(12609), AV2_PARA2(-1, -1, -1) },
                              { AV2_CDF2(11341), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(4556), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(24218), AV2_PARA2(0, 0, -1) },
                              { AV2_CDF2(13059), AV2_PARA2(0, -1, -2) },
                              { AV2_CDF2(15378), AV2_PARA2(-1, -1, -2) },
                              { AV2_CDF2(5858), AV2_PARA2(-1, -1, -2) },
                              { AV2_CDF2(21644), AV2_PARA2(-1, -1, -2) },
                              { AV2_CDF2(7767), AV2_PARA2(-1, -1, -1) },
                              { AV2_CDF2(8309), AV2_PARA2(0, -1, -1) },
                              { AV2_CDF2(1687), AV2_PARA2(0, 0, 0) },
                          },
                          {
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(25554), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(18892), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(18530), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(10806), AV2_PARA2(0, -1, -1) },
                              { AV2_CDF2(22504), AV2_PARA2(0, 0, -1) },
                              { AV2_CDF2(12140), AV2_PARA2(-1, -1, -1) },
                              { AV2_CDF2(11966), AV2_PARA2(-1, -1, -1) },
                              { AV2_CDF2(4984), AV2_PARA2(-1, -1, 0) },
                              { AV2_CDF2(24460), AV2_PARA2(-1, -1, -1) },
                              { AV2_CDF2(8698), AV2_PARA2(-1, -1, -1) },
                              { AV2_CDF2(9655), AV2_PARA2(-1, -1, -1) },
                              { AV2_CDF2(2563), AV2_PARA2(-1, -1, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(26227), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(17669), AV2_PARA2(0, 0, -1) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(26179), AV2_PARA2(0, 0, -1) },
                              { AV2_CDF2(17889), AV2_PARA2(0, 0, -1) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                              { AV2_CDF2(23500), AV2_PARA2(0, 0, -1) },
                              { AV2_CDF2(13115), AV2_PARA2(-1, -1, -1) },
                              { AV2_CDF2(15253), AV2_PARA2(-1, -1, -1) },
                              { AV2_CDF2(6458), AV2_PARA2(-2, -1, 0) },
                              { AV2_CDF2(22566), AV2_PARA2(-1, -1, -1) },
                              { AV2_CDF2(11497), AV2_PARA2(-1, -1, -2) },
                              { AV2_CDF2(10045), AV2_PARA2(-1, -1, -1) },
                              { AV2_CDF2(3750), AV2_PARA2(-1, 0, -1) },
                          },
                        };

static const aom_cdf_prob
    default_do_square_split_cdf[PARTITION_STRUCTURE_NUM][SQUARE_SPLIT_CONTEXTS]
                               [AV2_CDF_SIZE(2)] = {
                                 {
                                     { AV2_CDF2(18000), AV2_PARA2(0, -1, -2) },
                                     { AV2_CDF2(10521), AV2_PARA2(-1, -2, -2) },
                                     { AV2_CDF2(11395), AV2_PARA2(-2, -2, -2) },
                                     { AV2_CDF2(4419), AV2_PARA2(-1, -1, -2) },
                                     { AV2_CDF2(12996), AV2_PARA2(1, -2, 0) },
                                     { AV2_CDF2(8185), AV2_PARA2(-2, -1, 0) },
                                     { AV2_CDF2(10979), AV2_PARA2(-1, -2, -1) },
                                     { AV2_CDF2(5010), AV2_PARA2(-1, -1, -2) },
                                 },
                                 {
                                     { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                                     { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                                     { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                                     { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                                     { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                                     { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                                     { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                                     { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                                 },
                               };

static const aom_cdf_prob
    default_rect_type_cdf[PARTITION_STRUCTURE_NUM][PARTITION_CONTEXTS]
                         [AV2_CDF_SIZE(2)] = {
                           {
                               { AV2_CDF2(14644), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(10173), AV2_PARA2(1, 0, 0) },
                               { AV2_CDF2(18529), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16071), AV2_PARA2(1, 1, 0) },
                               { AV2_CDF2(20263), AV2_PARA2(0, 0, -1) },
                               { AV2_CDF2(12813), AV2_PARA2(0, 0, -1) },
                               { AV2_CDF2(26612), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(23277), AV2_PARA2(0, 0, -1) },
                               { AV2_CDF2(10594), AV2_PARA2(1, 0, -1) },
                               { AV2_CDF2(7000), AV2_PARA2(1, 0, 0) },
                               { AV2_CDF2(20002), AV2_PARA2(0, 0, -1) },
                               { AV2_CDF2(12889), AV2_PARA2(0, 0, -2) },
                               { AV2_CDF2(13854), AV2_PARA2(1, 0, -1) },
                               { AV2_CDF2(10750), AV2_PARA2(0, 0, -1) },
                               { AV2_CDF2(18380), AV2_PARA2(0, 0, -1) },
                               { AV2_CDF2(17505), AV2_PARA2(0, -1, -1) },
                               { AV2_CDF2(14430), AV2_PARA2(0, -1, -2) },
                               { AV2_CDF2(11554), AV2_PARA2(0, 0, -2) },
                               { AV2_CDF2(20078), AV2_PARA2(0, 0, -1) },
                               { AV2_CDF2(19097), AV2_PARA2(1, 0, -1) },
                               { AV2_CDF2(15278), AV2_PARA2(0, 0, -2) },
                               { AV2_CDF2(10137), AV2_PARA2(0, 0, -1) },
                               { AV2_CDF2(21921), AV2_PARA2(0, -1, -2) },
                               { AV2_CDF2(14621), AV2_PARA2(0, -1, -1) },
                               { AV2_CDF2(19330), AV2_PARA2(0, 0, -2) },
                               { AV2_CDF2(15921), AV2_PARA2(0, 0, -1) },
                               { AV2_CDF2(26218), AV2_PARA2(0, 0, -1) },
                               { AV2_CDF2(24318), AV2_PARA2(0, 0, -1) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16066), AV2_PARA2(1, 0, 1) },
                               { AV2_CDF2(9225), AV2_PARA2(0, 0, -2) },
                               { AV2_CDF2(22849), AV2_PARA2(-1, -1, -1) },
                               { AV2_CDF2(14817), AV2_PARA2(0, -2, -1) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(18543), AV2_PARA2(1, 0, 0) },
                               { AV2_CDF2(13210), AV2_PARA2(0, -2, 0) },
                               { AV2_CDF2(24367), AV2_PARA2(-1, -1, -2) },
                               { AV2_CDF2(18417), AV2_PARA2(-1, 0, 0) },
                               { AV2_CDF2(24701), AV2_PARA2(0, -1, -1) },
                               { AV2_CDF2(18911), AV2_PARA2(0, -1, -2) },
                               { AV2_CDF2(29590), AV2_PARA2(0, 0, -1) },
                               { AV2_CDF2(27778), AV2_PARA2(0, -1, -2) },
                               { AV2_CDF2(3400), AV2_PARA2(0, 0, -1) },
                               { AV2_CDF2(935), AV2_PARA2(1, 1, 0) },
                               { AV2_CDF2(10365), AV2_PARA2(-1, -1, -2) },
                               { AV2_CDF2(1723), AV2_PARA2(0, 0, -1) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                           },
                           {
                               { AV2_CDF2(15654), AV2_PARA2(1, 0, -1) },
                               { AV2_CDF2(13500), AV2_PARA2(1, 0, 0) },
                               { AV2_CDF2(19177), AV2_PARA2(1, 1, -1) },
                               { AV2_CDF2(14739), AV2_PARA2(1, 1, 0) },
                               { AV2_CDF2(18769), AV2_PARA2(1, 0, 0) },
                               { AV2_CDF2(13500), AV2_PARA2(0, 0, -1) },
                               { AV2_CDF2(23583), AV2_PARA2(1, 0, 0) },
                               { AV2_CDF2(20927), AV2_PARA2(1, 0, 0) },
                               { AV2_CDF2(15045), AV2_PARA2(0, 0, -1) },
                               { AV2_CDF2(10528), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(22474), AV2_PARA2(0, -1, -1) },
                               { AV2_CDF2(14250), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16561), AV2_PARA2(1, 0, 0) },
                               { AV2_CDF2(11427), AV2_PARA2(1, 0, -1) },
                               { AV2_CDF2(21874), AV2_PARA2(0, -1, -1) },
                               { AV2_CDF2(16344), AV2_PARA2(1, 1, 0) },
                               { AV2_CDF2(21566), AV2_PARA2(-1, -1, -1) },
                               { AV2_CDF2(13357), AV2_PARA2(0, 0, -2) },
                               { AV2_CDF2(27355), AV2_PARA2(0, 0, -1) },
                               { AV2_CDF2(24117), AV2_PARA2(0, -1, -1) },
                               { AV2_CDF2(10901), AV2_PARA2(1, 0, -2) },
                               { AV2_CDF2(5780), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(19056), AV2_PARA2(-1, -2, -2) },
                               { AV2_CDF2(9141), AV2_PARA2(0, 0, -1) },
                               { AV2_CDF2(20436), AV2_PARA2(0, -1, -2) },
                               { AV2_CDF2(15693), AV2_PARA2(-1, -1, -2) },
                               { AV2_CDF2(26536), AV2_PARA2(0, -1, -1) },
                               { AV2_CDF2(23667), AV2_PARA2(-1, -1, -1) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(19241), AV2_PARA2(0, -1, -2) },
                               { AV2_CDF2(13038), AV2_PARA2(-1, -1, -2) },
                               { AV2_CDF2(28903), AV2_PARA2(-1, -1, -2) },
                               { AV2_CDF2(24802), AV2_PARA2(-1, -1, -2) },
                               { AV2_CDF2(9097), AV2_PARA2(-1, -1, -2) },
                               { AV2_CDF2(2749), AV2_PARA2(0, -1, -1) },
                               { AV2_CDF2(15201), AV2_PARA2(-1, 0, -2) },
                               { AV2_CDF2(4449), AV2_PARA2(0, -1, -1) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                               { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
                           },
                         };

static const aom_cdf_prob default_do_ext_partition_cdf
    [PARTITION_STRUCTURE_NUM][NUM_RECT_CONTEXTS][PARTITION_CONTEXTS]
    [AV2_CDF_SIZE(2)] = {
      {
          {
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(31091), AV2_PARA2(0, 0, 1) },
              { AV2_CDF2(29638), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(28924), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(28653), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(30349), AV2_PARA2(1, 1, 1) },
              { AV2_CDF2(28265), AV2_PARA2(1, 0, 0) },
              { AV2_CDF2(27287), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(27721), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(29960), AV2_PARA2(1, 1, 1) },
              { AV2_CDF2(28345), AV2_PARA2(1, 1, 0) },
              { AV2_CDF2(27302), AV2_PARA2(1, 1, 0) },
              { AV2_CDF2(27252), AV2_PARA2(1, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(30342), AV2_PARA2(0, 0, -1) },
              { AV2_CDF2(27563), AV2_PARA2(0, -1, -1) },
              { AV2_CDF2(26168), AV2_PARA2(0, -1, -1) },
              { AV2_CDF2(23089), AV2_PARA2(-1, -1, -2) },
              { AV2_CDF2(30643), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(28683), AV2_PARA2(0, 0, -1) },
              { AV2_CDF2(28009), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(26186), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(29222), AV2_PARA2(0, 0, -1) },
              { AV2_CDF2(25740), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(24079), AV2_PARA2(0, -1, -1) },
              { AV2_CDF2(19806), AV2_PARA2(1, 0, -1) },
              { AV2_CDF2(29409), AV2_PARA2(1, 1, 0) },
              { AV2_CDF2(26825), AV2_PARA2(1, 1, 1) },
              { AV2_CDF2(25919), AV2_PARA2(1, 1, 1) },
              { AV2_CDF2(24417), AV2_PARA2(1, 1, 1) },
          },
      },
      {
          {
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(26866), AV2_PARA2(-1, -1, 0) },
              { AV2_CDF2(24499), AV2_PARA2(-1, 0, 0) },
              { AV2_CDF2(24732), AV2_PARA2(-1, -1, -1) },
              { AV2_CDF2(23387), AV2_PARA2(-1, 0, 0) },
              { AV2_CDF2(27477), AV2_PARA2(-1, -1, 0) },
              { AV2_CDF2(25837), AV2_PARA2(-1, -1, -1) },
              { AV2_CDF2(24621), AV2_PARA2(-1, -1, -1) },
              { AV2_CDF2(23604), AV2_PARA2(0, -1, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(24384), AV2_PARA2(-1, 0, -1) },
              { AV2_CDF2(22113), AV2_PARA2(-1, -1, -2) },
              { AV2_CDF2(21798), AV2_PARA2(0, 0, -2) },
              { AV2_CDF2(20067), AV2_PARA2(0, -1, -1) },
              { AV2_CDF2(26220), AV2_PARA2(-1, -1, -1) },
              { AV2_CDF2(22997), AV2_PARA2(-1, -1, 0) },
              { AV2_CDF2(22249), AV2_PARA2(-1, -1, -1) },
              { AV2_CDF2(20091), AV2_PARA2(0, 0, 0) },
          },
      },
    };

static const aom_cdf_prob default_do_uneven_4way_partition_cdf
    [PARTITION_STRUCTURE_NUM][NUM_RECT_CONTEXTS][PARTITION_CONTEXTS]
    [AV2_CDF_SIZE(2)] = {
      {
          {
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(25789), AV2_PARA2(-1, -1, -1) },
              { AV2_CDF2(25290), AV2_PARA2(-1, -1, -2) },
              { AV2_CDF2(24270), AV2_PARA2(-1, -1, -1) },
              { AV2_CDF2(22994), AV2_PARA2(-1, -1, -1) },
              { AV2_CDF2(25801), AV2_PARA2(0, 0, -1) },
              { AV2_CDF2(25260), AV2_PARA2(0, -1, -2) },
              { AV2_CDF2(24041), AV2_PARA2(0, 0, -1) },
              { AV2_CDF2(24281), AV2_PARA2(0, 0, -1) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(23038), AV2_PARA2(0, -1, -1) },
              { AV2_CDF2(19972), AV2_PARA2(-1, 1, 0) },
              { AV2_CDF2(19723), AV2_PARA2(0, -1, 1) },
              { AV2_CDF2(17939), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(15574), AV2_PARA2(0, -1, -1) },
              { AV2_CDF2(13761), AV2_PARA2(1, 0, -1) },
              { AV2_CDF2(12917), AV2_PARA2(1, 1, -1) },
              { AV2_CDF2(11328), AV2_PARA2(1, 0, 0) },
              { AV2_CDF2(17295), AV2_PARA2(-1, -1, -2) },
              { AV2_CDF2(14463), AV2_PARA2(-1, -2, 1) },
              { AV2_CDF2(14724), AV2_PARA2(0, -1, -1) },
              { AV2_CDF2(11653), AV2_PARA2(0, 0, -1) },
              { AV2_CDF2(13202), AV2_PARA2(-1, -1, -2) },
              { AV2_CDF2(10929), AV2_PARA2(0, -1, -2) },
              { AV2_CDF2(10348), AV2_PARA2(0, -1, -2) },
              { AV2_CDF2(8276), AV2_PARA2(0, 0, -1) },
          },
      },
      {
          {
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(24728), AV2_PARA2(-1, -1, -1) },
              { AV2_CDF2(22673), AV2_PARA2(-1, -1, -1) },
              { AV2_CDF2(21033), AV2_PARA2(0, -1, -1) },
              { AV2_CDF2(20321), AV2_PARA2(0, 0, -1) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(24631), AV2_PARA2(-2, 1, -2) },
              { AV2_CDF2(21363), AV2_PARA2(0, 1, 0) },
              { AV2_CDF2(20201), AV2_PARA2(0, 1, 0) },
              { AV2_CDF2(17529), AV2_PARA2(1, -1, -2) },
              { AV2_CDF2(21042), AV2_PARA2(0, 0, 1) },
              { AV2_CDF2(18640), AV2_PARA2(1, -1, 0) },
              { AV2_CDF2(18183), AV2_PARA2(1, 1, -1) },
              { AV2_CDF2(15590), AV2_PARA2(1, 0, -1) },
          },
      },
    };

static const aom_cdf_prob default_txfm_4way_partition_type_reduced_cdf
    [FSC_MODES][TX_PARTITION_DIRS][TX_PARTITION_TYPE_NUM_VERT_AND_HORZ]
    [AV2_CDF_SIZE(7)] = {
      {
          {
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
          },
          {
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
          },
      },
      {
          {
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
          },
          {
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
          },
      },
    };

static const aom_cdf_prob default_txfm_2or3_way_partition_type_cdf
    [FSC_MODES][TX_PARTITION_DIRS][TX_PARTITION_TYPE_NUM_VERT_OR_HORZ - 1]
    [AV2_CDF_SIZE(2)] = {
      {
          {
              { AV2_CDF2(25131), AV2_PARA2(-1, -1, 0) },
              { AV2_CDF2(24514), AV2_PARA2(-1, -1, 0) },
          },
          {
              { AV2_CDF2(19534), AV2_PARA2(0, -1, -1) },
              { AV2_CDF2(18637), AV2_PARA2(0, 0, 0) },
          },
      },
      {
          {
              { AV2_CDF2(30226), AV2_PARA2(-2, 0, 0) },
              { AV2_CDF2(30703), AV2_PARA2(-1, -1, 1) },
          },
          {
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          },
      },
    };

static const aom_cdf_prob default_most_probable_stx_set_cdf[AV2_CDF_SIZE(7)] = {
  AV2_CDF7(20712, 26263, 30623, 32732, 32736, 32740), AV2_PARA7(-2, -1, 0)
};

static const aom_cdf_prob default_most_probable_stx_set_cdf_ADST_ADST[AV2_CDF_SIZE(
    4)] = { AV2_CDF4(15897, 26144, 30010), AV2_PARA4(0, -1, 0) };

static const aom_cdf_prob default_txfm_do_partition_cdf
    [FSC_MODES][TX_PARTITION_DIRS][TXFM_SPLIT_GROUP][AV2_CDF_SIZE(2)] = {
      {
          {
              { AV2_CDF2(26330), AV2_PARA2(-1, -1, 0) },
              { AV2_CDF2(29620), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(20420), AV2_PARA2(-1, -1, 0) },
              { AV2_CDF2(21694), AV2_PARA2(-1, -1, -1) },
              { AV2_CDF2(13317), AV2_PARA2(-1, -1, -1) },
              { AV2_CDF2(15391), AV2_PARA2(-1, -1, -1) },
              { AV2_CDF2(15952), AV2_PARA2(-1, -1, -1) },
              { AV2_CDF2(14736), AV2_PARA2(-1, -1, -1) },
              { AV2_CDF2(13810), AV2_PARA2(0, -1, 0) },
          },
          {
              { AV2_CDF2(31646), AV2_PARA2(-2, -1, 0) },
              { AV2_CDF2(32393), AV2_PARA2(-1, 0, 0) },
              { AV2_CDF2(30802), AV2_PARA2(-1, 0, -1) },
              { AV2_CDF2(30485), AV2_PARA2(-1, -1, -1) },
              { AV2_CDF2(20759), AV2_PARA2(-1, -1, -2) },
              { AV2_CDF2(22159), AV2_PARA2(0, 0, -1) },
              { AV2_CDF2(26832), AV2_PARA2(-1, -1, -1) },
              { AV2_CDF2(27351), AV2_PARA2(0, 0, -1) },
              { AV2_CDF2(24696), AV2_PARA2(0, 0, -1) },
          },
      },
      {
          {
              { AV2_CDF2(29308), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(32550), AV2_PARA2(1, 1, 1) },
              { AV2_CDF2(27963), AV2_PARA2(0, 0, -1) },
              { AV2_CDF2(27618), AV2_PARA2(-1, -1, -1) },
              { AV2_CDF2(22367), AV2_PARA2(0, -1, -2) },
              { AV2_CDF2(23478), AV2_PARA2(-1, -2, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(27990), AV2_PARA2(0, 0, 0) },
          },
          {
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
              { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          },
      },
    };

static const aom_cdf_prob default_txfm_4way_partition_type_cdf
    [FSC_MODES][TX_PARTITION_DIRS][TX_PARTITION_TYPE_NUM_VERT_AND_HORZ]
    [AV2_CDF_SIZE(7)] = {
      {
          {
              { AV2_CDF7(32744, 32748, 32752, 32756, 32760, 32764),
                AV2_PARA7(-2, -2, 0) },
              { AV2_CDF7(3006, 22888, 27132, 29972, 29976, 32724),
                AV2_PARA7(-1, -1, -1) },
              { AV2_CDF7(3673, 8849, 27652, 27656, 29944, 29948),
                AV2_PARA7(-1, -1, -1) },
              { AV2_CDF7(5219, 19419, 28052, 28836, 29528, 31296),
                AV2_PARA7(-1, -1, -1) },
              { AV2_CDF7(3055, 19649, 22157, 27038, 27466, 31646),
                AV2_PARA7(-1, -1, -1) },
              { AV2_CDF7(6044, 11255, 26170, 26493, 28585, 29584),
                AV2_PARA7(-1, -1, -1) },
              { AV2_CDF7(5896, 20361, 25685, 27552, 28695, 31097),
                AV2_PARA7(-1, -1, -1) },
              { AV2_CDF7(2355, 17601, 21703, 26050, 27881, 31397),
                AV2_PARA7(-1, -1, -2) },
              { AV2_CDF7(4701, 13502, 24958, 26413, 28166, 30129),
                AV2_PARA7(-1, -1, -2) },
              { AV2_CDF7(3319, 16449, 21641, 27154, 29260, 31512),
                AV2_PARA7(-1, -1, -2) },
              { AV2_CDF7(2526, 17088, 19643, 29378, 29382, 32724),
                AV2_PARA7(-1, -1, -1) },
              { AV2_CDF7(2298, 4406, 23886, 23890, 30148, 30152),
                AV2_PARA7(-1, -1, -1) },
              { AV2_CDF7(1553, 16160, 18679, 27983, 29592, 32139),
                AV2_PARA7(-1, -1, -2) },
              { AV2_CDF7(2316, 4714, 22731, 23797, 29514, 30077),
                AV2_PARA7(-1, -2, -2) },
          },
          {
              { AV2_CDF7(10790, 22602, 32736, 32740, 32744, 32748),
                AV2_PARA7(-1, -1, -2) },
              { AV2_CDF7(14384, 19116, 26545, 28687, 28691, 32724),
                AV2_PARA7(-1, -1, -1) },
              { AV2_CDF7(13910, 21690, 26343, 26347, 28432, 28436),
                AV2_PARA7(-1, -1, -1) },
              { AV2_CDF7(15891, 21712, 28890, 29571, 30307, 31363),
                AV2_PARA7(-1, -1, -2) },
              { AV2_CDF7(15946, 19447, 20270, 23423, 23858, 31148),
                AV2_PARA7(-1, -1, -2) },
              { AV2_CDF7(16053, 16957, 20312, 20924, 24770, 25959),
                AV2_PARA7(-1, -1, -2) },
              { AV2_CDF7(18479, 23053, 26582, 26966, 27567, 29836),
                AV2_PARA7(-2, -2, -2) },
              { AV2_CDF7(9312, 11882, 14354, 17792, 19827, 29477),
                AV2_PARA7(-2, -2, -2) },
              { AV2_CDF7(8490, 10668, 13295, 15353, 19952, 22575),
                AV2_PARA7(-2, -2, -2) },
              { AV2_CDF7(6404, 12066, 16173, 20041, 24512, 28421),
                AV2_PARA7(-2, -2, -2) },
              { AV2_CDF7(10019, 14455, 17658, 27012, 27016, 32724),
                AV2_PARA7(-1, -2, -2) },
              { AV2_CDF7(9479, 14904, 19374, 19378, 28027, 28031),
                AV2_PARA7(-1, -2, -2) },
              { AV2_CDF7(3717, 7198, 8103, 20546, 23558, 31447),
                AV2_PARA7(-2, -2, -2) },
              { AV2_CDF7(4058, 5429, 8987, 13978, 25126, 26655),
                AV2_PARA7(-2, -2, -2) },
          },
      },
      {
          {
              { AV2_CDF7(32434, 32490, 32545, 32601, 32657, 32712),
                AV2_PARA7(-2, 0, 0) },
              { AV2_CDF7(1491, 14241, 29930, 32517, 32524, 32724),
                AV2_PARA7(-2, -1, -2) },
              { AV2_CDF7(1719, 16525, 31000, 31004, 32575, 32579),
                AV2_PARA7(-1, -1, 0) },
              { AV2_CDF7(1645, 16749, 29324, 30425, 32016, 32485),
                AV2_PARA7(-2, -2, -2) },
              { AV2_CDF7(2908, 15802, 24689, 28470, 32122, 32542),
                AV2_PARA7(-1, 0, 0) },
              { AV2_CDF7(3470, 17931, 25841, 29589, 31907, 32465),
                AV2_PARA7(-1, 1, 1) },
              { AV2_CDF7(5638, 19594, 28693, 29977, 30703, 32154),
                AV2_PARA7(-2, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(2734, 12129, 30342, 31805, 31844, 32724),
                AV2_PARA7(-2, 0, 0) },
              { AV2_CDF7(3849, 21783, 31043, 31056, 32181, 32193),
                AV2_PARA7(-2, 1, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
          },
          {
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
              { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087),
                AV2_PARA7(0, 0, 0) },
          },
      },
    };

static const aom_cdf_prob
    default_filter_dir_cdf[MHCCP_CONTEXT_GROUP_SIZE][AV2_CDF_SIZE(3)] = {
      { AV2_CDF3(10923, 21845), AV2_PARA3(0, 0, 0) },
      { AV2_CDF3(8795, 15105), AV2_PARA3(0, -1, -1) },
      { AV2_CDF3(10433, 15974), AV2_PARA3(-1, -1, -2) },
      { AV2_CDF3(17085, 21689), AV2_PARA3(-1, -1, -2) },
    };

static const aom_cdf_prob default_cfl_cdf[CFL_CONTEXTS][AV2_CDF_SIZE(2)] = {
  { AV2_CDF2(20441), AV2_PARA2(-1, -1, 0) },
  { AV2_CDF2(11610), AV2_PARA2(0, -1, -1) },
  { AV2_CDF2(4643), AV2_PARA2(0, 0, 0) },
};

static const aom_cdf_prob
    default_region_type_cdf[INTER_SDP_BSIZE_GROUP][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(2635), AV2_PARA2(-1, 0, 0) },
      { AV2_CDF2(883), AV2_PARA2(-1, 0, 0) },
      { AV2_CDF2(503), AV2_PARA2(-2, 0, 0) },
      { AV2_CDF2(279), AV2_PARA2(-2, -1, 0) },
    };

static const aom_cdf_prob
    default_morph_pred_cdf[MORPH_PRED_CTXS][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(31715), AV2_PARA2(-2, -1, 0) },
      { AV2_CDF2(19667), AV2_PARA2(0, 0, -1) },
      { AV2_CDF2(10555), AV2_PARA2(1, 1, -1) },
    };

static const aom_cdf_prob default_seg_tree_cdf[AV2_CDF_SIZE(8)] = {
  AV2_CDF8(4096, 8192, 12288, 16384, 20480, 24576, 28672), AV2_PARA8(0, 0, 0)
};

static const aom_cdf_prob default_seg_tree_cdf1[AV2_CDF_SIZE(8)] = {
  AV2_CDF8(4096, 8192, 12288, 16384, 20480, 24576, 28672), AV2_PARA8(0, 0, 0)
};

static const aom_cdf_prob
    default_segment_pred_cdf[SEG_TEMPORAL_PRED_CTXS][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
    };

static const aom_cdf_prob
    default_spatial_pred_seg_tree_cdf[SPATIAL_PREDICTION_PROBS][AV2_CDF_SIZE(8)] = {
      { AV2_CDF8(5622, 7893, 16093, 18233, 27809, 28373, 32533),
        AV2_PARA8(0, 0, 0) },
      { AV2_CDF8(14274, 18230, 22557, 24935, 29980, 30851, 32344),
        AV2_PARA8(0, 0, 0) },
      { AV2_CDF8(27527, 28487, 28723, 28890, 32397, 32647, 32679),
        AV2_PARA8(0, 0, 0) },
    };

static const aom_cdf_prob
    default_spatial_pred_seg_tree_cdf1[SPATIAL_PREDICTION_PROBS][AV2_CDF_SIZE(
        8)] = {
      { AV2_CDF8(4096, 8192, 12288, 16384, 20480, 24576, 28672),
        AV2_PARA8(0, 0, 0) },
      { AV2_CDF8(4096, 8192, 12288, 16384, 20480, 24576, 28672),
        AV2_PARA8(0, 0, 0) },
      { AV2_CDF8(4096, 8192, 12288, 16384, 20480, 24576, 28672),
        AV2_PARA8(0, 0, 0) },
    };

static const aom_cdf_prob
    default_seg_id_ext_flag_cdf[SPATIAL_PREDICTION_PROBS][AV2_CDF_SIZE(2)] = {
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
    };

static const aom_cdf_prob default_intra_dip_cdf[DIP_CTXS][AV2_CDF_SIZE(2)] = {
  { AV2_CDF2(11914), AV2_PARA2(-1, -1, -1) },
  { AV2_CDF2(5128), AV2_PARA2(-1, -1, -1) },
  { AV2_CDF2(3555), AV2_PARA2(-1, -1, -1) },
};

static const aom_cdf_prob default_intra_dip_mode_n6_cdf[AV2_CDF_SIZE(6)] = {
  AV2_CDF6(5753, 15408, 19070, 22631, 27578), AV2_PARA6(0, 0, 0)
};

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  // AV2_AV2_COMMON_ENTROPY_INITS_MODES_H_
