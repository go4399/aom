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

#include "av1/common/enums.h"
#include "av2/common/common.h"
#include "aom_scale/yv12config.h"
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
#define avm_compute_global_motion aom_compute_global_motion
#define avm_count_signed_primitive_refsubexpfin aom_count_signed_primitive_refsubexpfin
#define avm_count_primitive_quniform wb_count_primitive_quniform
typedef aom_variance_fn_t avm_variance_fn_t;

extern unsigned int aom_highbd_8_mse8x8(const uint8_t *src_ptr, int src_stride, const uint8_t *ref_ptr, int ref_stride, unsigned int *sse);
extern unsigned int aom_highbd_8_mse16x8(const uint8_t *src_ptr, int src_stride, const uint8_t *ref_ptr, int ref_stride, unsigned int *sse);
extern unsigned int aom_highbd_8_mse8x16(const uint8_t *src_ptr, int src_stride, const uint8_t *ref_ptr, int ref_stride, unsigned int *sse);
extern unsigned int aom_highbd_8_mse16x16(const uint8_t *src_ptr, int src_stride, const uint8_t *ref_ptr, int ref_stride, unsigned int *sse);

#define avm_highbd_8_mse8x8 aom_highbd_8_mse8x8
#define avm_highbd_8_mse16x8 aom_highbd_8_mse16x8
#define avm_highbd_8_mse8x16 aom_highbd_8_mse8x16
#define avm_highbd_8_mse16x16 aom_highbd_8_mse16x16

extern unsigned int aom_highbd_10_mse8x8(const uint8_t *src_ptr, int src_stride, const uint8_t *ref_ptr, int ref_stride, unsigned int *sse);
extern unsigned int aom_highbd_10_mse16x8(const uint8_t *src_ptr, int src_stride, const uint8_t *ref_ptr, int ref_stride, unsigned int *sse);
extern unsigned int aom_highbd_10_mse8x16(const uint8_t *src_ptr, int src_stride, const uint8_t *ref_ptr, int ref_stride, unsigned int *sse);
extern unsigned int aom_highbd_10_mse16x16(const uint8_t *src_ptr, int src_stride, const uint8_t *ref_ptr, int ref_stride, unsigned int *sse);

#define avm_highbd_10_mse8x8 aom_highbd_10_mse8x8
#define avm_highbd_10_mse16x8 aom_highbd_10_mse16x8
#define avm_highbd_10_mse8x16 aom_highbd_10_mse8x16
#define avm_highbd_10_mse16x16 aom_highbd_10_mse16x16

extern unsigned int aom_highbd_12_mse8x8(const uint8_t *src_ptr, int src_stride, const uint8_t *ref_ptr, int ref_stride, unsigned int *sse);
extern unsigned int aom_highbd_12_mse16x8(const uint8_t *src_ptr, int src_stride, const uint8_t *ref_ptr, int ref_stride, unsigned int *sse);
extern unsigned int aom_highbd_12_mse8x16(const uint8_t *src_ptr, int src_stride, const uint8_t *ref_ptr, int ref_stride, unsigned int *sse);
extern unsigned int aom_highbd_12_mse16x16(const uint8_t *src_ptr, int src_stride, const uint8_t *ref_ptr, int ref_stride, unsigned int *sse);

#define avm_highbd_12_mse8x8 aom_highbd_12_mse8x8
#define avm_highbd_12_mse16x8 aom_highbd_12_mse16x8
#define avm_highbd_12_mse8x16 aom_highbd_12_mse8x16
#define avm_highbd_12_mse16x16 aom_highbd_12_mse16x16

extern unsigned int aom_get_mb_ss(const int16_t *src);
#define avm_get_mb_ss aom_get_mb_ss

#include "aom_dsp/noise_model.h"
#define avm_denoise_and_model_alloc aom_denoise_and_model_alloc
static INLINE int avm_denoise_and_model_run(struct aom_denoise_and_model_t *ctx, const YV12_BUFFER_CONFIG *sd, aom_film_grain_t *film_grain) {
  return aom_denoise_and_model_run(ctx, sd, film_grain, film_grain->block_size);
}
#define avm_denoise_and_model_free aom_denoise_and_model_free

#define AOM_EFLAG_NO_UPD_ALL (AOM_EFLAG_NO_UPD_LAST | AOM_EFLAG_NO_UPD_GF | AOM_EFLAG_NO_UPD_ARF)
#define AVM_BLEND_A64_MAX_ALPHA AOM_BLEND_A64_MAX_ALPHA
#define av2_s_frame_info aom_s_frame_info

extern unsigned int aom_highbd_sad8x8_c(const uint8_t *src_ptr, int src_stride, const uint8_t *ref_ptr, int ref_stride);
extern unsigned int aom_highbd_sad16x8_c(const uint8_t *src_ptr, int src_stride, const uint8_t *ref_ptr, int ref_stride);
extern unsigned int aom_highbd_sad8x16_c(const uint8_t *src_ptr, int src_stride, const uint8_t *ref_ptr, int ref_stride);
extern unsigned int aom_highbd_sad16x16_c(const uint8_t *src_ptr, int src_stride, const uint8_t *ref_ptr, int ref_stride);
#define aom_highbd_sad8x8 aom_highbd_sad8x8_c
#define aom_highbd_sad16x8 aom_highbd_sad16x8_c
#define aom_highbd_sad8x16 aom_highbd_sad8x16_c
#define aom_highbd_sad16x16 aom_highbd_sad16x16_c

static INLINE unsigned int avm_highbd_sad8x8(const uint16_t *src, int src_stride, const uint16_t *ref, int ref_stride) {
  return aom_highbd_sad8x8((const uint8_t *)src, src_stride, (const uint8_t *)ref, ref_stride);
}
static INLINE unsigned int avm_highbd_sad16x8(const uint16_t *src, int src_stride, const uint16_t *ref, int ref_stride) {
  return aom_highbd_sad16x8((const uint8_t *)src, src_stride, (const uint8_t *)ref, ref_stride);
}
static INLINE unsigned int avm_highbd_sad8x16(const uint16_t *src, int src_stride, const uint16_t *ref, int ref_stride) {
  return aom_highbd_sad8x16((const uint8_t *)src, src_stride, (const uint8_t *)ref, ref_stride);
}
static INLINE unsigned int avm_highbd_sad16x16(const uint16_t *src, int src_stride, const uint16_t *ref, int ref_stride) {
  return aom_highbd_sad16x16((const uint8_t *)src, src_stride, (const uint8_t *)ref, ref_stride);
}
#define avm_free_frame_buffer aom_free_frame_buffer
#define avm_denoise_and_model_free aom_denoise_and_model_free
#define avm_img_metadata_array_free aom_img_metadata_array_free
#define avm_dsp_rtcd aom_dsp_rtcd
#define avm_scale_rtcd aom_scale_rtcd
#define avm_yv12_partial_coloc_copy_y aom_yv12_partial_coloc_copy_y
#define avm_yv12_partial_coloc_copy_u aom_yv12_partial_coloc_copy_u
#define avm_yv12_partial_coloc_copy_v aom_yv12_partial_coloc_copy_v

struct yv12_buffer_config;
extern void aom_yv12_partial_coloc_copy_y(const struct yv12_buffer_config *src_ybc, struct yv12_buffer_config *dst_ybc, int hstart, int hend, int vstart, int vend);
extern void aom_yv12_partial_coloc_copy_u(const struct yv12_buffer_config *src_ybc, struct yv12_buffer_config *dst_ybc, int hstart, int hend, int vstart, int vend);
extern void aom_yv12_partial_coloc_copy_v(const struct yv12_buffer_config *src_ybc, struct yv12_buffer_config *dst_ybc, int hstart, int hend, int vstart, int vend);

#define avm_free aom_free
#define avm_malloc aom_malloc
#define avm_convolve_copy aom_convolve_copy

#include "av1/common/convolve.h"

#define avm_extend_frame_borders aom_extend_frame_borders
#define avm_denoise_and_model_t aom_denoise_and_model_t

extern void aom_highbd_quantize_b_c(const tran_low_t *coeff_ptr, intptr_t n_coeffs, const int16_t *zbin_ptr, const int16_t *round_ptr, const int16_t *quant_ptr, const int16_t *quant_shift_ptr, tran_low_t *qcoeff_ptr, tran_low_t *dqcoeff_ptr, const int16_t *dequant_ptr, uint16_t *eob_ptr, const int16_t *scan, const int16_t *iscan);
#define aom_highbd_quantize_b aom_highbd_quantize_b_c

static INLINE void avm_highbd_quantize_b(
    const tran_low_t *coeff_ptr, intptr_t n_coeffs, const int32_t *zbin_ptr,
    const int32_t *round_ptr, const int32_t *quant_ptr,
    const int32_t *quant_shift_ptr, tran_low_t *qcoeff_ptr,
    tran_low_t *dqcoeff_ptr, const int32_t *dequant_ptr, uint16_t *eob_ptr,
    const int16_t *scan, const int16_t *iscan, const int log_scale) {
  int16_t zbin[2] = { (int16_t)zbin_ptr[0], (int16_t)zbin_ptr[1] };
  int16_t round[2] = { (int16_t)round_ptr[0], (int16_t)round_ptr[1] };
  int16_t quant[2] = { (int16_t)quant_ptr[0], (int16_t)quant_ptr[1] };
  int16_t quant_shift[2] = { (int16_t)quant_shift_ptr[0], (int16_t)quant_shift_ptr[1] };
  int16_t dequant[2] = { (int16_t)dequant_ptr[0], (int16_t)dequant_ptr[1] };
  (void)log_scale;

  aom_highbd_quantize_b(coeff_ptr, n_coeffs, zbin, round, quant, quant_shift,
                        qcoeff_ptr, dqcoeff_ptr, dequant, eob_ptr, scan, iscan);
}

void avm_highbd_quantize_b_c(
    const tran_low_t *coeff_ptr, intptr_t n_coeffs, const int32_t *zbin_ptr,
    const int32_t *round_ptr, const int32_t *quant_ptr,
    const int32_t *quant_shift_ptr, tran_low_t *qcoeff_ptr,
    tran_low_t *dqcoeff_ptr, const int32_t *dequant_ptr, uint16_t *eob_ptr,
    const int16_t *scan, const int16_t *iscan, const int log_scale);

extern int64_t aom_highbd_sse_c(const uint8_t *src, int src_stride, const uint8_t *ref, int ref_stride, int w, int h);
#define aom_highbd_sse aom_highbd_sse_c

static INLINE int64_t avm_highbd_sse(const uint16_t *a, int a_stride, const uint16_t *b, int b_stride, int width, int height) {
  return aom_highbd_sse((const uint8_t *)a, a_stride, (const uint8_t *)b, b_stride, width, height);
}

static INLINE int64_t avm_highbd_sse_c(const uint16_t *a, int a_stride, const uint16_t *b, int b_stride, int width, int height) {
  return aom_highbd_sse((const uint8_t *)a, a_stride, (const uint8_t *)b, b_stride, width, height);
}

extern void aom_highbd_subtract_block_c(int rows, int cols, int16_t *diff_ptr,
                                        ptrdiff_t diff_stride, const uint8_t *src_ptr,
                                        ptrdiff_t src_stride, const uint8_t *pred_ptr,
                                        ptrdiff_t pred_stride);
#define aom_highbd_subtract_block aom_highbd_subtract_block_c

static INLINE void avm_highbd_subtract_block(int rows, int cols, int16_t *diff_ptr, ptrdiff_t diff_stride, const uint16_t *src_ptr, ptrdiff_t src_stride, const uint16_t *pred_ptr, ptrdiff_t pred_stride, int bd) {
  (void)bd;
  aom_highbd_subtract_block(rows, cols, diff_ptr, diff_stride, (const uint8_t *)src_ptr, src_stride, (const uint8_t *)pred_ptr, pred_stride);
}

extern void aom_highbd_blend_a64_d16_mask_c(uint8_t *dst, uint32_t dst_stride,
                                            const CONV_BUF_TYPE *src0, uint32_t src0_stride,
                                            const CONV_BUF_TYPE *src1, uint32_t src1_stride,
                                            const uint8_t *mask, uint32_t mask_stride,
                                            int w, int h, int subw, int subh,
                                            ConvolveParams *conv_params, const int bd);

extern void aom_highbd_blend_a64_mask_c(uint8_t *dst, uint32_t dst_stride,
                                        const uint8_t *src0, uint32_t src0_stride,
                                        const uint8_t *src1, uint32_t src1_stride,
                                        const uint8_t *mask, uint32_t mask_stride,
                                        int w, int h, int subw, int subh, int bd);

static INLINE void avm_highbd_blend_a64_d16_mask(void *dst, uint32_t dst_stride,
                                                 const void *src0, uint32_t src0_stride,
                                                 const void *src1, uint32_t src1_stride,
                                                 const uint8_t *mask, uint32_t mask_stride,
                                                 int w, int h, int subw, int subh,
                                                 void *conv_params, const int bd) {
  aom_highbd_blend_a64_d16_mask_c((uint8_t *)dst, dst_stride,
                                  (const CONV_BUF_TYPE *)src0, src0_stride,
                                  (const CONV_BUF_TYPE *)src1, src1_stride,
                                  mask, mask_stride, w, h, subw, subh,
                                  (ConvolveParams *)conv_params, bd);
}

static INLINE void avm_highbd_blend_a64_mask(void *dst, uint32_t dst_stride,
                                             const void *src0, uint32_t src0_stride,
                                             const void *src1, uint32_t src1_stride,
                                             const uint8_t *mask, uint32_t mask_stride,
                                             int w, int h, int subw, int subh, int bd) {
  aom_highbd_blend_a64_mask_c((uint8_t *)dst, dst_stride,
                              (const uint8_t *)src0, src0_stride,
                              (const uint8_t *)src1, src1_stride,
                              mask, mask_stride, w, h, subw, subh, bd);
}

#define avm_reader aom_reader
#define avm_read_literal aom_read_literal
#define avm_read_symbol aom_read_symbol
#define avm_read_bit aom_read_bit
#define avm_ceil_log2 aom_ceil_log2
#define ACCT_INFO(a, ...) a

#define avm_rb_read_bit aom_rb_read_bit
#define avm_rb_read_literal aom_rb_read_literal
#define avm_rb_read_unsigned_literal aom_rb_read_unsigned_literal
#define avm_reader aom_reader
#define avm_read_bit_buffer aom_read_bit_buffer
#define avm_rb_error_handler aom_rb_error_handler

static INLINE int avm_rb_read_inv_signed_literal(struct aom_read_bit_buffer *rb, int bits) {
  const int nbits = sizeof(unsigned) * 8 - bits - 1;
  const unsigned value = (unsigned)aom_rb_read_literal(rb, bits + 1) << nbits;
  return ((int)value) >> nbits;
}

static INLINE uint16_t avm_rb_read_primitive_quniform(struct aom_read_bit_buffer *rb, uint16_t n) {
  if (n <= 1) return 0;
  const int l = get_unsigned_bits(n - 1);
  const int m = (1 << l) - n;
  const int v = aom_rb_read_literal(rb, l - 1);
  return v < m ? v : (v << 1) - m + aom_rb_read_bit(rb);
}

static INLINE uint16_t avm_rb_read_primitive_subexpfin(struct aom_read_bit_buffer *rb, uint16_t n, uint16_t k) {
  int i = 0;
  int mk = 0;
  while (1) {
    int b = (i ? k + i - 1 : k);
    int a = (1 << b);
    if (n <= mk + 3 * a) {
      return avm_rb_read_primitive_quniform(rb, n - mk) + mk;
    }
    if (!aom_rb_read_bit(rb)) {
      return aom_rb_read_literal(rb, b) + mk;
    }
    i = i + 1;
    mk += a;
  }
  return 0;
}

static INLINE uint16_t avm_rb_read_primitive_refsubexpfin(struct aom_read_bit_buffer *rb, uint16_t n, uint16_t k, uint16_t ref) {
  assert(ref < n);
  return inv_recenter_finite_nonneg(n, ref, avm_rb_read_primitive_subexpfin(rb, n, k));
}

static INLINE int avm_read_unary(aom_reader *r, int max_nbits, const char *acct) {
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

static INLINE uint16_t avm_read_primitive_quniform(aom_reader *r, uint16_t n, const char *acct) {
  (void)acct;
  if (n <= 1) return 0;
  const int l = get_unsigned_bits(n - 1);
  const int m = (1 << l) - n;
  int v = aom_read_literal(r, l - 1, NULL);
  if (v < m) return v;
  return (v << 1) - m + aom_read_bit(r, NULL);
}

#define avm_writer aom_writer
#define avm_write_bit aom_write_bit
#define avm_write_literal aom_write_literal

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

static INLINE void avm_write_4part(aom_writer *w, int symb, aom_cdf_prob *cdf, int nsymb_bits) {
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

static INLINE void avm_write_4part_wref(aom_writer *w, int ref_symb, int symb, aom_cdf_prob *cdf, int nsymb_bits) {
  const int recentered_symb = recenter_finite_nonneg(1 << nsymb_bits, ref_symb, symb);
  avm_write_4part(w, recentered_symb, cdf, nsymb_bits);
}

static INLINE int64_t avm_count_4part(int symb, const int *part_cost, int nsymb_bits, int scale_shift) {
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

static INLINE int64_t avm_count_4part_wref(int ref_symb, int symb, const int *part_cost, int nsymb_bits, int scale_shift) {
  const int recentered_symb = recenter_finite_nonneg(1 << nsymb_bits, ref_symb, symb);
  return avm_count_4part(recentered_symb, part_cost, nsymb_bits, scale_shift);
}
#endif

#define avm_merge_corrupted_flag aom_merge_corrupted_flag
#define AVM_DEC_BORDER_IN_PIXELS AOM_DEC_BORDER_IN_PIXELS
#define avm_yv12_copy_frame aom_yv12_copy_frame
#define AVM_NUM_SUPPORTED_BITDEPTH 3
#define avm_rb_read_uvlc aom_rb_read_uvlc
#define avm_rb_read_signed_primitive_refsubexpfin aom_rb_read_signed_primitive_refsubexpfin
#define avm_rb_bytes_read aom_rb_bytes_read

static INLINE int avm_realloc_frame_buffer(YV12_BUFFER_CONFIG *ybf, int width, int height,
                                           int ss_x, int ss_y, int border, int byte_alignment,
                                           aom_codec_frame_buffer_t *fb,
                                           aom_get_frame_buffer_cb_fn_t cb, void *cb_priv,
                                           bool alloc_pyramid) {
  return aom_realloc_frame_buffer(ybf, width, height, ss_x, ss_y, (ybf->bit_depth > 8),
                                  border, byte_alignment, fb, cb, cb_priv, alloc_pyramid, 0);
}

static INLINE int avm_alloc_frame_buffer(YV12_BUFFER_CONFIG *ybf, int width, int height, int ss_x, int ss_y, int border, int byte_alignment, bool alloc_pyramid) {
  return aom_alloc_frame_buffer(ybf, width, height, ss_x, ss_y, (ybf->bit_depth > 8), border, byte_alignment, alloc_pyramid, 0);
}
#define avm_remove_metadata_from_frame_buffer aom_remove_metadata_from_frame_buffer

static INLINE int avm_read_4part_(aom_reader *r, aom_cdf_prob *cdf, int nsymb_bits) {
  assert(nsymb_bits >= 3);
  int part_bits[4] = { (nsymb_bits - 3), (nsymb_bits - 3), (nsymb_bits - 2),
                       (nsymb_bits - 1) };
  int part_offs[4] = { 0, 1 << (nsymb_bits - 3), 1 << (nsymb_bits - 2),
                       1 << (nsymb_bits - 1) };
  const int part = aom_read_symbol(r, cdf, 4, NULL);
  return aom_read_literal(r, part_bits[part], NULL) + part_offs[part];
}

static INLINE int avm_read_4part_wref(aom_reader *r, int ref_symb, aom_cdf_prob *cdf, int nsymb_bits, const char *acct) {
  (void)acct;
  const int symb = avm_read_4part_(r, cdf, nsymb_bits);
  return inv_recenter_finite_nonneg(1 << nsymb_bits, ref_symb, symb);
}

static INLINE uint16_t avm_rb_read_primitive_ref_quniform(struct aom_read_bit_buffer *rb, uint16_t n, uint16_t r) {
  if (aom_rb_read_bit(rb)) {
    int v = avm_rb_read_primitive_quniform(rb, n - 1);
    v += (v >= r);
    return v;
  } else {
    return r;
  }
}

#define avm_img_metadata_array_alloc aom_img_metadata_array_alloc
#define avm_img_metadata_alloc aom_img_metadata_alloc


static INLINE int32_t avm_rb_read_svlc(struct aom_read_bit_buffer *rb) {
  const uint32_t value = aom_rb_read_uvlc(rb);
  if (value == UINT32_MAX) return INT32_MIN;
  const int32_t ceil_half = (int32_t)((value + 1) / 2);
  return (value % 2) ? ceil_half : -ceil_half;
}

static INLINE uint32_t avm_rb_read_rice_golomb(struct aom_read_bit_buffer *rb, int k) {
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

#define avm_ext_highbd_warp_affine av2_ext_highbd_warp_affine

struct ConvolveParams;
struct PadBlock;
extern void av2_ext_highbd_warp_affine(const int32_t *mat, const uint16_t *ref, int width, int height, int stride, uint16_t *pred, int p_col, int p_row, int p_width, int p_height, int p_stride, int subsampling_x, int subsampling_y, int bd, struct ConvolveParams *conv_params, int use_warp_bd_box, struct PadBlock *warp_bd_box);

#define FILTER_UNUSED -1

#define avm_highbd_convolve_copy aom_highbd_convolve_copy
#define avm_mse_wxh_16bit_highbd aom_mse_wxh_16bit_highbd

static INLINE uint64_t avm_sum_squares_i32(const int32_t *src, uint32_t n) {
  uint64_t sum = 0;
  for (uint32_t i = 0; i < n; ++i) {
    sum += (uint64_t)src[i] * src[i];
  }
  return sum;
}
#define DIVIDE_AND_ROUND_SIGNED(x, y) (((x) + (((x) < 0 ? -(y) : (y)) >> 1)) / (y))

#include "aom_mem/aom_mem.h"
#define avm_memset_int16(dest, val, count) aom_memset16((uint16_t *)(dest), (uint16_t)(val), (count))

extern void avm_highbd_subtract_block_vert(int rows, int cols, int16_t *diff_ptr, ptrdiff_t diff_stride, const uint16_t *src_ptr, ptrdiff_t src_stride, const uint16_t *pred_ptr, ptrdiff_t pred_stride, int bd);
extern void avm_highbd_subtract_block_horz(int rows, int cols, int16_t *diff_ptr, ptrdiff_t diff_stride, const uint16_t *src_ptr, ptrdiff_t src_stride, const uint16_t *pred_ptr, ptrdiff_t pred_stride, int bd);

#define avm_codec_pkt_list_decl aom_codec_pkt_list_decl
#define avm_codec_pkt_list_init aom_codec_pkt_list_init
#define avm_codec_pkt_list aom_codec_pkt_list
#define avm_codec_pkt_list_add aom_codec_pkt_list_add
#define avm_codec_pkt_list_get aom_codec_pkt_list_get
#define avm_codec_cx_pkt_t aom_codec_cx_pkt_t
#define avm_codec_cx_pkt aom_codec_cx_pkt
#define AVM_CODEC_PSNR_PKT AOM_CODEC_PSNR_PKT
#define AVM_CODEC_STATS_PKT AOM_CODEC_STATS_PKT
#define INTERP_EVAL_INVALID -1

#define avm_sum_squares_2d_i16 aom_sum_squares_2d_i16
#define avm_sum_sse_2d_i16 aom_sum_sse_2d_i16

struct yv12_buffer_config;
extern void aom_yv12_copy_y(const struct yv12_buffer_config *src_ybc, struct yv12_buffer_config *dst_ybc, int use_crop);
extern void aom_yv12_copy_u(const struct yv12_buffer_config *src_ybc, struct yv12_buffer_config *dst_ybc, int use_crop);
extern void aom_yv12_copy_v(const struct yv12_buffer_config *src_ybc, struct yv12_buffer_config *dst_ybc, int use_crop);
extern int64_t aom_get_sse_plane(const struct yv12_buffer_config *a, const struct yv12_buffer_config *b, int plane, int highbd);
extern uint64_t aom_highbd_sse_odd_size(const uint8_t *a, int a_stride, const uint8_t *b, int b_stride, int w, int h);

static INLINE uint64_t avm_highbd_sse_odd_size(const uint16_t *a, int a_stride, const uint16_t *b, int b_stride, int w, int h) {
  return aom_highbd_sse_odd_size((const uint8_t *)a, a_stride, (const uint8_t *)b, b_stride, w, h);
}

static INLINE void avm_yv12_copy_y(const struct yv12_buffer_config *src_ybc, struct yv12_buffer_config *dst_ybc) {
  aom_yv12_copy_y(src_ybc, dst_ybc, 0);
}
static INLINE void avm_yv12_copy_u(const struct yv12_buffer_config *src_ybc, struct yv12_buffer_config *dst_ybc) {
  aom_yv12_copy_u(src_ybc, dst_ybc, 0);
}
static INLINE void avm_yv12_copy_v(const struct yv12_buffer_config *src_ybc, struct yv12_buffer_config *dst_ybc) {
  aom_yv12_copy_v(src_ybc, dst_ybc, 0);
}
static INLINE int64_t avm_get_sse_plane(const struct yv12_buffer_config *a, const struct yv12_buffer_config *b, int plane) {
  return aom_get_sse_plane(a, b, plane, 1);
}

static INLINE int32_t clamp64_to_32(int64_t value) {
  return (int32_t)clamp64(value, INT_MIN, INT_MAX);
}

#define aom_reset_mmx_state aom_reset_mmx_state_dummy
static INLINE void aom_reset_mmx_state_dummy(void) {}

#define aom_highbd_ibp_dc_top_predictor_4x4_c avm_highbd_ibp_dc_top_predictor_4x4_c
#define aom_highbd_ibp_dc_top_predictor_8x8_c avm_highbd_ibp_dc_top_predictor_8x8_c
#define aom_highbd_ibp_dc_top_predictor_16x16_c avm_highbd_ibp_dc_top_predictor_16x16_c
#define aom_highbd_ibp_dc_top_predictor_32x32_c avm_highbd_ibp_dc_top_predictor_32x32_c
#define aom_highbd_ibp_dc_top_predictor_64x64_c avm_highbd_ibp_dc_top_predictor_64x64_c
#define aom_highbd_ibp_dc_top_predictor_4x8_c avm_highbd_ibp_dc_top_predictor_4x8_c
#define aom_highbd_ibp_dc_top_predictor_8x4_c avm_highbd_ibp_dc_top_predictor_8x4_c
#define aom_highbd_ibp_dc_top_predictor_8x16_c avm_highbd_ibp_dc_top_predictor_8x16_c
#define aom_highbd_ibp_dc_top_predictor_16x8_c avm_highbd_ibp_dc_top_predictor_16x8_c
#define aom_highbd_ibp_dc_top_predictor_16x32_c avm_highbd_ibp_dc_top_predictor_16x32_c
#define aom_highbd_ibp_dc_top_predictor_32x16_c avm_highbd_ibp_dc_top_predictor_32x16_c
#define aom_highbd_ibp_dc_top_predictor_32x64_c avm_highbd_ibp_dc_top_predictor_32x64_c
#define aom_highbd_ibp_dc_top_predictor_64x32_c avm_highbd_ibp_dc_top_predictor_64x32_c
#define aom_highbd_ibp_dc_top_predictor_4x16_c avm_highbd_ibp_dc_top_predictor_4x16_c
#define aom_highbd_ibp_dc_top_predictor_16x4_c avm_highbd_ibp_dc_top_predictor_16x4_c
#define aom_highbd_ibp_dc_top_predictor_8x32_c avm_highbd_ibp_dc_top_predictor_8x32_c
#define aom_highbd_ibp_dc_top_predictor_32x8_c avm_highbd_ibp_dc_top_predictor_32x8_c
#define aom_highbd_ibp_dc_top_predictor_16x64_c avm_highbd_ibp_dc_top_predictor_16x64_c
#define aom_highbd_ibp_dc_top_predictor_64x16_c avm_highbd_ibp_dc_top_predictor_64x16_c
#define aom_highbd_ibp_dc_top_predictor_4x32_c avm_highbd_ibp_dc_top_predictor_4x32_c
#define aom_highbd_ibp_dc_top_predictor_32x4_c avm_highbd_ibp_dc_top_predictor_32x4_c
#define aom_highbd_ibp_dc_top_predictor_8x64_c avm_highbd_ibp_dc_top_predictor_8x64_c
#define aom_highbd_ibp_dc_top_predictor_64x8_c avm_highbd_ibp_dc_top_predictor_64x8_c
#define aom_highbd_ibp_dc_top_predictor_4x64_c avm_highbd_ibp_dc_top_predictor_4x64_c
#define aom_highbd_ibp_dc_top_predictor_64x4_c avm_highbd_ibp_dc_top_predictor_64x4_c

#define aom_highbd_ibp_dc_left_predictor_4x4_c avm_highbd_ibp_dc_left_predictor_4x4_c
#define aom_highbd_ibp_dc_left_predictor_8x8_c avm_highbd_ibp_dc_left_predictor_8x8_c
#define aom_highbd_ibp_dc_left_predictor_16x16_c avm_highbd_ibp_dc_left_predictor_16x16_c
#define aom_highbd_ibp_dc_left_predictor_32x32_c avm_highbd_ibp_dc_left_predictor_32x32_c
#define aom_highbd_ibp_dc_left_predictor_64x64_c avm_highbd_ibp_dc_left_predictor_64x64_c
#define aom_highbd_ibp_dc_left_predictor_4x8_c avm_highbd_ibp_dc_left_predictor_4x8_c
#define aom_highbd_ibp_dc_left_predictor_8x4_c avm_highbd_ibp_dc_left_predictor_8x4_c
#define aom_highbd_ibp_dc_left_predictor_8x16_c avm_highbd_ibp_dc_left_predictor_8x16_c
#define aom_highbd_ibp_dc_left_predictor_16x8_c avm_highbd_ibp_dc_left_predictor_16x8_c
#define aom_highbd_ibp_dc_left_predictor_16x32_c avm_highbd_ibp_dc_left_predictor_16x32_c
#define aom_highbd_ibp_dc_left_predictor_32x16_c avm_highbd_ibp_dc_left_predictor_32x16_c
#define aom_highbd_ibp_dc_left_predictor_32x64_c avm_highbd_ibp_dc_left_predictor_32x64_c
#define aom_highbd_ibp_dc_left_predictor_64x32_c avm_highbd_ibp_dc_left_predictor_64x32_c
#define aom_highbd_ibp_dc_left_predictor_4x16_c avm_highbd_ibp_dc_left_predictor_4x16_c
#define aom_highbd_ibp_dc_left_predictor_16x4_c avm_highbd_ibp_dc_left_predictor_16x4_c
#define aom_highbd_ibp_dc_left_predictor_8x32_c avm_highbd_ibp_dc_left_predictor_8x32_c
#define aom_highbd_ibp_dc_left_predictor_32x8_c avm_highbd_ibp_dc_left_predictor_32x8_c
#define aom_highbd_ibp_dc_left_predictor_16x64_c avm_highbd_ibp_dc_left_predictor_16x64_c
#define aom_highbd_ibp_dc_left_predictor_64x16_c avm_highbd_ibp_dc_left_predictor_64x16_c
#define aom_highbd_ibp_dc_left_predictor_4x32_c avm_highbd_ibp_dc_left_predictor_4x32_c
#define aom_highbd_ibp_dc_left_predictor_32x4_c avm_highbd_ibp_dc_left_predictor_32x4_c
#define aom_highbd_ibp_dc_left_predictor_8x64_c avm_highbd_ibp_dc_left_predictor_8x64_c
#define aom_highbd_ibp_dc_left_predictor_64x8_c avm_highbd_ibp_dc_left_predictor_64x8_c
#define aom_highbd_ibp_dc_left_predictor_4x64_c avm_highbd_ibp_dc_left_predictor_4x64_c
#define aom_highbd_ibp_dc_left_predictor_64x4_c avm_highbd_ibp_dc_left_predictor_64x4_c

#define aom_highbd_ibp_dc_predictor_4x4_c avm_highbd_ibp_dc_predictor_4x4_c
#define aom_highbd_ibp_dc_predictor_8x8_c avm_highbd_ibp_dc_predictor_8x8_c
#define aom_highbd_ibp_dc_predictor_16x16_c avm_highbd_ibp_dc_predictor_16x16_c
#define aom_highbd_ibp_dc_predictor_32x32_c avm_highbd_ibp_dc_predictor_32x32_c
#define aom_highbd_ibp_dc_predictor_64x64_c avm_highbd_ibp_dc_predictor_64x64_c
#define aom_highbd_ibp_dc_predictor_4x8_c avm_highbd_ibp_dc_predictor_4x8_c
#define aom_highbd_ibp_dc_predictor_8x4_c avm_highbd_ibp_dc_predictor_8x4_c
#define aom_highbd_ibp_dc_predictor_8x16_c avm_highbd_ibp_dc_predictor_8x16_c
#define aom_highbd_ibp_dc_predictor_16x8_c avm_highbd_ibp_dc_predictor_16x8_c
#define aom_highbd_ibp_dc_predictor_16x32_c avm_highbd_ibp_dc_predictor_16x32_c
#define aom_highbd_ibp_dc_predictor_32x16_c avm_highbd_ibp_dc_predictor_32x16_c
#define aom_highbd_ibp_dc_predictor_32x64_c avm_highbd_ibp_dc_predictor_32x64_c
#define aom_highbd_ibp_dc_predictor_64x32_c avm_highbd_ibp_dc_predictor_64x32_c
#define aom_highbd_ibp_dc_predictor_4x16_c avm_highbd_ibp_dc_predictor_4x16_c
#define aom_highbd_ibp_dc_predictor_16x4_c avm_highbd_ibp_dc_predictor_16x4_c
#define aom_highbd_ibp_dc_predictor_8x32_c avm_highbd_ibp_dc_predictor_8x32_c
#define aom_highbd_ibp_dc_predictor_32x8_c avm_highbd_ibp_dc_predictor_32x8_c
#define aom_highbd_ibp_dc_predictor_16x64_c avm_highbd_ibp_dc_predictor_16x64_c
#define aom_highbd_ibp_dc_predictor_64x16_c avm_highbd_ibp_dc_predictor_64x16_c
#define aom_highbd_ibp_dc_predictor_4x32_c avm_highbd_ibp_dc_predictor_4x32_c
#define aom_highbd_ibp_dc_predictor_32x4_c avm_highbd_ibp_dc_predictor_32x4_c
#define aom_highbd_ibp_dc_predictor_8x64_c avm_highbd_ibp_dc_predictor_8x64_c
#define aom_highbd_ibp_dc_predictor_64x8_c avm_highbd_ibp_dc_predictor_64x8_c
#define aom_highbd_ibp_dc_predictor_4x64_c avm_highbd_ibp_dc_predictor_4x64_c
#define aom_highbd_ibp_dc_predictor_64x4_c avm_highbd_ibp_dc_predictor_64x4_c

#define aom_highbd_dc_128_predictor_4x32_c avm_highbd_dc_128_predictor_4x32_c
#define aom_highbd_dc_128_predictor_32x4_c avm_highbd_dc_128_predictor_32x4_c
#define aom_highbd_dc_128_predictor_8x64_c avm_highbd_dc_128_predictor_8x64_c
#define aom_highbd_dc_128_predictor_64x8_c avm_highbd_dc_128_predictor_64x8_c
#define aom_highbd_dc_128_predictor_4x64_c avm_highbd_dc_128_predictor_4x64_c
#define aom_highbd_dc_128_predictor_64x4_c avm_highbd_dc_128_predictor_64x4_c

#define aom_highbd_v_predictor_4x32_c avm_highbd_v_predictor_4x32_c
#define aom_highbd_v_predictor_32x4_c avm_highbd_v_predictor_32x4_c
#define aom_highbd_v_predictor_8x64_c avm_highbd_v_predictor_8x64_c
#define aom_highbd_v_predictor_64x8_c avm_highbd_v_predictor_64x8_c
#define aom_highbd_v_predictor_4x64_c avm_highbd_v_predictor_4x64_c
#define aom_highbd_v_predictor_64x4_c avm_highbd_v_predictor_64x4_c

#define aom_highbd_h_predictor_4x32_c avm_highbd_h_predictor_4x32_c
#define aom_highbd_h_predictor_32x4_c avm_highbd_h_predictor_32x4_c
#define aom_highbd_h_predictor_8x64_c avm_highbd_h_predictor_8x64_c
#define aom_highbd_h_predictor_64x8_c avm_highbd_h_predictor_64x8_c
#define aom_highbd_h_predictor_4x64_c avm_highbd_h_predictor_4x64_c
#define aom_highbd_h_predictor_64x4_c avm_highbd_h_predictor_64x4_c

#define aom_highbd_smooth_predictor_4x32_c avm_highbd_smooth_predictor_4x32_c
#define aom_highbd_smooth_predictor_32x4_c avm_highbd_smooth_predictor_32x4_c
#define aom_highbd_smooth_predictor_8x64_c avm_highbd_smooth_predictor_8x64_c
#define aom_highbd_smooth_predictor_64x8_c avm_highbd_smooth_predictor_64x8_c
#define aom_highbd_smooth_predictor_4x64_c avm_highbd_smooth_predictor_4x64_c
#define aom_highbd_smooth_predictor_64x4_c avm_highbd_smooth_predictor_64x4_c

#define aom_highbd_smooth_v_predictor_4x32_c avm_highbd_smooth_v_predictor_4x32_c
#define aom_highbd_smooth_v_predictor_32x4_c avm_highbd_smooth_v_predictor_32x4_c
#define aom_highbd_smooth_v_predictor_8x64_c avm_highbd_smooth_v_predictor_8x64_c
#define aom_highbd_smooth_v_predictor_64x8_c avm_highbd_smooth_v_predictor_64x8_c
#define aom_highbd_smooth_v_predictor_4x64_c avm_highbd_smooth_v_predictor_4x64_c
#define aom_highbd_smooth_v_predictor_64x4_c avm_highbd_smooth_v_predictor_64x4_c

#define aom_highbd_smooth_h_predictor_4x32_c avm_highbd_smooth_h_predictor_4x32_c
#define aom_highbd_smooth_h_predictor_32x4_c avm_highbd_smooth_h_predictor_32x4_c
#define aom_highbd_smooth_h_predictor_8x64_c avm_highbd_smooth_h_predictor_8x64_c
#define aom_highbd_smooth_h_predictor_64x8_c avm_highbd_smooth_h_predictor_64x8_c
#define aom_highbd_smooth_h_predictor_4x64_c avm_highbd_smooth_h_predictor_4x64_c
#define aom_highbd_smooth_h_predictor_64x4_c avm_highbd_smooth_h_predictor_64x4_c

#define aom_highbd_paeth_predictor_4x32_c avm_highbd_paeth_predictor_4x32_c
#define aom_highbd_paeth_predictor_32x4_c avm_highbd_paeth_predictor_32x4_c
#define aom_highbd_paeth_predictor_8x64_c avm_highbd_paeth_predictor_8x64_c
#define aom_highbd_paeth_predictor_64x8_c avm_highbd_paeth_predictor_64x8_c
#define aom_highbd_paeth_predictor_4x64_c avm_highbd_paeth_predictor_4x64_c
#define aom_highbd_paeth_predictor_64x4_c avm_highbd_paeth_predictor_64x4_c

#define aom_highbd_dc_top_predictor_4x32_c avm_highbd_dc_top_predictor_4x32_c
#define aom_highbd_dc_top_predictor_32x4_c avm_highbd_dc_top_predictor_32x4_c
#define aom_highbd_dc_top_predictor_8x64_c avm_highbd_dc_top_predictor_8x64_c
#define aom_highbd_dc_top_predictor_64x8_c avm_highbd_dc_top_predictor_64x8_c
#define aom_highbd_dc_top_predictor_4x64_c avm_highbd_dc_top_predictor_4x64_c
#define aom_highbd_dc_top_predictor_64x4_c avm_highbd_dc_top_predictor_64x4_c

#define aom_highbd_dc_left_predictor_4x32_c avm_highbd_dc_left_predictor_4x32_c
#define aom_highbd_dc_left_predictor_32x4_c avm_highbd_dc_left_predictor_32x4_c
#define aom_highbd_dc_left_predictor_8x64_c avm_highbd_dc_left_predictor_8x64_c
#define aom_highbd_dc_left_predictor_64x8_c avm_highbd_dc_left_predictor_64x8_c
#define aom_highbd_dc_left_predictor_4x64_c avm_highbd_dc_left_predictor_4x64_c
#define aom_highbd_dc_left_predictor_64x4_c avm_highbd_dc_left_predictor_64x4_c

#define aom_highbd_dc_predictor_4x32_c avm_highbd_dc_predictor_4x32_c
#define aom_highbd_dc_predictor_32x4_c avm_highbd_dc_predictor_32x4_c
#define aom_highbd_dc_predictor_8x64_c avm_highbd_dc_predictor_8x64_c
#define aom_highbd_dc_predictor_64x8_c avm_highbd_dc_predictor_64x8_c
#define aom_highbd_dc_predictor_4x64_c avm_highbd_dc_predictor_4x64_c
#define aom_highbd_dc_predictor_64x4_c avm_highbd_dc_predictor_64x4_c

#define av2_add_film_grain av1_add_film_grain

#endif  // AOM_AV2_COMMON_ENUMS_H_
