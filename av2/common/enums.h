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

#ifndef AOM_AV2_COMMON_ENUMS_H_
#define AOM_AV2_COMMON_ENUMS_H_

#include "av2/common/common.h"

// =============================================================================
// AV2 Unified Enums & Constants
// =============================================================================
#include "aom/aom_codec.h"
#include "aom/aom_integer.h"
#include "aom_dsp/txfm_common.h"
#include "aom_ports/mem.h"
#include "av2/common/entdec.h"
#include "av2/common/entenc.h"


// 4X4, 8X8, 16X16, 32X32, 64X64, 128X128
#define SQR_BLOCK_SIZES 6

// Safety Fallbacks for standard AV1 compatibility during AV2 builds
#ifndef LAST_FRAME
#define LAST_FRAME 1
#define LAST2_FRAME 2
#define LAST3_FRAME 3
#define GOLDEN_FRAME 4
#define BWDREF_FRAME 5
#define ALTREF2_FRAME 6
#define ALTREF_FRAME 7
#endif

#ifndef FWD_REFS
#define FWD_REFS 4
#define BWD_REFS 3
#define SINGLE_REFS (FWD_REFS + BWD_REFS)
#endif

#ifndef DUAL_FILTER_TYPE
typedef enum {
  REG_REG,
  REG_SMOOTH,
  REG_SHARP,
  SMOOTH_REG,
  SMOOTH_SMOOTH,
  SMOOTH_SHARP,
  SHARP_REG,
  SHARP_SMOOTH,
  SHARP_SHARP,
} DUAL_FILTER_TYPE;
#endif

#ifndef FILTER_INTRA_MODE
typedef enum {
  FILTER_DC_PRED,
  FILTER_V_PRED,
  FILTER_H_PRED,
  FILTER_D157_PRED,
  FILTER_PAETH_PRED,
  FILTER_INTRA_MODES,
} FILTER_INTRA_MODE;
#endif

#ifndef OBMC_CAUSAL
#define OBMC_CAUSAL 1
#define WARPED_CAUSAL 2
#endif

#ifndef TXB_SKIP_CONTEXTS
#define TXB_SKIP_CONTEXTS 3
#endif
#define TX_SIZE_CONTEXTS 3
#define INTER_COMPOUND_MODES 8

// Additional compatibility fallbacks for standard AV1 MV contexts
#ifndef NEARESTMV
#define NEARESTMV 100
#define NEAREST_NEARESTMV 101
#define NEAREST_NEWMV 102
#define NEW_NEARESTMV 103
#endif

#define NEWMV_MODE_CONTEXTS 6
#define GLOBALMV_MODE_CONTEXTS 2
#define REFMV_MODE_CONTEXTS 6
#define COMP_REF_TYPE_CONTEXTS 5
#define UNI_COMP_REF_CONTEXTS 3
#define UNIDIR_COMP_REFS 9
#define TXFM_PARTITION_CONTEXTS ((TX_SIZES - TX_8X8) * 6 - 3)
#define COMP_INDEX_CONTEXTS 6
#ifndef PARTITION_CONTEXTS
#define PARTITION_CONTEXTS (5 * 4)
#endif

#define TX_SIZE_CTX_MIN 1
#define MAX_TX_CATS (TX_SIZES - TX_SIZE_CTX_MIN)
#define MAX_TX_DEPTH 2
#define MAX_VARTX_DEPTH 2

#define FRAME_LF_COUNT 4
#define DELTA_LF_SMALL 3
#define DELTA_LF_PROBS 3
#define CFL_JOINT_SIGNS 8
#define CFL_ALPHA_CONTEXTS 6
#define CFL_SIGN_U(js) (((js + 1) * 11) >> 5)
#define CFL_SIGN_V(js) ((js + 1) - CFL_SIGNS * CFL_SIGN_U(js))
#define CFL_CONTEXT_U(js) (js + 1 - CFL_SIGNS)
#define CFL_CONTEXT_V(js) \
  (CFL_SIGN_V(js) * CFL_SIGNS + CFL_SIGN_U(js) - CFL_SIGNS)

// Reference frame compatibility flags and enums for standard AV1
#define AOM_LAST_FLAG (1 << 0)
#define AOM_LAST2_FLAG (1 << 1)
#define AOM_LAST3_FLAG (1 << 2)
#define AOM_GOLD_FLAG (1 << 3)
#define AOM_BWD_FLAG (1 << 4)
#define AOM_ALT2_FLAG (1 << 5)
#define AOM_ALT_FLAG (1 << 6)
#define AOM_REFFRAME_ALL ((1 << 7) - 1)
typedef int AOM_REFFRAME;

typedef enum {
  UNIDIR_COMP_REFERENCE,
  BIDIR_COMP_REFERENCE,
} COMP_REFERENCE_TYPE;

#define GLOBALMV_CTX_MASK ((1 << (REFMV_OFFSET - GLOBALMV_OFFSET)) - 1)
#define COMP_REFERENCE_TYPES 3
#define COMPOUND_DISTWTD 3

#define SEQ_LEVEL_KEEP_STATS 32
#define SEQ_LEVEL_2_2 22
#define SEQ_LEVEL_2_3 23
#define SEQ_LEVEL_3_2 32
#define SEQ_LEVEL_3_3 33
#define SEQ_LEVEL_4_2 42
#define SEQ_LEVEL_4_3 43

#define PROFILE_0 0
#define PROFILE_1 1
#define PROFILE_2 2

#define MAX_TILES 512
#define MAX_EXTERNAL_REFERENCES 128
#define FRAME_BUFFERS_ALLINTRA 2
#define SELECT_ALL_BUF_SLOTS 0xFF
#define PRED_MODE_INVALID MB_MODE_COUNT

#define MAX_TILE_ROWS 64
#define MAX_TILE_COLS 64

#define WEDGE_HORIZONTAL 0
#define WEDGE_VERTICAL 1
#define WEDGE_OBLIQUE27 2
#define WEDGE_OBLIQUE63 3
#define WEDGE_OBLIQUE117 4
#define WEDGE_OBLIQUE153 5
#define WEDGE_DIRECTIONS 6

#define GLOBALMV_OFFSET 3

#define DEFAULT_DELTA_Q_RES_DUCKY_ENCODE 4
#define DEFAULT_DELTA_LF_RES 2
#define DEFAULT_DELTA_LF_MULTI 0

#define PARTITION_HORZ_A PARTITION_HORZ_4A
#define PARTITION_HORZ_B PARTITION_HORZ_4B
#define PARTITION_VERT_A PARTITION_VERT_4A
#define PARTITION_VERT_B PARTITION_VERT_4B
#define PARTITION_HORZ_4 PARTITION_HORZ_3
#define PARTITION_VERT_4 PARTITION_VERT_3

// Additional AV1 reference, restoration, and superres compatibility flags
#define REFMV_OFFSET 4
#define REFMV_CTX_MASK ((1 << 3) - 1)
#define NEWMV_CTX_MASK ((1 << 3) - 1)
#define TOTAL_UNIDIR_COMP_REFS 4
#define FWD_RF_OFFSET(rf) (rf - LAST_FRAME)
#define BWD_RF_OFFSET(rf) (rf - BWDREF_FRAME)
#define TOTAL_COMP_REFS 12
#define COMP_NEWMV_CTXS 5
#define MAX_MODE_LF_DELTAS 2
#define EXTREF_FRAME 6
#define RESTORE_WIENER RESTORE_PC_WIENER
#define RESTORE_SGRPROJ RESTORE_WIENER_NONSEP
#define SUPERRES_SCALE_DENOMINATOR_MIN 9
#define SUPERRES_SCALE_BITS 3
#define REF_FRAMES_LOG2 3

typedef enum {
  AV2_OPFL_REFINE_NONE,
  AV2_OPFL_REFINE_SWITCHABLE,
  AV2_OPFL_REFINE_ALL,
  AV2_OPFL_REFINE_AUTO,
} av2_opfl_refine_type;

#ifdef __cplusplus
extern "C" {
#endif

/*! @file */

/*!\cond */

#undef MAX_SB_SIZE

#define MAX_PR_NUM 16
#define MAX_DR_STACK_SIZE 4
#define MAX_DR_PR_NUM 2

#define MAX_SEQ_NUM 16

// Macros related to joint shell signaling
#define FIRST_SHELL_CLASS 8
#define SECOND_SHELL_CLASS 8
// Macros related to wedge angle signaling
#define WEDGE_QUADS 4
#define QUAD_WEDGE_ANGLES 5
// Macros related to luma and chroma intra mode signaling
#define LUMA_INTRA_MODE_INDEX_COUNT 8
#define LUMA_INTRA_MODE_OFFSET_COUNT 6
#define CHROMA_INTRA_MODE_INDEX_COUNT 8
// Macro related to EOB position token signaling
#define EOB_PT_INDEX_COUNT 8
// Macros related Inter tx type signaling
#define INTER_TX_TYPE_INDEX_COUNT 8
#define INTER_TX_TYPE_OFFSET1_COUNT 8
#define INTER_TX_TYPE_OFFSET2_COUNT 4
#define INTER_TX_TYPE_SIGNALING_SETS 2
#define TX_EXT_32_CTXS 2

#define FGM_ID_BITS 3
#define MAX_FGM_NUM (1 << FGM_ID_BITS)

#define NUM_CTX_IS_JOINT 2
#define NUM_OPTIONS_IS_JOINT 2
#define NUM_CTX_NON_JOINT_TYPE INTER_MODE_CONTEXTS
#define NUM_OPTIONS_NON_JOINT_TYPE 5
#define NUM_CTX_JOINT_TYPE 1
#define NUM_OPTIONS_JOINT_TYPE 2

// maximum warp bound boxes.
#define MAX_WARP_BD_SIZE (1 << (8 - 3))
#define MAX_WARP_BD_SQ (MAX_WARP_BD_SIZE * MAX_WARP_BD_SIZE)

#define SUBBLK_REF_EXT_LINES 2

#define DMVR_SEARCH_EXT_LINES 2

#define WARP_STATS_BUFFER_SIZE \
  (MAX_WARP_REF_CANDIDATES * NUM_WARP_PRECISION_MODES)

#define AVG_CDF_WEIGHT_PRIMARY 7
#define AVG_CDF_WEIGHT_NON_PRIMARY 1

#define USE_TUNED_ADST4 1
#define USE_TUNED_ADST8 1
#define USE_TUNED_ADST16 1

#define NUM_REF_SAM_CFL 8

#define DEFAULT_IMP_MSK_WT 0  // default implict masked blending weight

#define BAWP_MAX_REF_NUMB 16

/*AV2_WEDGE_0 is defined in the three o'clock direciton, the angles are defined in
 * the anticlockwise.*/
enum {
  AV2_WEDGE_0,
  AV2_WEDGE_14,
  AV2_WEDGE_27,
  AV2_WEDGE_45,
  AV2_WEDGE_63,
  AV2_WEDGE_90,
  AV2_WEDGE_117,
  AV2_WEDGE_135,
  AV2_WEDGE_153,
  AV2_WEDGE_166,
  AV2_WEDGE_180,
  AV2_WEDGE_194,
  AV2_WEDGE_207,
  AV2_WEDGE_225,
  AV2_WEDGE_243,
  AV2_WEDGE_270,
  AV2_WEDGE_297,
  AV2_WEDGE_315,
  AV2_WEDGE_333,
  AV2_WEDGE_346,
  AV2_WEDGE_ANGLES
} UENUM1BYTE(AV2WedgeDirectionType);

#define H_WEDGE_ANGLES 10
#define NUM_WEDGE_DIST 4
#ifndef MAX_WEDGE_TYPES
#define MAX_WEDGE_TYPES 16
#endif
#define WEDGE_BLD_LUT_SIZE 128

#define WARP_CU_BANK 1
#define REFINEMV_SUBBLOCK_WIDTH 16
#define REFINEMV_SUBBLOCK_HEIGHT 16

#define IBP_WEIGHT_SIZE_LOG2 4
#define IBP_WEIGHT_SIZE (1 << IBP_WEIGHT_SIZE_LOG2)

// Supported scale modes for JOINT_NEWMV
#define JOINT_NEWMV_SCALE_FACTOR_CNT 5
// Supoorted scale modes for JOINT_AMVDNEWMV
#define JOINT_AMVD_SCALE_FACTOR_CNT 3

// Max superblock size
#define MAX_SB_SIZE_LOG2 8
#define MAX_SB_SIZE (1 << MAX_SB_SIZE_LOG2)
#define MAX_SB_SQUARE (MAX_SB_SIZE * MAX_SB_SIZE)
#define BLOCK_128_MI_SIZE_LOG2 5

// Cross-Component Sample Offset (CCSO)
#define CCSO_BLK_SIZE MAX_SB_SIZE_LOG2
#define CCSO_PADDING_SIZE 5
#define CCSO_BAND_NUM 64
#define CCSO_NUM_COMPONENTS 3

#define CFL_MHCCP_SWITCH_NUM 2
#define MHCCP_MODE_NUM 3
#define MHCCP_CONTEXT_GROUP_SIZE 4
// 2 lines of the luma buffer, and 1 line for the chroma buffer
#define LINE_NUM 1
#define MHCCP_NUM_PARAMS 3
#define MHCCP_WINDOW_SIZE 6
#define MHCCP_MAX_REF_SAMPLES \
  (2 * MHCCP_WINDOW_SIZE * (2 * MAX_SB_SIZE + MHCCP_WINDOW_SIZE))
#define MHCCP_DECIM_BITS 16
#define MHCCP_DECIM_ROUND (1 << (MHCCP_DECIM_BITS - 1))

// Min superblock size
#define MIN_SB_SIZE_LOG2 6

// Pixels per Mode Info (MI) unit
#define MI_SIZE_LOG2 2
#define MI_SIZE (1 << MI_SIZE_LOG2)
#define MI_SIZE_64X64 (64 >> MI_SIZE_LOG2)
#define MI_SIZE_128X128 (128 >> MI_SIZE_LOG2)
#define MI_SIZE_256X256 (256 >> MI_SIZE_LOG2)

// 1/8 pels per Mode Info (MI) unit
#define MI_SUBPEL_SIZE_LOG2 (MI_SIZE_LOG2 + 3)

// MI-units per max superblock (MI Block - MIB)
#define MAX_MIB_SIZE_LOG2 (MAX_SB_SIZE_LOG2 - MI_SIZE_LOG2)
#define MAX_MIB_SIZE (1 << MAX_MIB_SIZE_LOG2)

#define MAX_MIB_SQUARE (MAX_MIB_SIZE * MAX_MIB_SIZE)

#if CONFIG_INSPECTION
// MI-units per min superblock
#define MIN_MIB_SIZE_LOG2 (MIN_SB_SIZE_LOG2 - MI_SIZE_LOG2)
#endif  // CONFIG_INSPECTION

// Mask to extract MI offset within max MIB
#define MAX_MIB_MASK (MAX_MIB_SIZE - 1)

// The largest block size where we need to construct chroma blocks separately
// from luma blocks is 64x32. With the four way partition, we can get 64x4
// block sizes. So we only need to track results for 16 mi units.
#define MAX_MI_LUMA_SIZE_FOR_SUB_8 (64 >> MI_SIZE_LOG2)
#define SUB_8_BITMASK_T uint16_t
#define SUB_8_BITMASK_SIZE (16)
#define SUB_8_BITMASK_ON (UINT16_MAX)

// Maximum number of tile rows and tile columns
#define MAX_PALETTE_SQUARE (64 * 64)
// Maximum number of colors in a palette.
#define PALETTE_MAX_SIZE 8
// Minimum number of colors in a palette.
#define PALETTE_MIN_SIZE 2

#define FRAME_OFFSET_BITS 5
#define MAX_FRAME_DISTANCE ((1 << FRAME_OFFSET_BITS) - 1)

// Multiple reference line selection for intra prediction
#define MRL_LINE_NUMBER 4
#define FIRST_MODE_COUNT 13
#define SECOND_MODE_COUNT 16
#define Y_MODE_CONTEXTS 3
#define INTRA_MODE_SETS 4
#define NON_DIRECTIONAL_MODES_COUNT 5
#define UV_MODE_CONTEXTS 2
#define CFL_CONTEXTS 3

// Intra Secondary Transform
#define STX_TYPES 4
#define STX_PRED_CTXS 2
#define IST_SET_SIZE 7
#define IST_REDUCED_SET_SIZE 4
#define IST_4x4_SET_SIZE (IST_SET_SIZE + IST_SET_SIZE)
#define IST_8x8_SET_SIZE (IST_SET_SIZE + IST_REDUCED_SET_SIZE)

#define IST_4x4_WIDTH 16
#define IST_4x4_HEIGHT 8
#define IST_8x8_HEIGHT_RED 20
#define IST_8x8_WIDTH_MAX 64
#define IST_8x8_WIDTH 48
#define IST_8x8_HEIGHT_MAX 32
#define IST_8x8_HEIGHT 32
#define IST_ADST_NZ_CNT 20

#define STX_SYNTAX_DEBUG 0
#define STX_COEFF_DEBUG 0

#define FSC_MODES 2
#define FSC_MAXWIDTH 32
#define FSC_MAXHEIGHT 32
#define FSC_MINWIDTH 4
#define FSC_MINHEIGHT 4

#define DIST_PRECISION_BITS 4

#define IBC_TOP_INTERP_BORDER 0
#define IBC_LEFT_INTERP_BORDER 0
#define IBC_RIGHT_INTERP_BORDER 1
#define IBC_BOTTOM_INTERP_BORDER 1

#define DMVR_SEARCH_NUM_NEIGHBORS 24
#define MULTI_SEQ_CONFIG_BITS 6
#define PROFILE_BITS 5
#define CONFIG_BITS 6
#define INTEROP_BITS 4

// Refer to Table A.1 in annexA.c
enum {
  MAIN_420_10_IP0 = 0,
  MAIN_420_10_IP1 = 1,
  MAIN_420_10_IP2 = 2,
  MAIN_422_10_IP1 = 3,
  MAIN_444_10_IP1 = 4,
#if CONFIG_TESTONLY_12BIT_SUPPORT
  // Fake profile value only for testing 12-bit. Not defined in AV2 spec.
  TEST_ONLY_12BIT_PROFILE,
#endif  // CONFIG_TESTONLY_12BIT_SUPPORT
  RESERVED_PROFILES_START,
  CONFIGURABLE = 31,
  MAX_PROFILES,
} SENUM1BYTE(BITSTREAM_PROFILE);

#define AV2_MAX_NUM_STREAMS 32

enum {
  AV2_BITDEPTH_0 = 0,        /**< 10 bits */
  AV2_BITDEPTH_1 = 1,        /**< 8 bits */
  AV2_BITDEPTH_2 = 2,        /**< 12 bits */
  AV2_NUM_SUPPORTED_BITDEPTH /**<number of supported bitdepth>*/
};
#define FIXED_QP_OFFSET_COUNT 6

typedef enum {
  AV2_COLOR_DESC_IDC_EXPLICIT = 0,   // Explicitly signaled
  AV2_COLOR_DESC_IDC_BT709SDR = 1,   // CP=1, TC=1, MC=1
  AV2_COLOR_DESC_IDC_BT2100PQ = 2,   // CP=9, TC=16, MC=9
  AV2_COLOR_DESC_IDC_BT2100HLG = 3,  // CP=9, TC=18, MC=9
  AV2_COLOR_DESC_IDC_SRGB = 4,       // CP=1, TC=13, MC=0
  AV2_COLOR_DESC_IDC_SYCC = 5,       // CP=1, TC=13, MC=5
} av2_color_desc_idc_t;

#define BUFFER_POOL_MAX_SIZE 18  // Max VBI slots (16) + 2 extra

#define MAX_NUM_TLAYERS 4
#define MAX_NUM_MLAYERS 8
#define MAX_NUM_XLAYERS 32
#define RESERVED_NUM_MLAYERS -1
// bits for temporal, embedded, and extended layers
#define TLAYER_BITS 2  // 2 bits for MAX_NUM_TLAYERS
#define LOG2_TLAYERS TLAYER_BITS
#define MLAYER_BITS 3  // 3 bits for MAX_NUM_MLAYERS
#define XLAYER_BITS 5  // 5 bits for MAX_NUM_XLAYERS
#define MAX_NUM_OPERATING_POINTS (MAX_NUM_TLAYERS * MAX_NUM_MLAYERS)
#define OP_POINTS_CNT_MINUS_1_BITS (TLAYER_BITS + MLAYER_BITS)
#define OP_POINTS_IDC_BITS (MAX_NUM_TLAYERS + MAX_NUM_MLAYERS)

// LCR
#define LCR_ID_BITS 3
#define MAX_NUM_LCR (1 << LCR_ID_BITS)
#define MAX_LCR_TYPES 2
#define GLOBAL_XLAYER_ID 31
// OPS
#define OPS_ID_BITS 4
#define MAX_NUM_OPS_ID (1 << OPS_ID_BITS)
#define OPS_COUNT_BITS 3
#define MAX_OPS_COUNT ((1 << OPS_COUNT_BITS) - 1)
// Atlas
#define MAX_ATLAS_REGIONS 64  // maximum number of Atlas regions per dimension
#define ATLAS_SEG_ID_BITS 3
#define MAX_NUM_ATLAS_SEG_ID (1 << ATLAS_SEG_ID_BITS)
#define ATLAS_LABEL_SEG_ID_BITS 8
#define MAX_NUM_ATLAS_SEGMENTS (1 << ATLAS_LABEL_SEG_ID_BITS)

#define MULTILAYER_HLS_REMOVE_LOGS 1

#define MAX_NUM_SEQ_LCR_ID 7
#define LCR_ID_UNSPECIFIED 0

enum {
  TEXTURE_LAYER = 0,
  AUX_LAYER = 1,
  STEREO_LAYER = 2,
  DEPENDENT_LAYER = 3,
  LAYER_TYPES = 16
};

enum {
  LCR_ALPHA_AUX = 0,
  LCR_DEPTH_AUX = 1,
  LCR_SEGMENTATION_AUX = 2,
  LCR_GAIN_MAP_AUX = 3,
  LCR_AUX_TYPE_RESERVED_START = 4,
  LCR_AUX_TYPE_RESERVED_END = 127,
  LCR_AUX_TYPE_UNSPECIFIED_START = 128,
  LCR_AUX_TYPE_UNSPECIFIED_END = 159,
  LCR_AUX_TYPE_RESERVED2_START = 160,
  LCR_AUX_TYPE_RESERVED2_END = 255,
};

enum {
  VIEW_UNSPECIFIED = 0,
  VIEW_CENTER = 1,
  VIEW_LEFT = 2,
  VIEW_RIGHT = 3,
  VIEW_EXPLICIT = 4,
  VIEW_TYPES = 8
};

enum {
  ENHANCED_ATLAS = 0,
  BASIC_ATLAS = 1,
  SINGLE_ATLAS = 2,
  MULTISTREAM_ATLAS = 3,
  MULTISTREAM_ALPHA_ATLAS = 4,
  ATLAS_TYPES
};

enum {
  UNSPECIFIED_MULTIVIEW,
  STEREOSCOPIC_VIEWPORTS,
  IMMERSIVE_MULTIPLE_VIEWPORTS,
  IMMERSIVE_MULTIPLE_VIEWPORTS_ALPHA,
  IMMERSIVE_MULTIPLE_VIEWPORTS_DEPTH,
  IMMERSIVE_MULTIPLE_VIEWPORTS_ALPHA_DEPTH,
  MULTIVIEW_PLAYBACK,
  SUBREGION_PLAYBACK,
  GLOBAL_PURPOSE_ID_TYPES
};

typedef enum ATTRIBUTE_PACKED {
  BLOCK_4X4,
  BLOCK_4X8,
  BLOCK_8X4,
  BLOCK_8X8,
  BLOCK_8X16,
  BLOCK_16X8,
  BLOCK_16X16,
  BLOCK_16X32,
  BLOCK_32X16,
  BLOCK_32X32,
  BLOCK_32X64,
  BLOCK_64X32,
  BLOCK_64X64,
  BLOCK_64X128,
  BLOCK_128X64,
  BLOCK_128X128,
  BLOCK_128X256,
  BLOCK_256X128,
  BLOCK_256X256,
  BLOCK_4X16,
  BLOCK_16X4,
  BLOCK_8X32,
  BLOCK_32X8,
  BLOCK_16X64,
  BLOCK_64X16,
  BLOCK_4X32,
  BLOCK_32X4,
  BLOCK_8X64,
  BLOCK_64X8,
  BLOCK_SIZES_ALL,
  BLOCK_LARGEST = BLOCK_256X256,
  BLOCK_SIZES = BLOCK_4X32,
  BLOCK_INVALID = 255,
} BLOCK_SIZE;

static INLINE BLOCK_SIZE get_larger_sqr_bsize(BLOCK_SIZE bsize) {
  switch (bsize) {
    case BLOCK_4X4:
    case BLOCK_4X8:
    case BLOCK_8X4: return BLOCK_8X8;

    case BLOCK_8X8:
    case BLOCK_8X16:
    case BLOCK_16X8:
    case BLOCK_4X16:
    case BLOCK_16X4: return BLOCK_16X16;

    case BLOCK_16X16:
    case BLOCK_16X32:
    case BLOCK_32X16:
    case BLOCK_8X32:
    case BLOCK_32X8: return BLOCK_32X32;

    case BLOCK_32X32:
    case BLOCK_32X64:
    case BLOCK_64X32:
    case BLOCK_16X64:
    case BLOCK_64X16: return BLOCK_64X64;

    case BLOCK_64X64:
    case BLOCK_64X128:
    case BLOCK_128X64:
    case BLOCK_128X128: return BLOCK_128X128;
    default: return BLOCK_INVALID;
  }
}

enum {
  SHARED_PART = 0,
  LUMA_PART = 1,
  CHROMA_PART = 2,
  TREES_TYPES,
} UENUM1BYTE(TREE_TYPE);

enum {
  INTRA_REGION = 0,
  MIXED_INTER_INTRA_REGION = 1,
  REGION_TYPES = 2,
} UENUM1BYTE(REGION_TYPE);

enum {
  PARTITION_NONE,
  PARTITION_HORZ,
  PARTITION_VERT,
  PARTITION_HORZ_3,  // 3 horizontal sub-partitions with ratios 4:1, 2:1 and 4:1
  PARTITION_VERT_3,  // 3 vertical sub-partitions with ratios 4:1, 2:1 and 4:1
  PARTITION_HORZ_4A,  // 4 horizontal uneven sub-partitions (1:2:4:1).
  PARTITION_HORZ_4B,  // 4 horizontal uneven sub-partitions (1:4:2:1).
  PARTITION_VERT_4A,  // 4 vertical uneven sub-partitions (1:2:4:1).
  PARTITION_VERT_4B,  // 4 vertical uneven sub-partitions (1:4:2:1).
  PARTITION_SPLIT,
  EXT_PARTITION_TYPES = PARTITION_SPLIT,
  ALL_PARTITION_TYPES = EXT_PARTITION_TYPES + 1,
  PARTITION_TYPES = PARTITION_VERT + 1,
  PARTITION_INVALID = 255
} UENUM1BYTE(PARTITION_TYPE);

enum {
  AV2_HORZ = 0,
  AV2_VERT,
  AV2_NUM_RECT_PARTS,
  AV2_RECT_INVALID = AV2_NUM_RECT_PARTS
} UENUM1BYTE(AV2_RECT_PART_TYPE);

enum {
  UNEVEN_4A = 0,
  UNEVEN_4B,
  NUM_UNEVEN_4WAY_PARTS,
} UENUM1BYTE(UNEVEN_4WAY_PART_TYPE);

typedef char PARTITION_CONTEXT;
#define PARTITION_PLOFFSET 4  // number of probability models per block size

#define PARTITION_BLOCK_SIZES BLOCK_SIZES
#define SQUARE_SPLIT_CONTEXTS (2 * PARTITION_PLOFFSET)

#define INTER_SDP_BSIZE_GROUP 4
#define INTER_SDP_MAX_BLOCK_SIZE 64

// 1D tx types (Redeclared internally in txfm_common.h or shared)
enum {
  TX_PARTITION_NONE,
  TX_PARTITION_SPLIT,
  TX_PARTITION_HORZ,
  TX_PARTITION_VERT,
  TX_PARTITION_HORZ4,
  TX_PARTITION_VERT4,
  TX_PARTITION_HORZ5,
  TX_PARTITION_VERT5,
  TX_PARTITION_TYPES,
  TX_PARTITION_INVALID = 255
} UENUM1BYTE(TX_PARTITION_TYPE);

#define TX_PARTITION_TYPE_NUM (TX_PARTITION_TYPES - 1)
#define TX_PARTITION_TYPE_NUM_VERT_AND_HORZ 14
#define TX_PARTITION_TYPE_NUM_VERT_OR_HORZ 3
#define TX_PARTITION_DIRS 2  // Horizontal and vertical.

#define MAX_TX_SIZE_LOG2 (6)
#define MAX_TX_SIZE (1 << MAX_TX_SIZE_LOG2)
#define MIN_TX_SIZE_LOG2 2
#define MIN_TX_SIZE (1 << MIN_TX_SIZE_LOG2)
#define MAX_TX_SQUARE (MAX_TX_SIZE * MAX_TX_SIZE)
#define MAX_TRELLIS 1024

#define TX_PAD_HOR_LOG2 2
#define TX_PAD_HOR 4
#define TX_PAD_LEFT 4
#define TX_PAD_TOP 4
#define TX_PAD_BOTTOM 4
#define TX_PAD_VER (TX_PAD_TOP + TX_PAD_BOTTOM)
// Pad 16 extra bytes to avoid reading overflow in SIMD optimization.
#define TX_PAD_END 16
#define TX_PAD_2D ((32 + TX_PAD_HOR) * (32 + TX_PAD_VER) + TX_PAD_END)

// Number of maxium size transform blocks in the maximum size superblock
#define MAX_TX_BLOCKS_IN_MAX_SB_LOG2 ((MAX_SB_SIZE_LOG2 - MAX_TX_SIZE_LOG2) * 2)
#define MAX_TX_BLOCKS_IN_MAX_SB (1 << MAX_TX_BLOCKS_IN_MAX_SB_LOG2)

// frame transform mode
enum {
  ONLY_4X4,         // use only 4x4 transform
  TX_MODE_LARGEST,  // transform size is the largest possible for pu size
  TX_MODE_SELECT,   // transform specified for each block
  TX_MODES,
} UENUM1BYTE(TX_MODE);

// 1D tx types
enum {
  DCT_1D,
  ADST_1D,
  FLIPADST_1D,
  IDTX_1D,
  DDT_1D,
  TX_TYPES_1D,
} UENUM1BYTE(TX_TYPE_1D);

enum {
  DCT2,
  IDT,
  DST7,
  DCT8,
  DDTX,
  FDDT,
} UENUM2BYTE(TX1D_TYPE);

enum {
  CCTX_NONE,     // No cross chroma transform
  CCTX_45,       // 45 degrees rotation (Haar transform)
  CCTX_30,       // 30 degrees rotation
  CCTX_60,       // 60 degrees rotation
  CCTX_MINUS45,  // -45 degrees rotation
  CCTX_MINUS30,  // -30 degrees rotation
  CCTX_MINUS60,  // -60 degrees rotation
  CCTX_TYPES,
  CCTX_START = CCTX_NONE + 1,
} UENUM1BYTE(CctxType);

enum { FWD_TXFM, INV_TXFM, TXFM_DIRECTIONS } UENUM1BYTE(TXFM_DIRECTION);

#define EOB_TX_CTXS 3
#define EXT_TX_SIZES 4  // number of sizes that use extended transforms

#define EXT_TX_SETS_INTER 5  // Sets of transform selections for INTER
#define INTER_TX_SET1 16
#define INTER_TX_SET2 12
#define INTER_TX_SET3 2
#define INTER_TX_SET4 4

#define EXT_TX_SETS_INTRA 3  // Sets of transform selections for INTRA
#define INTRA_TX_SET1 7
#define INTRA_TX_SET2 2

enum { PLANE_TYPE_Y, PLANE_TYPE_UV, PLANE_TYPES } UENUM1BYTE(PLANE_TYPE);

#define CFL_ALPHABET_SIZE_LOG2 3
#define CFL_ALPHABET_SIZE (1 << CFL_ALPHABET_SIZE_LOG2)
#define CFL_MAGS_SIZE ((2 << CFL_ALPHABET_SIZE_LOG2) + 1)
#define CFL_INDEX_ZERO CFL_ALPHABET_SIZE
#define CFL_IDX_U(idx) (idx >> CFL_ALPHABET_SIZE_LOG2)
#define CFL_IDX_V(idx) (idx & (CFL_ALPHABET_SIZE - 1))

enum { CFL_PRED_U, CFL_PRED_V, CFL_PRED_PLANES } UENUM1BYTE(CFL_PRED_TYPE);

enum {
  CFL_SIGN_ZERO,
  CFL_SIGN_NEG,
  CFL_SIGN_POS,
  CFL_SIGNS
} UENUM1BYTE(CFL_SIGN_TYPE);

enum {
  CFL_DISALLOWED,
  CFL_ALLOWED,
  CFL_ALLOWED_TYPES
} UENUM1BYTE(CFL_ALLOWED_TYPE);

enum {
  MHCCP_DISALLOWED,
  MHCCP_ALLOWED,
  MHCCP_ALLOWED_TYPES
} UENUM1BYTE(MHCCP_ALLOWED_TYPE);

enum {
  CFL_DISALLOWED_FOR_CHROMA,
  CFL_ALLOWED_FOR_CHROMA,
  CFL_ALLOWED_TYPES_FOR_SDP
} UENUM1BYTE(CFL_ALLOWED_FOR_SDP_TYPE);

enum {
  PALETTE_MAP,
  COLOR_MAP_TYPES,
} UENUM1BYTE(COLOR_MAP_TYPE);

enum {
  TWO_COLORS,
  THREE_COLORS,
  FOUR_COLORS,
  FIVE_COLORS,
  SIX_COLORS,
  SEVEN_COLORS,
  EIGHT_COLORS,
  PALETTE_SIZES
} UENUM1BYTE(PALETTE_SIZE);

enum {
  PALETTE_COLOR_ONE,
  PALETTE_COLOR_TWO,
  PALETTE_COLOR_THREE,
  PALETTE_COLOR_FOUR,
  PALETTE_COLOR_FIVE,
  PALETTE_COLOR_SIX,
  PALETTE_COLOR_SEVEN,
  PALETTE_COLOR_EIGHT,
  PALETTE_COLORS
} UENUM1BYTE(PALETTE_COLOR);

// Note: All directional predictors must be between V_PRED and D67_PRED (both
// inclusive).
enum {
  DC_PRED,        // Average of above and left pixels
  V_PRED,         // Vertical
  H_PRED,         // Horizontal
  D45_PRED,       // Directional 45  degree
  D135_PRED,      // Directional 135 degree
  D113_PRED,      // Directional 113 degree
  D157_PRED,      // Directional 157 degree
  D203_PRED,      // Directional 203 degree
  D67_PRED,       // Directional 67  degree
  SMOOTH_PRED,    // Combination of horizontal and vertical interpolation
  SMOOTH_V_PRED,  // Vertical interpolation
  SMOOTH_H_PRED,  // Horizontal interpolation
  PAETH_PRED,     // Predict from the direction of smallest gradient
  NEARMV,
  GLOBALMV,
  NEWMV,
  WARPMV,      // WARPMV mode (original WARPMV)
  WARP_NEWMV,  // WARP_NEWMV mode (original warp modes under NEWMV)
               // Compound ref compound modes
  NEAR_NEARMV,
  NEAR_NEWMV,
  NEW_NEARMV,
  GLOBAL_GLOBALMV,
  NEW_NEWMV,
  JOINT_NEWMV,
  NEAR_NEARMV_OPTFLOW,
  NEAR_NEWMV_OPTFLOW,
  NEW_NEARMV_OPTFLOW,
  NEW_NEWMV_OPTFLOW,
  JOINT_NEWMV_OPTFLOW,
  MB_MODE_COUNT,
  INTRA_MODE_START = DC_PRED,
  INTRA_MODE_END = NEARMV,
  DIR_MODE_START = V_PRED,
  DIR_MODE_END = D67_PRED + 1,
  INTRA_MODE_NUM = INTRA_MODE_END - INTRA_MODE_START,
  SINGLE_INTER_MODE_START = NEARMV,
  SINGLE_INTER_MODE_END = NEAR_NEARMV,
  SINGLE_INTER_MODE_NUM = SINGLE_INTER_MODE_END - SINGLE_INTER_MODE_START,
  COMP_INTER_MODE_START = NEAR_NEARMV,
  COMP_INTER_MODE_END = MB_MODE_COUNT,
  COMP_INTER_MODE_NUM = COMP_INTER_MODE_END - COMP_INTER_MODE_START,
  COMP_OPTFLOW_MODE_START = NEAR_NEARMV_OPTFLOW,
  INTER_COMPOUND_REF_TYPES = COMP_OPTFLOW_MODE_START - COMP_INTER_MODE_START,
  INTER_MODE_START = NEARMV,
  INTER_MODE_END = MB_MODE_COUNT,
  INTRA_MODES = PAETH_PRED + 1,   // PAETH_PRED has to be the last intra mode.
  INTRA_INVALID = MB_MODE_COUNT,  // For uv_mode in inter blocks
  MODE_INVALID = 255
} UENUM1BYTE(PREDICTION_MODE);

// TODO(ltrudeau) Do we really want to pack this?
// TODO(ltrudeau) Do we match with PREDICTION_MODE?
enum {
  UV_DC_PRED,        // Average of above and left pixels
  UV_V_PRED,         // Vertical
  UV_H_PRED,         // Horizontal
  UV_D45_PRED,       // Directional 45  degree
  UV_D135_PRED,      // Directional 135 degree
  UV_D113_PRED,      // Directional 113 degree
  UV_D157_PRED,      // Directional 157 degree
  UV_D203_PRED,      // Directional 203 degree
  UV_D67_PRED,       // Directional 67  degree
  UV_SMOOTH_PRED,    // Combination of horizontal and vertical interpolation
  UV_SMOOTH_V_PRED,  // Vertical interpolation
  UV_SMOOTH_H_PRED,  // Horizontal interpolation
  UV_PAETH_PRED,     // Predict from the direction of smallest gradient
  UV_CFL_PRED,       // Chroma-from-Luma
  UV_INTRA_MODES,
  UV_MODE_INVALID,  // For uv_mode in inter blocks
} UENUM1BYTE(UV_PREDICTION_MODE);

enum {
  CFL_EXPLICIT,       // av2 cfl
  CFL_DERIVED_ALPHA,  // implicit CfL mode with derived scaling factor
  CFL_MULTI_PARAM,    // multi hypothesis cross component prediction
  CFL_TYPE_COUNT,     // CfL mode type count
} UENUM1BYTE(CFL_TYPE);

// Number of top model rd to store for pruning y modes in intra mode decision
#define TOP_INTRA_MODEL_COUNT 6
#define TOP_TX_PART_COUNT 4
#define TOP_INTER_TX_PART_COUNT 8
// Total number of luma intra prediction modes (include both directional and
// non-directional modes)
#define LUMA_MODE_COUNT 61

enum {
  SIMPLE_TRANSLATION,
  INTERINTRA,
  WARP_CAUSAL,  // Warp estimation from spatial MVs
  WARP_DELTA,   // Directly-signaled warp model
  WARP_EXTEND,  // Extension of an existing warp model into another block
  MOTION_MODES
} UENUM1BYTE(MOTION_MODE);

#define OPFL_GRAD_UNIT_LOG2 4
#define OPFL_GRAD_UNIT (1 << OPFL_GRAD_UNIT_LOG2)

#define COMPOUND_WARP_LINE_BUFFER_REDUCTION 1

enum {
  II_DC_PRED,
  II_V_PRED,
  II_H_PRED,
  II_SMOOTH_PRED,
  INTERINTRA_MODES
} UENUM1BYTE(INTERINTRA_MODE);

enum {
  COMPOUND_AVERAGE,
  COMPOUND_WEDGE,
  COMPOUND_DIFFWTD,
  COMPOUND_TYPES,
  MASKED_COMPOUND_TYPES = 2,
} UENUM1BYTE(COMPOUND_TYPE);

enum {
  SEQ_LEVEL_2_0 = 0,
  SEQ_LEVEL_2_1 = 1,
  SEQ_LEVEL_3_0 = 2,
  SEQ_LEVEL_3_1 = 3,
  SEQ_LEVEL_4_0 = 4,
  SEQ_LEVEL_4_1 = 5,
  SEQ_LEVEL_5_0 = 6,
  SEQ_LEVEL_5_1 = 7,
  SEQ_LEVEL_5_2 = 8,
  SEQ_LEVEL_5_3 = 9,
  SEQ_LEVEL_6_0 = 10,
  SEQ_LEVEL_6_1 = 11,
  SEQ_LEVEL_6_2 = 12,
  SEQ_LEVEL_6_3 = 13,
  SEQ_LEVEL_7_0 = 14,
  SEQ_LEVEL_7_1 = 15,
  SEQ_LEVEL_7_2 = 16,
  SEQ_LEVEL_7_3 = 17,
  SEQ_LEVEL_8_0 = 18,
  SEQ_LEVEL_8_1 = 19,
  SEQ_LEVEL_8_2 = 20,
  SEQ_LEVEL_8_3 = 21,
  SEQ_LEVELS,
  SEQ_LEVEL_MAX = 31
} UENUM1BYTE(AV2_LEVEL);

typedef AV2_LEVEL AV1_LEVEL;

#define LEVEL_BITS 5

#define DIRECTIONAL_MODES 8
#define MAX_ANGLE_DELTA 3
#define ANGLE_STEP 3

// Total delta angles for one nominal directional mode
#define TOTAL_ANGLE_DELTA_COUNT 7

// The warpmv and warpmv_new mode is signalled as a separate flag
// So the number of remaining modes to be signalled is (SINGLE_INTER_MODE_NUM-2)
#define INTER_SINGLE_MODES (SINGLE_INTER_MODE_NUM - 2)

#define SKIP_CONTEXTS 6
#define SKIP_MODE_CONTEXTS 3

#define INTRABC_CONTEXTS 3

#define NUM_ALLOWED_BV_PRECISIONS 2
#define NUM_BV_PRECISION_CONTEXTS 1
#define COMP_GROUP_IDX_CONTEXTS 12

#define MIN_MAX_DRL_BITS 1
#define MAX_MAX_DRL_BITS (MAX_REF_MV_STACK_SIZE - 1)

#define WARPMV_MODE_CONTEXT 5
#define MAX_REF_BV_STACK_SIZE 4
#define MIN_MAX_IBC_DRL_BITS 1
#define MAX_MAX_IBC_DRL_BITS (MAX_REF_BV_STACK_SIZE - 1)

#define OPFL_MODE_CONTEXTS 2

#define INTER_MODE_CONTEXTS 5
#define DRL_MODE_CONTEXTS INTER_MODE_CONTEXTS

#define INTER_COMPOUND_SAME_REFS_TYPES (INTER_COMPOUND_REF_TYPES - 2)

// Explicit BAWP scaling factor counts
#define EXPLICIT_BAWP_SCALE_CNT 2
// Explicit BAWP scaling factor context counts
#define BAWP_SCALES_CTX_COUNT 3
// The allowed value range for bawp_flag
#define BAWP_REF_LINES 1
#define BAWP_OPTION_CNT 4

#define AMVD_MODE_CONTEXTS 3
#define NUM_AMVD_MODES 9

// Number of supported factors for compound weighted prediction
#define MAX_CWP_NUM 5
// maximum value for the supported factors
#define CWP_MAX 20
// minimum value for the supported factors
#define CWP_MIN -4
// Weighting factor for simple average prediction
#define CWP_EQUAL 8
#define CWP_WEIGHT_BITS 4
#define MAX_CWP_CONTEXTS 2

// Number of supported factors for TIP weights
#define MAX_TIP_WTD_NUM 8
#define TIP_EQUAL_WTD 8
#define TIP_SINGLE_WTD 16
#define MAX_AMVD_INDEX 8

#define DELTA_Q_SMALL 7
#define DELTA_Q_SMALL_MINUS_2 (DELTA_Q_SMALL - 2)
#define DELTA_Q_PROBS (DELTA_Q_SMALL)
#define DEFAULT_DELTA_Q_RES_PERCEPTUAL 4
#define DEFAULT_DELTA_Q_RES_OBJECTIVE 4

#define MAX_MV_REF_CANDIDATES 2
#define MAX_REF_MV_STACK_SIZE 6
#define USABLE_REF_MV_STACK_SIZE (MAX_REF_MV_STACK_SIZE)

#define REF_CAT_LEVEL 0

#define MAX_WARP_REF_CANDIDATES 4
#define WARP_REF_CONTEXTS 1

#define INTRA_INTER_CONTEXTS 4
#define COMP_INTER_CONTEXTS 5
#define REF_CONTEXTS 3

// Group size from mapping block size to tx partition context
#define TXFM_SPLIT_GROUP 9
typedef uint16_t TXFM_CONTEXT;

#define TIP_CONTEXTS 3

#define MAX_MFH_NUM 16
#define WARP_CAUSAL_MODE_CTX 4

#define TIP_PRED_MODES 2
#define WARP_EXTEND_CTX 3

#define INTER_REFS_PER_FRAME 7

#define MAX_NUM_LONG_TERM_FRAMES INTER_REFS_PER_FRAME

// log 2 of max 8 references per-frame (7 inter + 1 intra)
// log2(INTER_REFS_PER_FRAME + 1)
#define MAX_REFS_PER_FRAME_LOG2 3

#define REF_FRAMES 16

// NOTE: A limited number of unidirectional reference pairs can be signalled for
//       compound prediction. The use of skip mode, on the other hand, makes it
//       possible to have a reference pair not listed for explicit signaling.
#define MODE_CTX_REF_FRAMES                                \
  (INTER_REFS_PER_FRAME * (INTER_REFS_PER_FRAME + 3) / 2 + \
   2)  // additional combinations for the same reference of compound mode

// With k=INTER_REFS_PER_FRAMES, indices 0 to k-1 represent rank 1 to rank k
// references. The next k(k-1)/2 indices are left for compound reference types
// (there are k choose 2 compound combinations). Then, index for intra frame is
// defined as k+k(k-1)/2.
#define INTRA_FRAME                                    \
  (INTER_REFS_PER_FRAME * (INTER_REFS_PER_FRAME + 3) / \
   2)  // additional combinations for the same reference of compound mode
// Used for indexing into arrays that contain reference data for
// inter and intra.
#define INTRA_FRAME_INDEX INTER_REFS_PER_FRAME
#define NONE_FRAME INVALID_IDX
#define AV2_REFFRAME_ALL ((1 << INTER_REFS_PER_FRAME) - 1)

// REF_FRAMES for the cm->ref_frame_map array, 1 scratch frame for the new
// frame in cm->cur_frame, INTER_REFS_PER_FRAME for scaled references on the
// encoder in the cpi->scaled_ref_buf array.
// Each substream has its own sub-DPB.
#define FRAME_BUFFERS \
  (REF_FRAMES + 1 + INTER_REFS_PER_FRAME) * AV2_MAX_NUM_STREAMS

#define TIP_FRAME (MODE_CTX_REF_FRAMES - 1)
#define TIP_FRAME_INDEX (INTER_REFS_PER_FRAME + 1)
#define SINGLE_REF_FRAMES (INTER_REFS_PER_FRAME + 2)
#define MAX_COMPOUND_REF_INDEX (SINGLE_REF_FRAMES - 1)

// Note: It includes single and compound references. So, it can take values from
// NONE_FRAME to (MODE_CTX_REF_FRAMES - 1). Hence, it is not defined as an enum.
typedef int8_t MV_REFERENCE_FRAME;

#define MAX_LR_FLEX_SWITCHABLE_BITS 3

/*!\endcond */

/*!\enum RestorationType
 * \brief This enumeration defines various restoration types supported
 */
typedef enum {
  RESTORE_NONE,          /**< No restoration */
  RESTORE_PC_WIENER,     /**< Pixel-classified Wiener restoration */
  RESTORE_WIENER_NONSEP, /**< Nonseparable Wiener restoration */
  RESTORE_SWITCHABLE,    /**< Switchable restoration */
  RESTORE_SWITCHABLE_TYPES = RESTORE_SWITCHABLE, /**< Num Switchable types */
  RESTORE_TYPES = RESTORE_SWITCHABLE + 1,        /**< Num Restore types */
} RestorationType;

/*!\cond */
// Picture prediction structures (0-12 are predefined) in scalability metadata.
enum {
  SCALABILITY_L1T2 = 0,
  SCALABILITY_L1T3 = 1,
  SCALABILITY_L2T1 = 2,
  SCALABILITY_L2T2 = 3,
  SCALABILITY_L2T3 = 4,
  SCALABILITY_S2T1 = 5,
  SCALABILITY_S2T2 = 6,
  SCALABILITY_S2T3 = 7,
  SCALABILITY_L2T1h = 8,
  SCALABILITY_L2T2h = 9,
  SCALABILITY_L2T3h = 10,
  SCALABILITY_S2T1h = 11,
  SCALABILITY_S2T2h = 12,
  SCALABILITY_S2T3h = 13,
  SCALABILITY_SS = 14
} UENUM1BYTE(SCALABILITY_STRUCTURES);

#define DIV_LUT_PREC_BITS 9
#define DIV_LUT_BITS 7

#define DIR_MODES_0_90 17
#define IBP_WEIGHT_SHIFT DIV_LUT_BITS

#define IBP_WEIGHT_MAX (1 << IBP_WEIGHT_SHIFT)
#define IBP_WEIGHT_REF IBP_WEIGHT_MAX
typedef uint16_t IbpWeightsType;

/*!\enum Warp projection type
 * \brief This enumeration defines various warp projection type supported
 */
typedef enum {
  PROJ_GLOBAL_MOTION,  /**< block is from global motion */
  PROJ_SPATIAL,        /**< Project from spatial neighborhood */
  PROJ_PARAM_BANK,     /**< Project from circular buffer */
  PROJ_DEFAULT,        /**< Default values */
  WARP_PROJ_TYPES = 4, /**< Num projection types */
} WarpProjectionType;

/*!\endcond */

#ifdef __cplusplus
}  // extern "C"
#endif

#include "aom_scale/yv12config.h"
#include "aom_ports/mem.h"
#include "aom/internal/aom_image_internal.h"
#include "aom_dsp/bitwriter_buffer.h"
#include "aom_dsp/bitwriter.h"
#include "aom_dsp/bitreader_buffer.h"
#include "aom_dsp/bitreader.h"
#include "aom_dsp/recenter.h"
#include "aom_dsp/flow_estimation/flow_estimation.h"
#include "aom_dsp/binary_codes_writer.h"
struct dist_wtd_comp_params;
typedef struct dist_wtd_comp_params DIST_WTD_COMP_PARAMS;
#include "aom_dsp/variance.h"
typedef uint16_t av2_tx_type;

#include "aom_dsp/noise_model.h"
static INLINE int av2_denoise_and_model_run(struct aom_denoise_and_model_t *ctx,
                                            const YV12_BUFFER_CONFIG *sd,
                                            aom_film_grain_t *film_grain) {
  return aom_denoise_and_model_run(ctx, sd, film_grain, film_grain->block_size);
}
#define av2_denoise_and_model_free aom_denoise_and_model_free

#define AOM_EFLAG_NO_UPD_ALL \
  (AOM_EFLAG_NO_UPD_LAST | AOM_EFLAG_NO_UPD_GF | AOM_EFLAG_NO_UPD_ARF)
#define AV2_BLEND_A64_MAX_ALPHA AOM_BLEND_A64_MAX_ALPHA
#define av2_s_frame_info aom_s_frame_info

/* Declare the _c SAD implementations directly and call them in the wrappers
 * below, preserving the original (non-dispatched) behavior without hijacking
 * the RTCD dispatch names (which collide with config/aom_dsp_rtcd.h). */
#ifdef __cplusplus
extern "C" {
#endif
extern unsigned int aom_highbd_sad8x8_c(const uint8_t *src_ptr, int src_stride,
                                        const uint8_t *ref_ptr, int ref_stride);
extern unsigned int aom_highbd_sad16x8_c(const uint8_t *src_ptr, int src_stride,
                                         const uint8_t *ref_ptr,
                                         int ref_stride);
extern unsigned int aom_highbd_sad8x16_c(const uint8_t *src_ptr, int src_stride,
                                         const uint8_t *ref_ptr,
                                         int ref_stride);
extern unsigned int aom_highbd_sad16x16_c(const uint8_t *src_ptr,
                                           int src_stride,
                                           const uint8_t *ref_ptr,
                                           int ref_stride);
#ifdef __cplusplus
}
#endif

static INLINE unsigned int av2_highbd_sad8x8(const uint16_t *src,
                                             int src_stride,
                                             const uint16_t *ref,
                                             int ref_stride) {
  return aom_highbd_sad8x8_c(CONVERT_TO_BYTEPTR(src), src_stride,
                             CONVERT_TO_BYTEPTR(ref), ref_stride);
}
static INLINE unsigned int av2_highbd_sad16x8(const uint16_t *src,
                                              int src_stride,
                                              const uint16_t *ref,
                                              int ref_stride) {
  return aom_highbd_sad16x8_c(CONVERT_TO_BYTEPTR(src), src_stride,
                              CONVERT_TO_BYTEPTR(ref), ref_stride);
}
static INLINE unsigned int av2_highbd_sad8x16(const uint16_t *src,
                                              int src_stride,
                                              const uint16_t *ref,
                                              int ref_stride) {
  return aom_highbd_sad8x16_c(CONVERT_TO_BYTEPTR(src), src_stride,
                              CONVERT_TO_BYTEPTR(ref), ref_stride);
}
static INLINE unsigned int av2_highbd_sad16x16(const uint16_t *src,
                                               int src_stride,
                                               const uint16_t *ref,
                                               int ref_stride) {
  return aom_highbd_sad16x16_c(CONVERT_TO_BYTEPTR(src), src_stride,
                               CONVERT_TO_BYTEPTR(ref), ref_stride);
}

struct yv12_buffer_config;
#ifdef __cplusplus
extern "C" {
#endif
extern void aom_yv12_partial_coloc_copy_y(
    const struct yv12_buffer_config *src_ybc,
    struct yv12_buffer_config *dst_ybc, int hstart, int hend, int vstart,
    int vend);
extern void aom_yv12_partial_coloc_copy_u(
    const struct yv12_buffer_config *src_ybc,
    struct yv12_buffer_config *dst_ybc, int hstart, int hend, int vstart,
    int vend);
extern void aom_yv12_partial_coloc_copy_v(
    const struct yv12_buffer_config *src_ybc,
    struct yv12_buffer_config *dst_ybc, int hstart, int hend, int vstart,
    int vend);
#ifdef __cplusplus
}
#endif

#include "av2/common/convolve.h"

#ifdef __cplusplus
extern "C" {
#endif
extern void aom_highbd_quantize_b_c(
    const tran_low_t *coeff_ptr, intptr_t n_coeffs, const int16_t *zbin_ptr,
    const int16_t *round_ptr, const int16_t *quant_ptr,
    const int16_t *quant_shift_ptr, tran_low_t *qcoeff_ptr,
    tran_low_t *dqcoeff_ptr, const int16_t *dequant_ptr, uint16_t *eob_ptr,
    const int16_t *scan, const int16_t *iscan);
#ifdef __cplusplus
}
#endif

static INLINE void av2_highbd_quantize_b(
    const tran_low_t *coeff_ptr, intptr_t n_coeffs, const int32_t *zbin_ptr,
    const int32_t *round_ptr, const int32_t *quant_ptr,
    const int32_t *quant_shift_ptr, tran_low_t *qcoeff_ptr,
    tran_low_t *dqcoeff_ptr, const int32_t *dequant_ptr, uint16_t *eob_ptr,
    const int16_t *scan, const int16_t *iscan, const int log_scale) {
  int16_t zbin[2] = { (int16_t)zbin_ptr[0], (int16_t)zbin_ptr[1] };
  int16_t round[2] = { (int16_t)round_ptr[0], (int16_t)round_ptr[1] };
  int16_t quant[2] = { (int16_t)quant_ptr[0], (int16_t)quant_ptr[1] };
  int16_t quant_shift[2] = { (int16_t)quant_shift_ptr[0],
                             (int16_t)quant_shift_ptr[1] };
  int16_t dequant[2] = { (int16_t)dequant_ptr[0], (int16_t)dequant_ptr[1] };
  (void)log_scale;

  aom_highbd_quantize_b_c(coeff_ptr, n_coeffs, zbin, round, quant, quant_shift,
                          qcoeff_ptr, dqcoeff_ptr, dequant, eob_ptr, scan,
                          iscan);
}

void av2_highbd_quantize_b_c(const tran_low_t *coeff_ptr, intptr_t n_coeffs,
                             const int32_t *zbin_ptr, const int32_t *round_ptr,
                             const int32_t *quant_ptr,
                             const int32_t *quant_shift_ptr,
                             tran_low_t *qcoeff_ptr, tran_low_t *dqcoeff_ptr,
                             const int32_t *dequant_ptr, uint16_t *eob_ptr,
                             const int16_t *scan, const int16_t *iscan,
                             const int log_scale);

#ifdef __cplusplus
extern "C" {
#endif
extern int64_t aom_highbd_sse_c(const uint8_t *src, int src_stride,
                                const uint8_t *ref, int ref_stride, int w,
                                int h);
#ifdef __cplusplus
}
#endif
static INLINE int64_t av2_highbd_sse(const uint16_t *a, int a_stride,
                                     const uint16_t *b, int b_stride, int width,
                                     int height) {
  return aom_highbd_sse_c(CONVERT_TO_BYTEPTR(a), a_stride, CONVERT_TO_BYTEPTR(b),
                          b_stride, width, height);
}

static INLINE int64_t av2_highbd_sse_c(const uint16_t *a, int a_stride,
                                       const uint16_t *b, int b_stride,
                                       int width, int height) {
  return aom_highbd_sse_c(CONVERT_TO_BYTEPTR(a), a_stride, CONVERT_TO_BYTEPTR(b),
                          b_stride, width, height);
}

#ifdef __cplusplus
extern "C" {
#endif
extern void aom_highbd_subtract_block_c(int rows, int cols, int16_t *diff_ptr,
                                        ptrdiff_t diff_stride,
                                        const uint8_t *src_ptr,
                                        ptrdiff_t src_stride,
                                        const uint8_t *pred_ptr,
                                        ptrdiff_t pred_stride);
#ifdef __cplusplus
}
#endif
static INLINE void av2_highbd_subtract_block(
    int rows, int cols, int16_t *diff_ptr, ptrdiff_t diff_stride,
    const uint16_t *src_ptr, ptrdiff_t src_stride, const uint16_t *pred_ptr,
    ptrdiff_t pred_stride, int bd) {
  (void)bd;
  aom_highbd_subtract_block_c(rows, cols, diff_ptr, diff_stride,
                              CONVERT_TO_BYTEPTR(src_ptr), src_stride,
                              CONVERT_TO_BYTEPTR(pred_ptr), pred_stride);
}

#ifdef __cplusplus
extern "C" {
#endif
extern void aom_highbd_blend_a64_d16_mask_c(
    uint8_t *dst, uint32_t dst_stride, const CONV_BUF_TYPE *src0,
    uint32_t src0_stride, const CONV_BUF_TYPE *src1, uint32_t src1_stride,
    const uint8_t *mask, uint32_t mask_stride, int w, int h, int subw, int subh,
    ConvolveParams *conv_params, const int bd);

extern void aom_highbd_blend_a64_mask_c(
    uint8_t *dst, uint32_t dst_stride, const uint8_t *src0,
    uint32_t src0_stride, const uint8_t *src1, uint32_t src1_stride,
    const uint8_t *mask, uint32_t mask_stride, int w, int h, int subw, int subh,
    int bd);
#ifdef __cplusplus
}
#endif

static INLINE void av2_highbd_blend_a64_d16_mask(
    void *dst, uint32_t dst_stride, const void *src0, uint32_t src0_stride,
    const void *src1, uint32_t src1_stride, const uint8_t *mask,
    uint32_t mask_stride, int w, int h, int subw, int subh, void *conv_params,
    const int bd) {
  // dst is a real uint16_t* pixel buffer; aom_highbd_blend_a64_d16_mask_c
  // applies CONVERT_TO_SHORTPTR to it, so tag it with CONVERT_TO_BYTEPTR. src0/
  // src1 are CONV_BUF_TYPE (uint16_t) intermediates used directly, not tagged.
  aom_highbd_blend_a64_d16_mask_c(
      CONVERT_TO_BYTEPTR((const uint16_t *)dst), dst_stride,
      (const CONV_BUF_TYPE *)src0, src0_stride, (const CONV_BUF_TYPE *)src1,
      src1_stride, mask, mask_stride, w, h, subw, subh,
      (ConvolveParams *)conv_params, bd);
}

static INLINE void av2_highbd_blend_a64_mask(
    void *dst, uint32_t dst_stride, const void *src0, uint32_t src0_stride,
    const void *src1, uint32_t src1_stride, const uint8_t *mask,
    uint32_t mask_stride, int w, int h, int subw, int subh, int bd) {
  // aom_highbd_blend_a64_mask_c applies CONVERT_TO_SHORTPTR to dst/src0/src1;
  // AV2 passes real uint16_t* pixel buffers, so tag them with CONVERT_TO_BYTEPTR
  // (mask is a real uint8_t* alpha map and is passed through unchanged).
  aom_highbd_blend_a64_mask_c(
      CONVERT_TO_BYTEPTR((const uint16_t *)dst), dst_stride,
      CONVERT_TO_BYTEPTR((const uint16_t *)src0), src0_stride,
      CONVERT_TO_BYTEPTR((const uint16_t *)src1), src1_stride, mask, mask_stride,
      w, h, subw, subh, bd);
}

#define ACCT_INFO(a, ...) a

static INLINE int av2_rb_read_inv_signed_literal(struct aom_read_bit_buffer *rb,
                                                 int bits) {
  const int nbits = sizeof(unsigned) * 8 - bits - 1;
  const unsigned value = (unsigned)aom_rb_read_literal(rb, bits + 1) << nbits;
  return ((int)value) >> nbits;
}

static INLINE uint16_t
av2_rb_read_primitive_quniform(struct aom_read_bit_buffer *rb, uint16_t n) {
  if (n <= 1) return 0;
  const int l = get_unsigned_bits(n - 1);
  const int m = (1 << l) - n;
  const int v = aom_rb_read_literal(rb, l - 1);
  return v < m ? v : (v << 1) - m + aom_rb_read_bit(rb);
}

static INLINE uint16_t av2_rb_read_primitive_subexpfin(
    struct aom_read_bit_buffer *rb, uint16_t n, uint16_t k) {
  int i = 0;
  int mk = 0;
  while (1) {
    int b = (i ? k + i - 1 : k);
    int a = (1 << b);
    if (n <= mk + 3 * a) {
      return av2_rb_read_primitive_quniform(rb, n - mk) + mk;
    }
    if (!aom_rb_read_bit(rb)) {
      return aom_rb_read_literal(rb, b) + mk;
    }
    i = i + 1;
    mk += a;
  }
  return 0;
}

static INLINE uint16_t av2_rb_read_primitive_refsubexpfin(
    struct aom_read_bit_buffer *rb, uint16_t n, uint16_t k, uint16_t ref) {
  assert(ref < n);
  return inv_recenter_finite_nonneg(n, ref,
                                    av2_rb_read_primitive_subexpfin(rb, n, k));
}

static INLINE int av2_read_unary(aom_reader *r, int max_nbits,
                                 const char *acct) {
  (void)acct;
  int ret = 0;
  for (int bit = 0; bit < max_nbits; bit++) {
    if (aom_read_bit(r, NULL)) {
      ret++;
    } else {
      break;
    }
  }
  return ret;
}

static INLINE uint16_t av2_read_primitive_quniform(aom_reader *r, uint16_t n,
                                                   const char *acct) {
  (void)acct;
  if (n <= 1) return 0;
  const int l = get_unsigned_bits(n - 1);
  const int m = (1 << l) - n;
  int v = aom_read_literal(r, l - 1, NULL);
  if (v < m) return v;
  return (v << 1) - m + aom_read_bit(r, NULL);
}


#ifndef symb_to_part_defined
#define symb_to_part_defined
static INLINE int symb_to_part(int symb, int nsymb_bits) {
  assert(nsymb_bits >= 3);
  int part_offs[4] = { 0, 1 << (nsymb_bits - 3), 1 << (nsymb_bits - 2),
                       1 << (nsymb_bits - 1) };
  if (symb < part_offs[1])
    return 0;
  else if (symb < part_offs[2])
    return 1;
  else if (symb < part_offs[3])
    return 2;
  else
    return 3;
}

static INLINE void av2_write_4part(aom_writer *w, int symb, aom_cdf_prob *cdf,
                                   int nsymb_bits) {
  assert(nsymb_bits >= 3);
  int part;
  int part_bits[4] = { (nsymb_bits - 3), (nsymb_bits - 3), (nsymb_bits - 2),
                       (nsymb_bits - 1) };
  int part_offs[4] = { 0, 1 << (nsymb_bits - 3), 1 << (nsymb_bits - 2),
                       1 << (nsymb_bits - 1) };
  if (symb < part_offs[1])
    part = 0;
  else if (symb < part_offs[2])
    part = 1;
  else if (symb < part_offs[3])
    part = 2;
  else
    part = 3;
  aom_write_symbol(w, part, cdf, 4);
  aom_write_literal(w, symb - part_offs[part], part_bits[part]);
}

static INLINE void av2_write_4part_wref(aom_writer *w, int ref_symb, int symb,
                                        aom_cdf_prob *cdf, int nsymb_bits) {
  const int recentered_symb =
      recenter_finite_nonneg(1 << nsymb_bits, ref_symb, symb);
  av2_write_4part(w, recentered_symb, cdf, nsymb_bits);
}

static INLINE int64_t av2_count_4part(int symb, const int *part_cost,
                                      int nsymb_bits, int scale_shift) {
  assert(nsymb_bits >= 3);
  int part_bits[4] = { (nsymb_bits - 3), (nsymb_bits - 3), (nsymb_bits - 2),
                       (nsymb_bits - 1) };
  int part_offs[4] = { 0, 1 << (nsymb_bits - 3), 1 << (nsymb_bits - 2),
                       1 << (nsymb_bits - 1) };
  if (symb < part_offs[1])
    return part_cost[0] + (part_bits[0] << scale_shift);
  else if (symb < part_offs[2])
    return part_cost[1] + (part_bits[1] << scale_shift);
  else if (symb < part_offs[3])
    return part_cost[2] + (part_bits[2] << scale_shift);
  else
    return part_cost[3] + (part_bits[3] << scale_shift);
}

static INLINE int64_t av2_count_4part_wref(int ref_symb, int symb,
                                           const int *part_cost, int nsymb_bits,
                                           int scale_shift) {
  const int recentered_symb =
      recenter_finite_nonneg(1 << nsymb_bits, ref_symb, symb);
  return av2_count_4part(recentered_symb, part_cost, nsymb_bits, scale_shift);
}
#endif



static INLINE int av2_realloc_frame_buffer(YV12_BUFFER_CONFIG *ybf, int width,
                                           int height, int ss_x, int ss_y,
                                           int border, int byte_alignment,
                                           aom_codec_frame_buffer_t *fb,
                                           aom_get_frame_buffer_cb_fn_t cb,
                                           void *cb_priv, bool alloc_pyramid) {
  return aom_realloc_frame_buffer(ybf, width, height, ss_x, ss_y,
                                  1, border, byte_alignment,
                                  fb, cb, cb_priv, alloc_pyramid, 0);
}

static INLINE int av2_alloc_frame_buffer(YV12_BUFFER_CONFIG *ybf, int width,
                                         int height, int ss_x, int ss_y,
                                         int border, int byte_alignment,
                                         bool alloc_pyramid) {
  return aom_alloc_frame_buffer(ybf, width, height, ss_x, ss_y,
                                1, border, byte_alignment,
                                alloc_pyramid, 0);
}

static INLINE int av2_read_4part_(aom_reader *r, aom_cdf_prob *cdf,
                                  int nsymb_bits) {
  assert(nsymb_bits >= 3);
  int part_bits[4] = { (nsymb_bits - 3), (nsymb_bits - 3), (nsymb_bits - 2),
                       (nsymb_bits - 1) };
  int part_offs[4] = { 0, 1 << (nsymb_bits - 3), 1 << (nsymb_bits - 2),
                       1 << (nsymb_bits - 1) };
  const int part = aom_read_symbol(r, cdf, 4, NULL);
  return aom_read_literal(r, part_bits[part], NULL) + part_offs[part];
}

static INLINE int av2_read_4part_wref(aom_reader *r, int ref_symb,
                                      aom_cdf_prob *cdf, int nsymb_bits,
                                      const char *acct) {
  (void)acct;
  const int symb = av2_read_4part_(r, cdf, nsymb_bits);
  return inv_recenter_finite_nonneg(1 << nsymb_bits, ref_symb, symb);
}

static INLINE uint16_t av2_rb_read_primitive_ref_quniform(
    struct aom_read_bit_buffer *rb, uint16_t n, uint16_t r) {
  if (aom_rb_read_bit(rb)) {
    int v = av2_rb_read_primitive_quniform(rb, n - 1);
    v += (v >= r);
    return v;
  } else {
    return r;
  }
}


static INLINE int32_t av2_rb_read_svlc(struct aom_read_bit_buffer *rb) {
  const uint32_t value = aom_rb_read_uvlc(rb);
  if (value == UINT32_MAX) return INT32_MIN;
  const int32_t ceil_half = (int32_t)((value + 1) / 2);
  return (value % 2) ? ceil_half : -ceil_half;
}

static INLINE uint32_t av2_rb_read_rice_golomb(struct aom_read_bit_buffer *rb,
                                               int k) {
  assert(k <= 26);
  uint32_t quotient = 0;
  uint32_t remainder = 0;
  uint32_t M = 1 << k;
  while (quotient < 32 && aom_rb_read_bit(rb) != 0) quotient++;
  if (quotient == 32) {
    return UINT32_MAX;
  }
  remainder = aom_rb_read_unsigned_literal(rb, k);
  return quotient * M + remainder;
}

static INLINE uint64_t aom_rb_read_uleb(struct aom_read_bit_buffer *rb) {
  uint64_t value = 0;
  int shift = 0;
  uint8_t byte;
  do {
    byte = aom_rb_read_literal(rb, 8);
    value |= (uint64_t)(byte & 0x7f) << shift;
    shift += 7;
  } while (byte & 0x80);
  return value;
}

#define FILTER_UNUSED -1

#ifdef __cplusplus
extern "C" {
#endif
extern void aom_highbd_convolve_copy_c(const uint16_t *src,
                                       ptrdiff_t src_stride, uint16_t *dst,
                                       ptrdiff_t dst_stride, int w, int h);
#ifdef __cplusplus
}
#endif
#define av2_highbd_convolve_copy(src, src_stride, dst, dst_stride, w, h) \
  ((((intptr_t)(dst) & 15) != 0 || ((dst_stride) & 7) != 0)              \
       ? aom_highbd_convolve_copy_c(src, src_stride, dst, dst_stride, w, \
                                    h)                                   \
       : aom_highbd_convolve_copy(src, src_stride, dst, dst_stride, w, h))

static INLINE uint64_t av2_sum_squares_i32(const int32_t *src, uint32_t n) {
  uint64_t sum = 0;
  for (uint32_t i = 0; i < n; ++i) {
    sum += (uint64_t)src[i] * src[i];
  }
  return sum;
}
#define DIVIDE_AND_ROUND_SIGNED(x, y) \
  (((x) + (((x) < 0 ? -(y) : (y)) >> 1)) / (y))

#include "aom_mem/aom_mem.h"

#ifdef __cplusplus
extern "C" {
#endif
extern void av2_highbd_subtract_block_vert(
    int rows, int cols, int16_t *diff_ptr, ptrdiff_t diff_stride,
    const uint16_t *src_ptr, ptrdiff_t src_stride, const uint16_t *pred_ptr,
    ptrdiff_t pred_stride, int bd);
extern void av2_highbd_subtract_block_horz(
    int rows, int cols, int16_t *diff_ptr, ptrdiff_t diff_stride,
    const uint16_t *src_ptr, ptrdiff_t src_stride, const uint16_t *pred_ptr,
    ptrdiff_t pred_stride, int bd);
#ifdef __cplusplus
}
#endif

#define INTERP_EVAL_INVALID -1

struct yv12_buffer_config;
#ifdef __cplusplus
extern "C" {
#endif
extern void aom_yv12_copy_y(const struct yv12_buffer_config *src_ybc,
                            struct yv12_buffer_config *dst_ybc, int use_crop);
extern void aom_yv12_copy_u(const struct yv12_buffer_config *src_ybc,
                            struct yv12_buffer_config *dst_ybc, int use_crop);
extern void aom_yv12_copy_v(const struct yv12_buffer_config *src_ybc,
                            struct yv12_buffer_config *dst_ybc, int use_crop);
extern int64_t aom_get_sse_plane(const struct yv12_buffer_config *a,
                                 const struct yv12_buffer_config *b, int plane,
                                 int highbd);
extern uint64_t aom_highbd_sse_odd_size(const uint8_t *a, int a_stride,
                                        const uint8_t *b, int b_stride, int w,
                                        int h);
#ifdef __cplusplus
}
#endif

static INLINE uint64_t av2_highbd_sse_odd_size(const uint16_t *a, int a_stride,
                                               const uint16_t *b, int b_stride,
                                               int w, int h) {
  // aom_highbd_sse_odd_size() expects tagged byte pointers (it applies
  // CONVERT_TO_SHORTPTR internally); AV2 passes real uint16_t* buffers, so tag
  // them with CONVERT_TO_BYTEPTR like the other shims above.
  return aom_highbd_sse_odd_size(CONVERT_TO_BYTEPTR(a), a_stride,
                                 CONVERT_TO_BYTEPTR(b), b_stride, w, h);
}

static INLINE void av2_yv12_copy_y(const struct yv12_buffer_config *src_ybc,
                                   struct yv12_buffer_config *dst_ybc) {
  aom_yv12_copy_y(src_ybc, dst_ybc, 0);
}
static INLINE void av2_yv12_copy_u(const struct yv12_buffer_config *src_ybc,
                                   struct yv12_buffer_config *dst_ybc) {
  aom_yv12_copy_u(src_ybc, dst_ybc, 0);
}
static INLINE void av2_yv12_copy_v(const struct yv12_buffer_config *src_ybc,
                                   struct yv12_buffer_config *dst_ybc) {
  aom_yv12_copy_v(src_ybc, dst_ybc, 0);
}
static INLINE int64_t av2_get_sse_plane(const struct yv12_buffer_config *a,
                                        const struct yv12_buffer_config *b,
                                        int plane) {
  return aom_get_sse_plane(a, b, plane, 1);
}

static INLINE int32_t clamp64_to_32(int64_t value) {
  return (int32_t)clamp64(value, INT_MIN, INT_MAX);
}

#define aom_reset_mmx_state aom_reset_mmx_state_dummy
static INLINE void aom_reset_mmx_state_dummy(void) {}

#define aom_highbd_ibp_dc_top_predictor_4x4_c \
  av2_highbd_ibp_dc_top_predictor_4x4_c
#define aom_highbd_ibp_dc_top_predictor_8x8_c \
  av2_highbd_ibp_dc_top_predictor_8x8_c
#define aom_highbd_ibp_dc_top_predictor_16x16_c \
  av2_highbd_ibp_dc_top_predictor_16x16_c
#define aom_highbd_ibp_dc_top_predictor_32x32_c \
  av2_highbd_ibp_dc_top_predictor_32x32_c
#define aom_highbd_ibp_dc_top_predictor_64x64_c \
  av2_highbd_ibp_dc_top_predictor_64x64_c
#define aom_highbd_ibp_dc_top_predictor_4x8_c \
  av2_highbd_ibp_dc_top_predictor_4x8_c
#define aom_highbd_ibp_dc_top_predictor_8x4_c \
  av2_highbd_ibp_dc_top_predictor_8x4_c
#define aom_highbd_ibp_dc_top_predictor_8x16_c \
  av2_highbd_ibp_dc_top_predictor_8x16_c
#define aom_highbd_ibp_dc_top_predictor_16x8_c \
  av2_highbd_ibp_dc_top_predictor_16x8_c
#define aom_highbd_ibp_dc_top_predictor_16x32_c \
  av2_highbd_ibp_dc_top_predictor_16x32_c
#define aom_highbd_ibp_dc_top_predictor_32x16_c \
  av2_highbd_ibp_dc_top_predictor_32x16_c
#define aom_highbd_ibp_dc_top_predictor_32x64_c \
  av2_highbd_ibp_dc_top_predictor_32x64_c
#define aom_highbd_ibp_dc_top_predictor_64x32_c \
  av2_highbd_ibp_dc_top_predictor_64x32_c
#define aom_highbd_ibp_dc_top_predictor_4x16_c \
  av2_highbd_ibp_dc_top_predictor_4x16_c
#define aom_highbd_ibp_dc_top_predictor_16x4_c \
  av2_highbd_ibp_dc_top_predictor_16x4_c
#define aom_highbd_ibp_dc_top_predictor_8x32_c \
  av2_highbd_ibp_dc_top_predictor_8x32_c
#define aom_highbd_ibp_dc_top_predictor_32x8_c \
  av2_highbd_ibp_dc_top_predictor_32x8_c
#define aom_highbd_ibp_dc_top_predictor_16x64_c \
  av2_highbd_ibp_dc_top_predictor_16x64_c
#define aom_highbd_ibp_dc_top_predictor_64x16_c \
  av2_highbd_ibp_dc_top_predictor_64x16_c
#define aom_highbd_ibp_dc_top_predictor_4x32_c \
  av2_highbd_ibp_dc_top_predictor_4x32_c
#define aom_highbd_ibp_dc_top_predictor_32x4_c \
  av2_highbd_ibp_dc_top_predictor_32x4_c
#define aom_highbd_ibp_dc_top_predictor_8x64_c \
  av2_highbd_ibp_dc_top_predictor_8x64_c
#define aom_highbd_ibp_dc_top_predictor_64x8_c \
  av2_highbd_ibp_dc_top_predictor_64x8_c
#define aom_highbd_ibp_dc_top_predictor_4x64_c \
  av2_highbd_ibp_dc_top_predictor_4x64_c
#define aom_highbd_ibp_dc_top_predictor_64x4_c \
  av2_highbd_ibp_dc_top_predictor_64x4_c

#define aom_highbd_ibp_dc_left_predictor_4x4_c \
  av2_highbd_ibp_dc_left_predictor_4x4_c
#define aom_highbd_ibp_dc_left_predictor_8x8_c \
  av2_highbd_ibp_dc_left_predictor_8x8_c
#define aom_highbd_ibp_dc_left_predictor_16x16_c \
  av2_highbd_ibp_dc_left_predictor_16x16_c
#define aom_highbd_ibp_dc_left_predictor_32x32_c \
  av2_highbd_ibp_dc_left_predictor_32x32_c
#define aom_highbd_ibp_dc_left_predictor_64x64_c \
  av2_highbd_ibp_dc_left_predictor_64x64_c
#define aom_highbd_ibp_dc_left_predictor_4x8_c \
  av2_highbd_ibp_dc_left_predictor_4x8_c
#define aom_highbd_ibp_dc_left_predictor_8x4_c \
  av2_highbd_ibp_dc_left_predictor_8x4_c
#define aom_highbd_ibp_dc_left_predictor_8x16_c \
  av2_highbd_ibp_dc_left_predictor_8x16_c
#define aom_highbd_ibp_dc_left_predictor_16x8_c \
  av2_highbd_ibp_dc_left_predictor_16x8_c
#define aom_highbd_ibp_dc_left_predictor_16x32_c \
  av2_highbd_ibp_dc_left_predictor_16x32_c
#define aom_highbd_ibp_dc_left_predictor_32x16_c \
  av2_highbd_ibp_dc_left_predictor_32x16_c
#define aom_highbd_ibp_dc_left_predictor_32x64_c \
  av2_highbd_ibp_dc_left_predictor_32x64_c
#define aom_highbd_ibp_dc_left_predictor_64x32_c \
  av2_highbd_ibp_dc_left_predictor_64x32_c
#define aom_highbd_ibp_dc_left_predictor_4x16_c \
  av2_highbd_ibp_dc_left_predictor_4x16_c
#define aom_highbd_ibp_dc_left_predictor_16x4_c \
  av2_highbd_ibp_dc_left_predictor_16x4_c
#define aom_highbd_ibp_dc_left_predictor_8x32_c \
  av2_highbd_ibp_dc_left_predictor_8x32_c
#define aom_highbd_ibp_dc_left_predictor_32x8_c \
  av2_highbd_ibp_dc_left_predictor_32x8_c
#define aom_highbd_ibp_dc_left_predictor_16x64_c \
  av2_highbd_ibp_dc_left_predictor_16x64_c
#define aom_highbd_ibp_dc_left_predictor_64x16_c \
  av2_highbd_ibp_dc_left_predictor_64x16_c
#define aom_highbd_ibp_dc_left_predictor_4x32_c \
  av2_highbd_ibp_dc_left_predictor_4x32_c
#define aom_highbd_ibp_dc_left_predictor_32x4_c \
  av2_highbd_ibp_dc_left_predictor_32x4_c
#define aom_highbd_ibp_dc_left_predictor_8x64_c \
  av2_highbd_ibp_dc_left_predictor_8x64_c
#define aom_highbd_ibp_dc_left_predictor_64x8_c \
  av2_highbd_ibp_dc_left_predictor_64x8_c
#define aom_highbd_ibp_dc_left_predictor_4x64_c \
  av2_highbd_ibp_dc_left_predictor_4x64_c
#define aom_highbd_ibp_dc_left_predictor_64x4_c \
  av2_highbd_ibp_dc_left_predictor_64x4_c

#define aom_highbd_ibp_dc_predictor_4x4_c av2_highbd_ibp_dc_predictor_4x4_c
#define aom_highbd_ibp_dc_predictor_8x8_c av2_highbd_ibp_dc_predictor_8x8_c
#define aom_highbd_ibp_dc_predictor_16x16_c av2_highbd_ibp_dc_predictor_16x16_c
#define aom_highbd_ibp_dc_predictor_32x32_c av2_highbd_ibp_dc_predictor_32x32_c
#define aom_highbd_ibp_dc_predictor_64x64_c av2_highbd_ibp_dc_predictor_64x64_c
#define aom_highbd_ibp_dc_predictor_4x8_c av2_highbd_ibp_dc_predictor_4x8_c
#define aom_highbd_ibp_dc_predictor_8x4_c av2_highbd_ibp_dc_predictor_8x4_c
#define aom_highbd_ibp_dc_predictor_8x16_c av2_highbd_ibp_dc_predictor_8x16_c
#define aom_highbd_ibp_dc_predictor_16x8_c av2_highbd_ibp_dc_predictor_16x8_c
#define aom_highbd_ibp_dc_predictor_16x32_c av2_highbd_ibp_dc_predictor_16x32_c
#define aom_highbd_ibp_dc_predictor_32x16_c av2_highbd_ibp_dc_predictor_32x16_c
#define aom_highbd_ibp_dc_predictor_32x64_c av2_highbd_ibp_dc_predictor_32x64_c
#define aom_highbd_ibp_dc_predictor_64x32_c av2_highbd_ibp_dc_predictor_64x32_c
#define aom_highbd_ibp_dc_predictor_4x16_c av2_highbd_ibp_dc_predictor_4x16_c
#define aom_highbd_ibp_dc_predictor_16x4_c av2_highbd_ibp_dc_predictor_16x4_c
#define aom_highbd_ibp_dc_predictor_8x32_c av2_highbd_ibp_dc_predictor_8x32_c
#define aom_highbd_ibp_dc_predictor_32x8_c av2_highbd_ibp_dc_predictor_32x8_c
#define aom_highbd_ibp_dc_predictor_16x64_c av2_highbd_ibp_dc_predictor_16x64_c
#define aom_highbd_ibp_dc_predictor_64x16_c av2_highbd_ibp_dc_predictor_64x16_c
#define aom_highbd_ibp_dc_predictor_4x32_c av2_highbd_ibp_dc_predictor_4x32_c
#define aom_highbd_ibp_dc_predictor_32x4_c av2_highbd_ibp_dc_predictor_32x4_c
#define aom_highbd_ibp_dc_predictor_8x64_c av2_highbd_ibp_dc_predictor_8x64_c
#define aom_highbd_ibp_dc_predictor_64x8_c av2_highbd_ibp_dc_predictor_64x8_c
#define aom_highbd_ibp_dc_predictor_4x64_c av2_highbd_ibp_dc_predictor_4x64_c
#define aom_highbd_ibp_dc_predictor_64x4_c av2_highbd_ibp_dc_predictor_64x4_c

#define aom_highbd_dc_128_predictor_4x32_c av2_highbd_dc_128_predictor_4x32_c
#define aom_highbd_dc_128_predictor_32x4_c av2_highbd_dc_128_predictor_32x4_c
#define aom_highbd_dc_128_predictor_8x64_c av2_highbd_dc_128_predictor_8x64_c
#define aom_highbd_dc_128_predictor_64x8_c av2_highbd_dc_128_predictor_64x8_c
#define aom_highbd_dc_128_predictor_4x64_c av2_highbd_dc_128_predictor_4x64_c
#define aom_highbd_dc_128_predictor_64x4_c av2_highbd_dc_128_predictor_64x4_c

#define aom_highbd_v_predictor_4x32_c av2_highbd_v_predictor_4x32_c
#define aom_highbd_v_predictor_32x4_c av2_highbd_v_predictor_32x4_c
#define aom_highbd_v_predictor_8x64_c av2_highbd_v_predictor_8x64_c
#define aom_highbd_v_predictor_64x8_c av2_highbd_v_predictor_64x8_c
#define aom_highbd_v_predictor_4x64_c av2_highbd_v_predictor_4x64_c
#define aom_highbd_v_predictor_64x4_c av2_highbd_v_predictor_64x4_c

#define aom_highbd_h_predictor_4x32_c av2_highbd_h_predictor_4x32_c
#define aom_highbd_h_predictor_32x4_c av2_highbd_h_predictor_32x4_c
#define aom_highbd_h_predictor_8x64_c av2_highbd_h_predictor_8x64_c
#define aom_highbd_h_predictor_64x8_c av2_highbd_h_predictor_64x8_c
#define aom_highbd_h_predictor_4x64_c av2_highbd_h_predictor_4x64_c
#define aom_highbd_h_predictor_64x4_c av2_highbd_h_predictor_64x4_c

#define aom_highbd_smooth_predictor_4x32_c av2_highbd_smooth_predictor_4x32_c
#define aom_highbd_smooth_predictor_32x4_c av2_highbd_smooth_predictor_32x4_c
#define aom_highbd_smooth_predictor_8x64_c av2_highbd_smooth_predictor_8x64_c
#define aom_highbd_smooth_predictor_64x8_c av2_highbd_smooth_predictor_64x8_c
#define aom_highbd_smooth_predictor_4x64_c av2_highbd_smooth_predictor_4x64_c
#define aom_highbd_smooth_predictor_64x4_c av2_highbd_smooth_predictor_64x4_c

#define aom_highbd_smooth_v_predictor_4x32_c \
  av2_highbd_smooth_v_predictor_4x32_c
#define aom_highbd_smooth_v_predictor_32x4_c \
  av2_highbd_smooth_v_predictor_32x4_c
#define aom_highbd_smooth_v_predictor_8x64_c \
  av2_highbd_smooth_v_predictor_8x64_c
#define aom_highbd_smooth_v_predictor_64x8_c \
  av2_highbd_smooth_v_predictor_64x8_c
#define aom_highbd_smooth_v_predictor_4x64_c \
  av2_highbd_smooth_v_predictor_4x64_c
#define aom_highbd_smooth_v_predictor_64x4_c \
  av2_highbd_smooth_v_predictor_64x4_c

#define aom_highbd_smooth_h_predictor_4x32_c \
  av2_highbd_smooth_h_predictor_4x32_c
#define aom_highbd_smooth_h_predictor_32x4_c \
  av2_highbd_smooth_h_predictor_32x4_c
#define aom_highbd_smooth_h_predictor_8x64_c \
  av2_highbd_smooth_h_predictor_8x64_c
#define aom_highbd_smooth_h_predictor_64x8_c \
  av2_highbd_smooth_h_predictor_64x8_c
#define aom_highbd_smooth_h_predictor_4x64_c \
  av2_highbd_smooth_h_predictor_4x64_c
#define aom_highbd_smooth_h_predictor_64x4_c \
  av2_highbd_smooth_h_predictor_64x4_c

#define aom_highbd_paeth_predictor_4x32_c av2_highbd_paeth_predictor_4x32_c
#define aom_highbd_paeth_predictor_32x4_c av2_highbd_paeth_predictor_32x4_c
#define aom_highbd_paeth_predictor_8x64_c av2_highbd_paeth_predictor_8x64_c
#define aom_highbd_paeth_predictor_64x8_c av2_highbd_paeth_predictor_64x8_c
#define aom_highbd_paeth_predictor_4x64_c av2_highbd_paeth_predictor_4x64_c
#define aom_highbd_paeth_predictor_64x4_c av2_highbd_paeth_predictor_64x4_c

#define aom_highbd_dc_top_predictor_4x32_c av2_highbd_dc_top_predictor_4x32_c
#define aom_highbd_dc_top_predictor_32x4_c av2_highbd_dc_top_predictor_32x4_c
#define aom_highbd_dc_top_predictor_8x64_c av2_highbd_dc_top_predictor_8x64_c
#define aom_highbd_dc_top_predictor_64x8_c av2_highbd_dc_top_predictor_64x8_c
#define aom_highbd_dc_top_predictor_4x64_c av2_highbd_dc_top_predictor_4x64_c
#define aom_highbd_dc_top_predictor_64x4_c av2_highbd_dc_top_predictor_64x4_c

#define aom_highbd_dc_left_predictor_4x32_c av2_highbd_dc_left_predictor_4x32_c
#define aom_highbd_dc_left_predictor_32x4_c av2_highbd_dc_left_predictor_32x4_c
#define aom_highbd_dc_left_predictor_8x64_c av2_highbd_dc_left_predictor_8x64_c
#define aom_highbd_dc_left_predictor_64x8_c av2_highbd_dc_left_predictor_64x8_c
#define aom_highbd_dc_left_predictor_4x64_c av2_highbd_dc_left_predictor_4x64_c
#define aom_highbd_dc_left_predictor_64x4_c av2_highbd_dc_left_predictor_64x4_c

#define aom_highbd_dc_predictor_4x32_c av2_highbd_dc_predictor_4x32_c
#define aom_highbd_dc_predictor_32x4_c av2_highbd_dc_predictor_32x4_c
#define aom_highbd_dc_predictor_8x64_c av2_highbd_dc_predictor_8x64_c
#define aom_highbd_dc_predictor_64x8_c av2_highbd_dc_predictor_64x8_c
#define aom_highbd_dc_predictor_4x64_c av2_highbd_dc_predictor_4x64_c
#define aom_highbd_dc_predictor_64x4_c av2_highbd_dc_predictor_64x4_c

#define av2_add_film_grain av1_add_film_grain

#endif  // AOM_AV2_COMMON_ENUMS_H_
