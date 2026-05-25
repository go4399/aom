#ifndef AVM_AVM_INTEGER_H_
#define AVM_AVM_INTEGER_H_

#include <stdbool.h>
#include "config/aom_config.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_decoder.h"
#include "aom_dsp/grain_params.h"
#include "aom_dsp/grain_table.h"
#include "aom_ports/mem.h"
#include "aom_scale/yv12config.h"

// Compatibility types and mappings
typedef uint16_t avm_cdf_prob;
typedef aom_image_t avm_image_t;
typedef aom_metadata_insert_flags_t avm_metadata_insert_flags_t;
typedef aom_codec_err_t avm_codec_err_t;
typedef aom_codec_frame_buffer_t avm_codec_frame_buffer_t;
typedef aom_film_grain_t avm_film_grain_t;
typedef aom_get_frame_buffer_cb_fn_t avm_get_frame_buffer_cb_fn_t;
typedef aom_release_frame_buffer_cb_fn_t avm_release_frame_buffer_cb_fn_t;
typedef aom_color_primaries_t avm_color_primaries_t;
typedef aom_transfer_characteristics_t avm_transfer_characteristics_t;
typedef aom_matrix_coefficients_t avm_matrix_coefficients_t;
typedef long avm_enc_frame_flags_t;
typedef aom_chroma_sample_position_t avm_chroma_sample_position_t;
typedef aom_color_range_t avm_color_range_t;
typedef enum avm_superblock_size {
  AVM_SUPERBLOCK_SIZE_64X64,
  AVM_SUPERBLOCK_SIZE_128X128,
  AVM_SUPERBLOCK_SIZE_256X256,
  AVM_SUPERBLOCK_SIZE_DYNAMIC
} avm_superblock_size_t;
typedef aom_codec_ctx_t avm_codec_ctx_t;
typedef aom_codec_priv_t avm_codec_priv_t;
typedef aom_codec_iface_t avm_codec_iface_t;
typedef int64_t avm_codec_pts_t;
typedef uint32_t avm_codec_flags_t;
typedef aom_codec_caps_t avm_codec_caps_t;
typedef aom_codec_dec_cfg_t avm_codec_dec_cfg_t;
typedef struct avm_codec_alg_priv avm_codec_alg_priv_t;
typedef aom_codec_iter_t avm_codec_iter_t;
typedef aom_metadata_array_t avm_metadata_array_t;
typedef aom_codec_stream_info_t avm_codec_stream_info_t;
typedef aom_img_fmt_t avm_img_fmt_t;
typedef struct avm_reader avm_reader;
#ifndef AVM_S_FRAME_INFO_DEFINED
#define AVM_S_FRAME_INFO_DEFINED
typedef struct avm_s_frame_info {
  int is_s_frame;
  int is_s_frame_at_altref;
} avm_s_frame_info;
#endif

int av2_get_bitdepth_from_index(uint32_t bitdepth_lut_idx);

// Forward-declared entropy coder structures
typedef struct avm_writer avm_writer;

// Forward declarations for DSP typedefs to prevent circular header dependencies
struct dist_wtd_comp_params;

typedef unsigned int (*avm_sad_fn_t)(const uint16_t *a, int a_stride,
                                     const uint16_t *b, int b_stride);

typedef unsigned int (*avm_sad_avg_fn_t)(const uint16_t *a, int a_stride,
                                         const uint16_t *b, int b_stride,
                                         const uint16_t *second_pred);

typedef void (*avm_copy32xn_fn_t)(const uint16_t *a, int a_stride, uint16_t *b,
                                  int b_stride, int n);

typedef void (*avm_sad_multi_d_fn_t)(const uint16_t *a, int a_stride,
                                     const uint16_t *const b_array[],
                                     int b_stride, unsigned int *sad_array);

typedef unsigned int (*avm_variance_fn_t)(const uint8_t *a, int a_stride,
                                          const uint8_t *b, int b_stride,
                                          unsigned int *sse);

typedef unsigned int (*avm_subpixvariance_fn_t)(const uint8_t *a, int a_stride,
                                                int xoffset, int yoffset,
                                                const uint8_t *b, int b_stride,
                                                unsigned int *sse);

typedef unsigned int (*avm_subp_avg_variance_fn_t)(
    const uint8_t *a, int a_stride, int xoffset, int yoffset,
    const uint8_t *b, int b_stride, unsigned int *sse,
    const uint8_t *second_pred);

typedef unsigned int (*avm_dist_wtd_sad_avg_fn_t)(
    const uint8_t *a, int a_stride, const uint8_t *b, int b_stride,
    const uint8_t *second_pred, const struct dist_wtd_comp_params *jcp_param);

typedef unsigned int (*avm_dist_wtd_subp_avg_variance_fn_t)(
    const uint8_t *a, int a_stride, int xoffset, int yoffset,
    const uint8_t *b, int b_stride, unsigned int *sse,
    const uint8_t *second_pred, const struct dist_wtd_comp_params *jcp_param);

typedef unsigned int (*avm_masked_sad_fn_t)(const uint8_t *src, int src_stride,
                                            const uint8_t *ref, int ref_stride,
                                            const uint8_t *second_pred,
                                            const uint8_t *msk, int msk_stride,
                                            int invert_mask);

typedef unsigned int (*avm_masked_subpixvariance_fn_t)(
    const uint8_t *src, int src_stride, int xoffset, int yoffset,
    const uint8_t *ref, int ref_stride, const uint8_t *second_pred,
    const uint8_t *msk, int msk_stride, int invert_mask, unsigned int *sse);

typedef struct avm_variance_vtable {
  avm_sad_fn_t sdf;
  avm_sad_fn_t sdsf;
  avm_sad_avg_fn_t sdaf;
  avm_variance_fn_t vf;
  avm_subpixvariance_fn_t svf;
  avm_subp_avg_variance_fn_t svaf;
  avm_sad_multi_d_fn_t sdx4df;
  avm_sad_multi_d_fn_t sdsx4df;
  avm_masked_sad_fn_t msdf;
  avm_masked_subpixvariance_fn_t msvf;
  avm_dist_wtd_sad_avg_fn_t jsdaf;
  avm_dist_wtd_subp_avg_variance_fn_t jsvaf;
} avm_variance_fn_ptr_t;

#define DIVIDE_AND_ROUND_SIGNED(n, d) \
  ((((n) < 0) ^ ((d) < 0)) ? (((n) - (d) / 2) / (d)) : (((n) + (d) / 2) / (d)))

static inline void *avm_memset_int16(void *dest, int16_t val, size_t length) {
  size_t i;
  int16_t *dest16 = (int16_t *)dest;
  for (i = 0; i < length; i++) *dest16++ = val;
  return dest;
}

void avm_highbd_subtract_block_vert(int rows, int cols, int16_t *diff,
                                    ptrdiff_t diff_stride, const uint16_t *src,
                                    ptrdiff_t src_stride, const uint16_t *pred,
                                    ptrdiff_t pred_stride, int bd);

void avm_highbd_subtract_block_horz(int rows, int cols, int16_t *diff,
                                    ptrdiff_t diff_stride, const uint16_t *src,
                                    ptrdiff_t src_stride, const uint16_t *pred,
                                    ptrdiff_t pred_stride, int bd);

typedef enum avm_bit_depth {
  AVM_BITS_8 = 8,
  AVM_BITS_10 = 10,
  AVM_BITS_12 = 12,
} avm_bit_depth_t;

typedef enum avm_bit_depth_lut_index {
  AVM_BITDEPTH_0 = 0,
  AVM_BITDEPTH_1 = 1,
  AVM_BITDEPTH_2 = 2,
} avm_bit_depth_lut_index_t;

// Compatibility inline and unused keywords (AOM uses lowercase 'inline' natively in C, so define INLINE and AVM_INLINE as 'inline')
#ifndef INLINE
#define INLINE inline
#endif
#define AVM_INLINE inline
#ifndef AVM_FORCE_INLINE
#define AVM_FORCE_INLINE inline
#endif
#define AVM_UNUSED AOM_UNUSED

// Compatibility min/max macros
#define AVMMAX AOMMAX
#define AVMMIN AOMMIN

// Compatibility plane and interpolation constants
#define AVM_PLANE_Y AOM_PLANE_Y
#define AVM_PLANE_U AOM_PLANE_U
#define AVM_PLANE_V AOM_PLANE_V

#ifndef AVM_INTERP_EXTEND
#define AVM_INTERP_EXTEND 4
#endif

// Compatibility Rate Control and Usage Modes
#define avm_rc_mode aom_rc_mode
#define avm_enc_pass aom_enc_pass
#define avm_kf_mode aom_kf_mode
#define AVM_VBR AOM_VBR
#define AVM_CBR AOM_CBR
#define AVM_CQ AOM_CQ
#define AVM_Q AOM_Q
#define AVM_KF_AUTO AOM_KF_AUTO
#define AVM_RC_ONE_PASS AOM_RC_ONE_PASS
#define FIXED_QP_OFFSET_COUNT 6

typedef aom_codec_frame_flags_t avm_codec_frame_flags_t;

// Compatibility tuning options


// Compatibility CICP Color Profiles
#define AVM_CICP_MC_IDENTITY AOM_CICP_MC_IDENTITY
#define AVM_CICP_MC_BT_709 AOM_CICP_MC_BT_709
#define AVM_CICP_MC_UNSPECIFIED AOM_CICP_MC_UNSPECIFIED
#define AVM_CICP_MC_FCC AOM_CICP_MC_FCC
#define AVM_CICP_MC_BT_470_B_G AOM_CICP_MC_BT_470_B_G
#define AVM_CICP_MC_BT_601 AOM_CICP_MC_BT_601
#define AVM_CICP_MC_SMPTE_240 AOM_CICP_MC_SMPTE_240
#define AVM_CICP_MC_SMPTE_YCGCO AOM_CICP_MC_SMPTE_YCGCO
#define AVM_CICP_MC_BT_2020_NCL AOM_CICP_MC_BT_2020_NCL
#define AVM_CICP_MC_BT_2020_CL AOM_CICP_MC_BT_2020_CL
#define AVM_CICP_MC_SMPTE_2085 AOM_CICP_MC_SMPTE_2085
#define AVM_CICP_MC_CHROMAT_NCL AOM_CICP_MC_CHROMAT_NCL
#define AVM_CICP_MC_CHROMAT_CL AOM_CICP_MC_CHROMAT_CL
#define AVM_CICP_MC_ICTCP AOM_CICP_MC_ICTCP
#define AVM_CICP_MC_IPT_C2 15
#define AVM_CICP_MC_YCGCO_RE 16
#define AVM_CICP_MC_YCGCO_RO 17


// Compatibility thread & bounds limits
#define AVM_MAX_NUM_STREAMS 1
#define MAX_NUM_THREADS 64
#define AVM_MAXIMUM_REF_BUFFERS AOM_MAXIMUM_REF_BUFFERS
#define AVM_MAXIMUM_WORK_BUFFERS AOM_MAXIMUM_WORK_BUFFERS

// Color description enums and constants
typedef enum avm_color_description {
  AVM_COLOR_DESC_IDC_EXPLICIT = 0,
  AVM_COLOR_DESC_IDC_BT709SDR = 1,
  AVM_COLOR_DESC_IDC_BT2100PQ = 2,
  AVM_COLOR_DESC_IDC_BT2100HLG = 3,
  AVM_COLOR_DESC_IDC_SRGB = 4,
  AVM_COLOR_DESC_IDC_SYCC = 5,
} avm_color_description_t;

#define AVM_CICP_CP_BT_709 AOM_CICP_CP_BT_709
#define AVM_CICP_CP_UNSPECIFIED AOM_CICP_CP_UNSPECIFIED
#define AVM_CICP_CP_BT_2020 AOM_CICP_CP_BT_2020
#define AVM_CICP_TC_BT_709 AOM_CICP_TC_BT_709
#define AVM_CICP_TC_UNSPECIFIED AOM_CICP_TC_UNSPECIFIED
#define AVM_CICP_TC_SMPTE_2084 AOM_CICP_TC_SMPTE_2084
#define AVM_CICP_TC_HLG AOM_CICP_TC_HLG
#define AVM_CICP_TC_SRGB AOM_CICP_TC_SRGB
#define AVM_CICP_MC_BT_709 AOM_CICP_MC_BT_709
#define AVM_CICP_MC_UNSPECIFIED AOM_CICP_MC_UNSPECIFIED
#define AVM_CICP_MC_BT_2020_NCL AOM_CICP_MC_BT_2020_NCL
#define AVM_CICP_MC_IDENTITY AOM_CICP_MC_IDENTITY
#define AVM_CICP_MC_BT_470_B_G AOM_CICP_MC_BT_470_B_G
#define AVM_CICP_CP_UNSPECIFIED AOM_CICP_CP_UNSPECIFIED
#define AVM_CICP_TC_UNSPECIFIED AOM_CICP_TC_UNSPECIFIED
#define AVM_CICP_MC_UNSPECIFIED AOM_CICP_MC_UNSPECIFIED

#define AVM_CSP_UNSPECIFIED AOM_CSP_UNKNOWN

// Redirect buffers to buffers_u16 inside AV2 files (safely defined after yv12config.h has been parsed)
#define buffers buffers_u16
#define y_buffer buffers_u16[0]
#define u_buffer buffers_u16[1]
#define v_buffer buffers_u16[2]
#define store_buf_adr store_buf_adr_u16

// Compatibility structure mappings
#define cfg_options avm_cfg_options
#define cfg_options_t avm_cfg_options_t
#define avm_internal_error_info aom_internal_error_info
#define avm_internal_error aom_internal_error
#define avm_free aom_free
#define avm_malloc aom_malloc
#define avm_calloc aom_calloc
#define avm_memalign aom_memalign
#define avm_invalidate_pyramid aom_invalidate_pyramid
#define av2_invalidate_corner_list av1_invalidate_corner_list
#define AVM_CHECK_MEM_ERROR AOM_CHECK_MEM_ERROR
#define avm_get_worker_interface aom_get_worker_interface



// Mappings for CICP color space enums
#define AVM_CICP_CP_BT_601 AOM_CICP_CP_BT_601
#define AVM_CICP_CP_BT_470_M AOM_CICP_CP_BT_470_M
#define AVM_CICP_CP_BT_470_B_G AOM_CICP_CP_BT_470_B_G
#define AVM_CICP_CP_SMPTE_240 AOM_CICP_CP_SMPTE_240
#define AVM_CICP_CP_GENERIC_FILM AOM_CICP_CP_GENERIC_FILM
#define AVM_CICP_CP_XYZ AOM_CICP_CP_XYZ
#define AVM_CICP_CP_SMPTE_431 AOM_CICP_CP_SMPTE_431
#define AVM_CICP_CP_SMPTE_432 AOM_CICP_CP_SMPTE_432
#define AVM_CICP_CP_EBU_3213 AOM_CICP_CP_EBU_3213

#define AVM_CICP_TC_BT_470_M AOM_CICP_TC_BT_470_M
#define AVM_CICP_TC_BT_470_B_G AOM_CICP_TC_BT_470_B_G
#define AVM_CICP_TC_BT_601 AOM_CICP_TC_BT_601
#define AVM_CICP_TC_SMPTE_240 AOM_CICP_TC_SMPTE_240
#define AVM_CICP_TC_LINEAR AOM_CICP_TC_LINEAR
#define AVM_CICP_TC_LOG_100 AOM_CICP_TC_LOG_100
#define AVM_CICP_TC_LOG_100_SQRT10 AOM_CICP_TC_LOG_100_SQRT10
#define AVM_CICP_TC_IEC_61966 AOM_CICP_TC_IEC_61966
#define AVM_CICP_TC_BT_1361 AOM_CICP_TC_BT_1361
#define AVM_CICP_TC_BT_2020_10_BIT AOM_CICP_TC_BT_2020_10_BIT
#define AVM_CICP_TC_BT_2020_12_BIT AOM_CICP_TC_BT_2020_12_BIT
#define AVM_CICP_TC_SMPTE_428 AOM_CICP_TC_SMPTE_428

// Mappings for Chroma Sample Position
#define AVM_CSP_LEFT AOM_CSP_VERTICAL
#define AVM_CSP_CENTER AOM_CSP_COLOCATED
#define AVM_CSP_TOPLEFT 2
#define AVM_CSP_TOP AOM_CSP_UNKNOWN
#define AVM_CSP_BOTTOMLEFT AOM_CSP_UNKNOWN
#define AVM_CSP_BOTTOM AOM_CSP_UNKNOWN

static AVM_INLINE int32_t clamp64_to_32(int64_t value) {
  if (value < -2147483648LL) return -2147483648LL;
  if (value > 2147483647LL) return 2147483647LL;
  return (int32_t)value;
}

// Frame buffer allocator compatibility redirects
#define avm_free_frame_buffer aom_free_frame_buffer
#define avm_alloc_frame_buffer(ybf, width, height, ss_x, ss_y, border, byte_alignment, alloc_pyramid) \
  aom_alloc_frame_buffer(ybf, width, height, ss_x, ss_y, 1, border, byte_alignment, alloc_pyramid, 0)

#define avm_realloc_frame_buffer(ybf, width, height, ss_x, ss_y, border, byte_alignment, fb, cb, cb_priv, alloc_pyramid) \
  aom_realloc_frame_buffer(ybf, width, height, ss_x, ss_y, 1, border, byte_alignment, fb, cb, cb_priv, alloc_pyramid, 0)
#define avm_extend_frame_borders(ybf, num_planes, something) \
  aom_extend_frame_borders(ybf, num_planes)

// Memory utility redirects
#define avm_memset16 aom_memset16
#define avm_yv12_partial_coloc_copy_y aom_yv12_partial_coloc_copy_y
#define avm_yv12_partial_coloc_copy_u aom_yv12_partial_coloc_copy_u
#define avm_yv12_partial_coloc_copy_v aom_yv12_partial_coloc_copy_v

#define avm_highbd_convolve_copy aom_highbd_convolve_copy
#define avm_convolve_copy aom_convolve_copy

// Standard highbd SAD redirects
#define avm_highbd_sad8x8(src, src_stride, ref, ref_stride) \
  aom_highbd_sad8x8((const uint8_t *)(src), src_stride, (const uint8_t *)(ref), ref_stride)

#define avm_highbd_sad16x8(src, src_stride, ref, ref_stride) \
  aom_highbd_sad16x8((const uint8_t *)(src), src_stride, (const uint8_t *)(ref), ref_stride)

#define avm_highbd_sad8x16(src, src_stride, ref, ref_stride) \
  aom_highbd_sad8x16((const uint8_t *)(src), src_stride, (const uint8_t *)(ref), ref_stride)

#define avm_highbd_sad16x16(src, src_stride, ref, ref_stride) \
  aom_highbd_sad16x16((const uint8_t *)(src), src_stride, (const uint8_t *)(ref), ref_stride)

// Highbd blend utility redirects
#define avm_highbd_blend_a64_mask(dst, dst_stride, src0, src0_stride, src1, src1_stride, mask, mask_stride, w, h, subw, subh, bd) \
  aom_highbd_blend_a64_mask((uint8_t *)(dst), dst_stride, (const uint8_t *)(src0), src0_stride, (const uint8_t *)(src1), src1_stride, mask, mask_stride, w, h, subw, subh, bd)

#define avm_highbd_blend_a64_d16_mask(dst, dst_stride, src0, src0_stride, src1, src1_stride, mask, mask_stride, w, h, subw, subh, conv_params, bd) \
  aom_highbd_blend_a64_d16_mask((uint8_t *)(dst), dst_stride, src0, src0_stride, src1, src1_stride, mask, mask_stride, w, h, subw, subh, conv_params, bd)

// Bounds & margins redirects
#define AVM_BORDER_IN_PIXELS AOM_BORDER_IN_PIXELS

#define avm_uleb_decode aom_uleb_decode
#define avm_uleb_encode aom_uleb_encode
#define avm_uleb_encode_fixed_size aom_uleb_encode_fixed_size

typedef enum {
  AVM_OPFL_REFINE_NONE,
  AVM_OPFL_REFINE_SWITCHABLE,
  AVM_OPFL_REFINE_ALL,
  AVM_OPFL_REFINE_AUTO,
} avm_opfl_refine_type;





// Additional advanced compatibility types and variables
typedef aom_film_grain_table_t avm_film_grain_table_t;
#define avm_denoise_and_model_t aom_denoise_and_model_t
#define avm_denoise_and_model_alloc aom_denoise_and_model_alloc
#define avm_denoise_and_model_run(ctx, sd, grain) aom_denoise_and_model_run(ctx, sd, grain, 1)
#define avm_denoise_and_model_free aom_denoise_and_model_free
#define avm_film_grain_table_append aom_film_grain_table_append
#define avm_film_grain_table_free aom_film_grain_table_free
#define avm_remove_metadata_from_frame_buffer aom_remove_metadata_from_frame_buffer
#define avm_copy_metadata_to_frame_buffer aom_copy_metadata_to_frame_buffer
#define avm_highbd_sse(a, as, b, bs, w, h) aom_highbd_sse(CONVERT_TO_BYTEPTR(a), as, CONVERT_TO_BYTEPTR(b), bs, w, h)
#define avm_highbd_sse_c(a, as, b, bs, w, h) aom_highbd_sse_c(CONVERT_TO_BYTEPTR(a), as, CONVERT_TO_BYTEPTR(b), bs, w, h)
#define avm_highbd_subtract_block(r, c, d, ds, s, ss, p, ps, bd) aom_highbd_subtract_block(r, c, d, ds, CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(p), ps)
#define avm_get_mb_ss aom_get_mb_ss
#define avm_compute_global_motion aom_compute_global_motion
#define avm_mse_wxh_16bit_highbd aom_mse_wxh_16bit_highbd
#define avm_yv12_copy_y(src, dst) aom_yv12_copy_y(src, dst, 0)
#define avm_yv12_copy_u(src, dst) aom_yv12_copy_u(src, dst, 0)
#define avm_yv12_copy_v(src, dst) aom_yv12_copy_v(src, dst, 0)
#define avm_get_sse_plane(a, b, p) aom_get_sse_plane(a, b, p, 1)
#define avm_highbd_get_y_sse_part aom_highbd_get_y_sse_part
#define avm_highbd_get_u_sse_part aom_highbd_get_u_sse_part
#define avm_highbd_get_v_sse_part aom_highbd_get_v_sse_part
#define avm_satd aom_satd
#define avm_sum_squares_2d_i16 aom_sum_squares_2d_i16
#define avm_sum_sse_2d_i16 aom_sum_sse_2d_i16
#define avm_highbd_sse_odd_size(a, as, b, bs, w, h) aom_highbd_sse_odd_size((const uint8_t *)(a), as, (const uint8_t *)(b), bs, w, h)


static inline uint64_t avm_sum_squares_i32(const int32_t *src, uint32_t n) {
  uint64_t sum = 0;
  for (uint32_t i = 0; i < n; i++) {
    sum += (int64_t)src[i] * src[i];
  }
  return sum;
}

#define avm_highbd_8_mse8x8 aom_highbd_8_mse8x8
#define avm_highbd_8_mse16x8 aom_highbd_8_mse16x8
#define avm_highbd_8_mse8x16 aom_highbd_8_mse8x16
#define avm_highbd_8_mse16x16 aom_highbd_8_mse16x16

#define avm_highbd_10_mse8x8 aom_highbd_10_mse8x8
#define avm_highbd_10_mse16x8 aom_highbd_10_mse16x8
#define avm_highbd_10_mse8x16 aom_highbd_10_mse8x16
#define avm_highbd_10_mse16x16 aom_highbd_10_mse16x16

#define avm_highbd_12_mse8x8 aom_highbd_12_mse8x8
#define avm_highbd_12_mse16x8 aom_highbd_12_mse16x8
#define avm_highbd_12_mse8x16 aom_highbd_12_mse8x16
#define avm_highbd_12_mse16x16 aom_highbd_12_mse16x16

// Quantization and SIMD compatibility constants
#define QUANT_FP_BITS 4
#define QUANT_TABLE_BITS 3
#define AVM_QM_BITS AOM_QM_BITS

// Algorithm Error Codes mapping to native AOM
#define AVM_CODEC_OK AOM_CODEC_OK
#define AVM_CODEC_ERROR AOM_CODEC_ERROR
#define AVM_CODEC_MEM_ERROR AOM_CODEC_MEM_ERROR
#define AVM_CODEC_UNSUP_BITSTREAM AOM_CODEC_UNSUP_BITSTREAM
#define AVM_CODEC_CORRUPT_FRAME AOM_CODEC_CORRUPT_FRAME
#define AVM_CODEC_INVALID_PARAM AOM_CODEC_INVALID_PARAM
#define AVM_CODEC_INCAPABLE AOM_CODEC_INCAPABLE
#define AVM_CODEC_ABI_MISMATCH AOM_CODEC_ABI_MISMATCH
#define AVM_CODEC_UNSUP_FEATURE AOM_CODEC_UNSUP_FEATURE

// Native ABI / Capabilities mappings
#define AVM_CODEC_INTERNAL_ABI_VERSION AOM_CODEC_INTERNAL_ABI_VERSION
#define AVM_CODEC_CAP_ENCODER AOM_CODEC_CAP_ENCODER
#define AVM_CODEC_CAP_PSNR AOM_CODEC_CAP_PSNR
#define AVM_USAGE_GOOD_QUALITY AOM_USAGE_GOOD_QUALITY
#define AVM_RC_ONE_PASS AOM_RC_ONE_PASS
#define AVM_KF_AUTO AOM_KF_AUTO

// Architecture Compatibility Remaps
#define ARCH_X86_64 AOM_ARCH_X86_64
#define ARCH_X86 AOM_ARCH_X86
#define ARCH_ARM AOM_ARCH_ARM

// OBU Metadata type definitions
#define OBU_METADATA_TYPE_DECODED_FRAME_HASH 6
#define OBU_METADATA_TYPE_BANDING_HINTS 7
#define OBU_METADATA_TYPE_ICC_PROFILE 8
#define OBU_METADATA_TYPE_SCAN_TYPE 9
#define OBU_METADATA_TYPE_TEMPORAL_POINT_INFO 10
#define OBU_METADATA_TYPE_USER_DATA_UNREGISTERED 11

// Image metadata insertion mapping
#define avm_img_add_metadata aom_img_add_metadata
#define AVM_MIF_ANY_FRAME AOM_MIF_ANY_FRAME
#define AVM_MIF_KEY_FRAME AOM_MIF_KEY_FRAME
#define AVM_MIF_NON_KEY_FRAME AOM_MIF_NON_KEY_FRAME

// Image formats mapping
#define AVM_IMG_FMT_I444 AOM_IMG_FMT_I444
#define AVM_IMG_FMT_I422 AOM_IMG_FMT_I422
#define AVM_IMG_FMT_I420 AOM_IMG_FMT_I420
#define AVM_IMG_FMT_YV12 AOM_IMG_FMT_YV12
#define AVM_IMG_FMT_HIGHBITDEPTH AOM_IMG_FMT_HIGHBITDEPTH
#define AVM_IMG_FMT_YV1216 AOM_IMG_FMT_YV1216
#define AVM_IMG_FMT_I42016 AOM_IMG_FMT_I42016
#define AVM_IMG_FMT_I42216 AOM_IMG_FMT_I42216
#define AVM_IMG_FMT_I44416 AOM_IMG_FMT_I44416

// Frame classification flags
#define AVM_FRAME_IS_KEY AOM_FRAME_IS_KEY
#define AVM_FRAME_IS_INTRAONLY AOM_FRAME_IS_INTRAONLY
#define AVM_FRAME_IS_DELAYED_RANDOM_ACCESS_POINT AOM_FRAME_IS_DELAYED_RANDOM_ACCESS_POINT
#define AVM_FRAME_IS_SWITCH AOM_FRAME_IS_SWITCH
#define AVM_FRAME_IS_DROPPABLE AOM_FRAME_IS_DROPPABLE
#define AVM_FRAME_HAS_FILM_GRAIN_PARAMS 0x100

// Custom encoder configuration constants
#define CONFIG_SHARP_SETTINGS 0
#define AVM_ENC_NO_SCALE_BORDER AOM_ENC_NO_SCALE_BORDER
#define AVM_KF_DISABLED AOM_KF_DISABLED

// Timer compatibility mappings
#define avm_usec_timer aom_usec_timer
#define avm_usec_timer_start aom_usec_timer_start
#define avm_usec_timer_mark aom_usec_timer_mark
#define avm_usec_timer_elapsed aom_usec_timer_elapsed

// PSNR compatibility mappings
#define avm_calc_highbd_psnr(a, b, p, bd, in_bd, l) aom_calc_highbd_psnr(a, b, p, bd, in_bd)

// ULEB and argument matching helper redirects
#define avm_uleb_size_in_bytes aom_uleb_size_in_bytes
#define avm_arg_match_helper arg_match_helper
#define avm_arg_parse_uint_helper arg_parse_uint_helper
#define avm_arg_parse_enum_helper arg_parse_enum_helper
#define avm_arg_parse_int_helper arg_parse_int_helper
#define avm_od_ec_decode_cdf_q15 avm_od_ec_decode_cdf_q15_c
#define av2_prob_inc_tbl avm_av2_prob_inc_tbl

// Quantization & metrics redirects
#define AVMSIGN AOMSIGN
#define AVM_CR_FULL_RANGE AOM_CR_FULL_RANGE
#define avm_highbd_get_y_sse aom_highbd_get_y_sse
#define avm_yv12_realloc_with_new_border aom_yv12_realloc_with_new_border
#define avm_film_grain_table_read aom_film_grain_table_read
#define avm_highbd_quantize_b_adaptive avm_highbd_quantize_b_adaptive_c
#define avm_highbd_quantize_b_32x32_adaptive avm_highbd_quantize_b_32x32_adaptive_c
#define avm_highbd_quantize_b_64x64_adaptive avm_highbd_quantize_b_64x64_adaptive_c
#define avm_highbd_quantize_b avm_highbd_quantize_b_c

// Image allocation & cleanup redirects
#define avm_img_free aom_img_free
#define avm_img_alloc aom_img_alloc
#define avm_img_alloc_with_cb aom_img_alloc_with_cb
#define avm_img_remove_metadata aom_img_remove_metadata
#define avm_img_metadata_array_free aom_img_metadata_array_free

// RTCD redirects
#define avm_dsp_rtcd aom_dsp_rtcd
#define avm_scale_rtcd aom_scale_rtcd
void avm_img_upshift(avm_image_t *dst, const avm_image_t *src, int input_shift);

// Film grain and metrics redirects
#define av2_add_film_grain av1_add_film_grain

#include "aom_dsp/bitreader_buffer.h"
#include "aom_dsp/recenter.h"
#include "aom_ports/bitops.h"


#define avm_yv12_copy_frame aom_yv12_copy_frame
#define avm_merge_corrupted_flag aom_merge_corrupted_flag
#define avm_obu_type_to_string aom_obu_type_to_string
#define avm_copy_metadata_to_frame_buffer aom_copy_metadata_to_frame_buffer
#define avm_film_grain_table_lookup aom_film_grain_table_lookup

// Extended error parameter redirects
#define avm_read_4part_wref(r, ref_symb, cdf, nsymb_bits, ACCT_INFO_NAME) \
  avm_read_4part_wref_(r, ref_symb, cdf, nsymb_bits ACCT_INFO_ARG(ACCT_INFO_NAME))

// Extra constants
#define AVM_DEC_BORDER_IN_PIXELS AOM_DEC_BORDER_IN_PIXELS
#define AVM_NUM_SUPPORTED_BITDEPTH 3



// Custom OBU Types (isolated from standard AOM enums to prevent duplicate switch value collisions)
#define OBU_SEQUENCE_HEADER 1
#define OBU_TEMPORAL_DELIMITER 2
#define OBU_MULTI_FRAME_HEADER 3
#define OBU_CLOSED_LOOP_KEY 4
#define OBU_OPEN_LOOP_KEY 5
#define OBU_LEADING_TILE_GROUP 6
#define OBU_REGULAR_TILE_GROUP 7
#define OBU_METADATA_SHORT 8
#define OBU_METADATA_GROUP 9
#define OBU_SWITCH 10
#define OBU_LEADING_SEF 11
#define OBU_REGULAR_SEF 12
#define OBU_LEADING_TIP 13
#define OBU_REGULAR_TIP 14
#define OBU_BUFFER_REMOVAL_TIMING 15
#define OBU_LAYER_CONFIGURATION_RECORD 16
#define OBU_ATLAS_SEGMENT 17
#define OBU_OPERATING_POINT_SET 18
#define OBU_BRIDGE_FRAME 19
#define OBU_MULTI_STREAM_DECODER_OPERATION 20
#define OBU_RAS_FRAME 21
#define OBU_QUANTIZATION_MATRIX 22
#define OBU_FILM_GRAIN_MODEL 23
#define OBU_CONTENT_INTERPRETATION 24
#define OBU_PADDING 25
#define NUM_OBU_TYPES 32

// Metadata types and allocators mapping
typedef aom_metadata_t avm_metadata_t;
#define avm_img_metadata_array_alloc aom_img_metadata_array_alloc
#define avm_img_metadata_alloc aom_img_metadata_alloc
#define avm_img_metadata_free aom_img_metadata_free
#define OBU_METADATA_TYPE_AVM_RESERVED_0 OBU_METADATA_TYPE_AOM_RESERVED_0
#define NUM_OBU_METADATA_TYPES 11

// OBU validation and text helpers
static INLINE int avm_obu_type_is_valid(int obu_type) {
  switch (obu_type) {
    case OBU_SEQUENCE_HEADER:
    case OBU_TEMPORAL_DELIMITER:
    case OBU_MULTI_FRAME_HEADER:
    case OBU_CLOSED_LOOP_KEY:
    case OBU_OPEN_LOOP_KEY:
    case OBU_LEADING_TILE_GROUP:
    case OBU_REGULAR_TILE_GROUP:
    case OBU_METADATA_SHORT:
    case OBU_METADATA_GROUP:
    case OBU_SWITCH:
    case OBU_LEADING_SEF:
    case OBU_REGULAR_SEF:
    case OBU_LEADING_TIP:
    case OBU_REGULAR_TIP:
    case OBU_BUFFER_REMOVAL_TIMING:
    case OBU_LAYER_CONFIGURATION_RECORD:
    case OBU_ATLAS_SEGMENT:
    case OBU_OPERATING_POINT_SET:
    case OBU_BRIDGE_FRAME:
    case OBU_MULTI_STREAM_DECODER_OPERATION:
    case OBU_RAS_FRAME:
    case OBU_QUANTIZATION_MATRIX:
    case OBU_FILM_GRAIN_MODEL:
    case OBU_CONTENT_INTERPRETATION:
    case OBU_PADDING: return 1;
  }
  return 0;
}



#endif  // AVM_AVM_INTEGER_H_
