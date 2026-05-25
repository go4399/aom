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

#ifndef AVM_AV2_COMMON_X86_AV2_CONVOLVE_AVX2_H_
#define AVM_AVM_DSP_X86_CONVOLVE_AVX2_H_

#include "config/aom_config.h"
#include "aom_dsp/x86/synonyms.h"

// filters for 16
DECLARE_ALIGNED(32, static const uint8_t, filt_global_avx2[]) = {
  0,  1,  1,  2,  2, 3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  0,  1,  1,
  2,  2,  3,  3,  4, 4,  5,  5,  6,  6,  7,  7,  8,  2,  3,  3,  4,  4,  5,
  5,  6,  6,  7,  7, 8,  8,  9,  9,  10, 2,  3,  3,  4,  4,  5,  5,  6,  6,
  7,  7,  8,  8,  9, 9,  10, 4,  5,  5,  6,  6,  7,  7,  8,  8,  9,  9,  10,
  10, 11, 11, 12, 4, 5,  5,  6,  6,  7,  7,  8,  8,  9,  9,  10, 10, 11, 11,
  12, 6,  7,  7,  8, 8,  9,  9,  10, 10, 11, 11, 12, 12, 13, 13, 14, 6,  7,
  7,  8,  8,  9,  9, 10, 10, 11, 11, 12, 12, 13, 13, 14
};

DECLARE_ALIGNED(32, static const uint8_t, filt_d4_global_avx2[]) = {
  0, 1, 2, 3,  1, 2, 3, 4, 2, 3, 4, 5, 3, 4, 5, 6, 0, 1, 2, 3,  1, 2,
  3, 4, 2, 3,  4, 5, 3, 4, 5, 6, 4, 5, 6, 7, 5, 6, 7, 8, 6, 7,  8, 9,
  7, 8, 9, 10, 4, 5, 6, 7, 5, 6, 7, 8, 6, 7, 8, 9, 7, 8, 9, 10,
};

DECLARE_ALIGNED(32, static const uint8_t, filt4_d4_global_avx2[]) = {
  2, 3, 4, 5, 3, 4, 5, 6, 4, 5, 6, 7, 5, 6, 7, 8,
  2, 3, 4, 5, 3, 4, 5, 6, 4, 5, 6, 7, 5, 6, 7, 8,
};

DECLARE_ALIGNED(32, static const uint8_t, filt_center_global_avx2[32]) = {
  3, 255, 4, 255, 5, 255, 6, 255, 7, 255, 8, 255, 9, 255, 10, 255,
  3, 255, 4, 255, 5, 255, 6, 255, 7, 255, 8, 255, 9, 255, 10, 255
};

DECLARE_ALIGNED(32, static const uint8_t,
                filt1_global_avx2[32]) = { 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5,
                                           6, 6, 7, 7, 8, 0, 1, 1, 2, 2, 3,
                                           3, 4, 4, 5, 5, 6, 6, 7, 7, 8 };

DECLARE_ALIGNED(32, static const uint8_t,
                filt2_global_avx2[32]) = { 2, 3, 3, 4, 4,  5, 5, 6, 6, 7, 7,
                                           8, 8, 9, 9, 10, 2, 3, 3, 4, 4, 5,
                                           5, 6, 6, 7, 7,  8, 8, 9, 9, 10 };

DECLARE_ALIGNED(32, static const uint8_t, filt3_global_avx2[32]) = {
  4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12,
  4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12
};

DECLARE_ALIGNED(32, static const uint8_t, filt4_global_avx2[32]) = {
  6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14,
  6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14
};

/* --- HELPER FUNCTIONS FIRST --- */

static inline void prepare_coeffs_lowbd(
    const InterpFilterParams *const filter_params, const int subpel_q4,
    __m256i *const coeffs /* [4] */) {
  const int16_t *const filter = av1_get_interp_filter_subpel_kernel(
      filter_params, subpel_q4 & SUBPEL_MASK);
  const __m128i coeffs_8 = _mm_loadu_si128((__m128i *)filter);
  const __m256i filter_coeffs = _mm256_broadcastsi128_si256(coeffs_8);
  assert(_mm_test_all_zeros(_mm_and_si128(coeffs_8, _mm_set1_epi16(1)),
                            _mm_set1_epi16((short)0xffff)));
  const __m256i coeffs_1 = _mm256_srai_epi16(filter_coeffs, 1);
  coeffs[0] = _mm256_shuffle_epi8(coeffs_1, _mm256_set1_epi16(0x0200u));
  coeffs[1] = _mm256_shuffle_epi8(coeffs_1, _mm256_set1_epi16(0x0604u));
  coeffs[2] = _mm256_shuffle_epi8(coeffs_1, _mm256_set1_epi16(0x0a08u));
  coeffs[3] = _mm256_shuffle_epi8(coeffs_1, _mm256_set1_epi16(0x0e0cu));
}

static inline void prepare_coeffs_6t_lowbd(
    const InterpFilterParams *const filter_params, const int subpel_q4,
    __m256i *const coeffs /* [4] */) {
  const int16_t *const filter = av1_get_interp_filter_subpel_kernel(
      filter_params, subpel_q4 & SUBPEL_MASK);
  const __m128i coeffs_8 = _mm_loadu_si128((__m128i *)filter);
  const __m256i filter_coeffs = _mm256_broadcastsi128_si256(coeffs_8);
  assert(_mm_test_all_zeros(_mm_and_si128(coeffs_8, _mm_set1_epi16(1)),
                            _mm_set1_epi16((int16_t)0xffff)));
  const __m256i coeffs_1 = _mm256_srai_epi16(filter_coeffs, 1);
  coeffs[0] = _mm256_shuffle_epi8(coeffs_1, _mm256_set1_epi16(0x0402u));
  coeffs[1] = _mm256_shuffle_epi8(coeffs_1, _mm256_set1_epi16(0x0806u));
  coeffs[2] = _mm256_shuffle_epi8(coeffs_1, _mm256_set1_epi16(0x0c0au));
}

static inline void prepare_coeffs_6t(
    const InterpFilterParams *const filter_params, const int subpel_q4,
    __m256i *const coeffs /* [4] */) {
  const int16_t *filter = av1_get_interp_filter_subpel_kernel(
      filter_params, subpel_q4 & SUBPEL_MASK);
  const __m128i coeff_8 = _mm_loadu_si128((__m128i *)(filter + 1));
  const __m256i coeff = _mm256_broadcastsi128_si256(coeff_8);
  coeffs[0] = _mm256_shuffle_epi32(coeff, 0x00);
  coeffs[1] = _mm256_shuffle_epi32(coeff, 0x55);
  coeffs[2] = _mm256_shuffle_epi32(coeff, 0xaa);
}

static inline void prepare_coeffs(const InterpFilterParams *const filter_params,
                                  const int subpel_q4,
                                  __m256i *const coeffs /* [4] */) {
  const int16_t *filter = av1_get_interp_filter_subpel_kernel(
      filter_params, subpel_q4 & SUBPEL_MASK);
  const __m128i coeff_8 = _mm_loadu_si128((__m128i *)filter);
  const __m256i coeff = _mm256_broadcastsi128_si256(coeff_8);
  coeffs[0] = _mm256_shuffle_epi32(coeff, 0x00);
  coeffs[1] = _mm256_shuffle_epi32(coeff, 0x55);
  coeffs[2] = _mm256_shuffle_epi32(coeff, 0xaa);
  coeffs[3] = _mm256_shuffle_epi32(coeff, 0xff);
}

static inline void prepare_coeffs_4t(
    const InterpFilterParams *const filter_params, const int subpel_q4,
    __m256i *const coeffs /* [4] */) {
  const int16_t *filter = av1_get_interp_filter_subpel_kernel(
      filter_params, subpel_q4 & SUBPEL_MASK);
  const __m128i coeff_8 = _mm_loadu_si128((__m128i *)filter);
  const __m256i coeff = _mm256_broadcastsi128_si256(coeff_8);
  coeffs[0] = _mm256_shuffle_epi32(coeff, 0x55);
  coeffs[1] = _mm256_shuffle_epi32(coeff, 0xaa);
}

static inline void prepare_coeffs_12taps(
    const InterpFilterParams *const filter_params, const int subpel_q4,
    __m256i *const coeffs /* [4] */) {
  const int16_t *filter = av1_get_interp_filter_subpel_kernel(
      filter_params, subpel_q4 & SUBPEL_MASK);
  __m128i coeff_8 = _mm_loadu_si128((__m128i *)filter);
  __m256i coeff = _mm256_broadcastsi128_si256(coeff_8);
  coeffs[0] = _mm256_shuffle_epi32(coeff, 0x00);
  coeffs[1] = _mm256_shuffle_epi32(coeff, 0x55);
  coeffs[2] = _mm256_shuffle_epi32(coeff, 0xaa);
  coeffs[3] = _mm256_shuffle_epi32(coeff, 0xff);
  coeff_8 = _mm_loadl_epi64((__m128i *)(filter + 8));
  coeff = _mm256_broadcastq_epi64(coeff_8);
  coeffs[4] = _mm256_shuffle_epi32(coeff, 0x00);
  coeffs[5] = _mm256_shuffle_epi32(coeff, 0x55);
}

static inline __m256i convolve_lowbd(const __m256i *const s,
                                     const __m256i *const coeffs) {
  const __m256i res_01 = _mm256_maddubs_epi16(s[0], coeffs[0]);
  const __m256i res_23 = _mm256_maddubs_epi16(s[1], coeffs[1]);
  const __m256i res_45 = _mm256_maddubs_epi16(s[2], coeffs[2]);
  const __m256i res_67 = _mm256_maddubs_epi16(s[3], coeffs[3]);
  return _mm256_add_epi16(_mm256_add_epi16(res_01, res_45),
                          _mm256_add_epi16(res_23, res_67));
}

static inline __m256i convolve_lowbd_6tap(const __m256i *const s,
                                          const __m256i *const coeffs) {
  const __m256i res_01 = _mm256_maddubs_epi16(s[0], coeffs[0]);
  const __m256i res_23 = _mm256_maddubs_epi16(s[1], coeffs[1]);
  const __m256i res_45 = _mm256_maddubs_epi16(s[2], coeffs[2]);
  return _mm256_add_epi16(_mm256_add_epi16(res_01, res_45), res_23);
}

static inline __m256i convolve_lowbd_4tap(const __m256i *const s,
                                          const __m256i *const coeffs) {
  const __m256i res_23 = _mm256_maddubs_epi16(s[0], coeffs[0]);
  const __m256i res_45 = _mm256_maddubs_epi16(s[1], coeffs[1]);
  return _mm256_add_epi16(res_45, res_23);
}

static inline __m256i convolve_6tap(const __m256i *const s,
                                    const __m256i *const coeffs) {
  const __m256i res_0 = _mm256_madd_epi16(s[0], coeffs[0]);
  const __m256i res_1 = _mm256_madd_epi16(s[1], coeffs[1]);
  const __m256i res_2 = _mm256_madd_epi16(s[2], coeffs[2]);
  return _mm256_add_epi32(_mm256_add_epi32(res_0, res_1), res_2);
}

static inline __m256i convolve(const __m256i *const s,
                               const __m256i *const coeffs) {
  const __m256i res_0 = _mm256_madd_epi16(s[0], coeffs[0]);
  const __m256i res_1 = _mm256_madd_epi16(s[1], coeffs[1]);
  const __m256i res_2 = _mm256_madd_epi16(s[2], coeffs[2]);
  const __m256i res_3 = _mm256_madd_epi16(s[3], coeffs[3]);
  return _mm256_add_epi32(_mm256_add_epi32(res_0, res_1),
                          _mm256_add_epi32(res_2, res_3));
}

static inline __m256i convolve_4tap(const __m256i *const s,
                                    const __m256i *const coeffs) {
  const __m256i res_1 = _mm256_madd_epi16(s[0], coeffs[0]);
  const __m256i res_2 = _mm256_madd_epi16(s[1], coeffs[1]);
  return _mm256_add_epi32(res_1, res_2);
}

static inline __m256i convolve_lowbd_x(const __m256i data,
                                       const __m256i *const coeffs,
                                       const __m256i *const filt) {
  __m256i s[4];
  s[0] = _mm256_shuffle_epi8(data, filt[0]);
  s[1] = _mm256_shuffle_epi8(data, filt[1]);
  s[2] = _mm256_shuffle_epi8(data, filt[2]);
  s[3] = _mm256_shuffle_epi8(data, filt[3]);
  return convolve_lowbd(s, coeffs);
}

static inline __m256i convolve_lowbd_x_6tap(const __m256i data,
                                            const __m256i *const coeffs,
                                            const __m256i *const filt) {
  __m256i s[4];
  s[0] = _mm256_shuffle_epi8(data, filt[0]);
  s[1] = _mm256_shuffle_epi8(data, filt[1]);
  s[2] = _mm256_shuffle_epi8(data, filt[2]);
  return convolve_lowbd_6tap(s, coeffs);
}

static inline __m256i convolve_lowbd_x_4tap(const __m256i data,
                                            const __m256i *const coeffs,
                                            const __m256i *const filt) {
  __m256i s[2];
  s[0] = _mm256_shuffle_epi8(data, filt[0]);
  s[1] = _mm256_shuffle_epi8(data, filt[1]);
  return convolve_lowbd_4tap(s, coeffs);
}

static inline void add_store_aligned_256(CONV_BUF_TYPE *const dst,
                                         const __m256i *const res,
                                         const int do_average) {
  __m256i d;
  if (do_average) {
    d = _mm256_loadu_si256((__m256i *)dst);
    d = _mm256_add_epi32(d, *res);
    d = _mm256_srai_epi32(d, 1);
  } else {
    d = *res;
  }
  _mm256_store_si256((__m256i *)dst, d);
}

static inline __m256i comp_avg(const __m256i *const data_ref_0,
                               const __m256i *const res_unsigned,
                               const __m256i *const wt,
                               const int use_wtd_comp_avg) {
  __m256i res;
  if (use_wtd_comp_avg) {
    const __m256i data_lo = _mm256_unpacklo_epi16(*data_ref_0, *res_unsigned);
    const __m256i data_hi = _mm256_unpackhi_epi16(*data_ref_0, *res_unsigned);
    const __m256i wt_res_lo = _mm256_madd_epi16(data_lo, *wt);
    const __m256i wt_res_hi = _mm256_madd_epi16(data_hi, *wt);
    const __m256i res_lo = _mm256_srai_epi32(wt_res_lo, DIST_PRECISION_BITS);
    const __m256i res_hi = _mm256_srai_epi32(wt_res_hi, DIST_PRECISION_BITS);
    res = _mm256_packs_epi32(res_lo, res_hi);
  } else {
    const __m256i wt_res = _mm256_add_epi16(*data_ref_0, *res_unsigned);
    res = _mm256_srai_epi16(wt_res, 1);
  }
  return res;
}

static inline __m256i convolve_rounding(const __m256i *const res_unsigned,
                                        const __m256i *const offset_const,
                                        const __m256i *const round_const,
                                        const int round_shift) {
  const __m256i res_signed = _mm256_sub_epi16(*res_unsigned, *offset_const);
  const __m256i res_round = _mm256_srai_epi16(
      _mm256_add_epi16(res_signed, *round_const), round_shift);
  return res_round;
}

static inline __m256i highbd_comp_avg(const __m256i *const data_ref_0,
                                      const __m256i *const res_unsigned,
                                      const __m256i *const wt0,
                                      const __m256i *const wt1,
                                      const int use_wtd_comp_avg) {
  __m256i res;
  if (use_wtd_comp_avg) {
    const __m256i wt0_res = _mm256_mullo_epi32(*data_ref_0, *wt0);
    const __m256i wt1_res = _mm256_mullo_epi32(*res_unsigned, *wt1);
    const __m256i wt_res = _mm256_add_epi32(wt0_res, wt1_res);
    res = _mm256_srai_epi32(wt_res, DIST_PRECISION_BITS);
  } else {
    const __m256i wt_res = _mm256_add_epi32(*data_ref_0, *res_unsigned);
    res = _mm256_srai_epi32(wt_res, 1);
  }
  return res;
}

static inline __m256i highbd_convolve_rounding(
    const __m256i *const res_unsigned, const __m256i *const offset_const,
    const __m256i *const round_const, const int round_shift) {
  const __m256i res_signed = _mm256_sub_epi32(*res_unsigned, *offset_const);
  const __m256i res_round = _mm256_srai_epi32(
      _mm256_add_epi32(res_signed, *round_const), round_shift);
  return res_round;
}

/* --- NOW MACROS --- */

#define CONVOLVE_HORIZ_FILTER_8TAP                                           \
  for (i = 0; i < im_h; i += 2) {                                            \
    const __m256i row0 =                                                     \
        _mm256_loadu_si256((__m256i *)&src_ptr[i * src_stride + j]);         \
    __m256i row1 = _mm256_set1_epi16(0);                                     \
    if (i + 1 < im_h)                                                        \
      row1 =                                                                 \
          _mm256_loadu_si256((__m256i *)&src_ptr[(i + 1) * src_stride + j]); \
                                                                             \
    const __m256i r0 = _mm256_permute2x128_si256(row0, row1, 0x20);          \
    const __m256i r1 = _mm256_permute2x128_si256(row0, row1, 0x31);          \
                                                                             \
    s[0] = r0;                                                               \
    s[1] = _mm256_alignr_epi8(r1, r0, 4);                                    \
    s[2] = _mm256_alignr_epi8(r1, r0, 8);                                    \
    s[3] = _mm256_alignr_epi8(r1, r0, 12);                                   \
                                                                             \
    __m256i res_even = convolve(s, coeffs_x);                                \
    res_even = _mm256_sra_epi32(_mm256_add_epi32(res_even, round_const_x),   \
                                round_shift_x);                              \
                                                                             \
    s[0] = _mm256_alignr_epi8(r1, r0, 2);                                    \
    s[1] = _mm256_alignr_epi8(r1, r0, 6);                                    \
    s[2] = _mm256_alignr_epi8(r1, r0, 10);                                   \
    s[3] = _mm256_alignr_epi8(r1, r0, 14);                                   \
                                                                             \
    __m256i res_odd = convolve(s, coeffs_x);                                 \
    res_odd = _mm256_sra_epi32(_mm256_add_epi32(res_odd, round_const_x),     \
                               round_shift_x);                               \
                                                                             \
    const __m256i res_even1 = _mm256_packs_epi32(res_even, res_even);        \
    const __m256i res_odd1 = _mm256_packs_epi32(res_odd, res_odd);           \
    const __m256i res = _mm256_unpacklo_epi16(res_even1, res_odd1);          \
                                                                             \
    _mm256_store_si256((__m256i *)&im_block[i * im_stride], res);            \
  }

#define CONVOLVE_HORIZ_FILTER_6TAP                                           \
  for (i = 0; i < im_h; i += 2) {                                            \
    const __m256i row0 =                                                     \
        _mm256_loadu_si256((__m256i *)&src_ptr[i * src_stride + j]);         \
    __m256i row1 = _mm256_set1_epi16(0);                                     \
    if (i + 1 < im_h)                                                        \
      row1 =                                                                 \
          _mm256_loadu_si256((__m256i *)&src_ptr[(i + 1) * src_stride + j]); \
                                                                             \
    const __m256i r0 = _mm256_permute2x128_si256(row0, row1, 0x20);          \
    const __m256i r1 = _mm256_permute2x128_si256(row0, row1, 0x31);          \
                                                                             \
    s[0] = r0;                                                               \
    s[1] = _mm256_alignr_epi8(r1, r0, 4);                                    \
    s[2] = _mm256_alignr_epi8(r1, r0, 8);                                    \
                                                                             \
    __m256i res_even = convolve_6tap(s, coeffs_x);                           \
    res_even = _mm256_sra_epi32(_mm256_add_epi32(res_even, round_const_x),   \
                                round_shift_x);                              \
                                                                             \
    s[0] = _mm256_alignr_epi8(r1, r0, 2);                                    \
    s[1] = _mm256_alignr_epi8(r1, r0, 6);                                    \
    s[2] = _mm256_alignr_epi8(r1, r0, 10);                                   \
                                                                             \
    __m256i res_odd = convolve_6tap(s, coeffs_x);                            \
    res_odd = _mm256_sra_epi32(_mm256_add_epi32(res_odd, round_const_x),     \
                               round_shift_x);                               \
                                                                             \
    const __m256i res_even1 = _mm256_packs_epi32(res_even, res_even);        \
    const __m256i res_odd1 = _mm256_packs_epi32(res_odd, res_odd);           \
    const __m256i res = _mm256_unpacklo_epi16(res_even1, res_odd1);          \
                                                                             \
    _mm256_store_si256((__m256i *)&im_block[i * im_stride], res);            \
  }

#define CONVOLVE_HORIZ_FILTER_4TAP                                           \
  for (i = 0; i < im_h; i += 2) {                                            \
    const __m256i row0 =                                                     \
        _mm256_loadu_si256((__m256i *)&src_ptr[i * src_stride + j]);         \
    __m256i row1 = _mm256_set1_epi16(0);                                     \
    if (i + 1 < im_h)                                                        \
      row1 =                                                                 \
          _mm256_loadu_si256((__m256i *)&src_ptr[(i + 1) * src_stride + j]); \
                                                                             \
    const __m256i r0 = _mm256_permute2x128_si256(row0, row1, 0x20);          \
    const __m256i r1 = _mm256_permute2x128_si256(row0, row1, 0x31);          \
                                                                             \
    s[0] = r0;                                                               \
    s[1] = _mm256_alignr_epi8(r1, r0, 4);                                    \
                                                                             \
    __m256i res_even = convolve_4tap(s, coeffs_x);                           \
    res_even = _mm256_sra_epi32(_mm256_add_epi32(res_even, round_const_x),   \
                                round_shift_x);                              \
                                                                             \
    s[0] = _mm256_alignr_epi8(r1, r0, 2);                                    \
    s[1] = _mm256_alignr_epi8(r1, r0, 6);                                    \
                                                                             \
    __m256i res_odd = convolve_4tap(s, coeffs_x);                            \
    res_odd = _mm256_sra_epi32(_mm256_add_epi32(res_odd, round_const_x),     \
                               round_shift_x);                               \
                                                                             \
    const __m256i res_even1 = _mm256_packs_epi32(res_even, res_even);        \
    const __m256i res_odd1 = _mm256_packs_epi32(res_odd, res_odd);           \
    const __m256i res = _mm256_unpacklo_epi16(res_even1, res_odd1);          \
                                                                             \
    _mm256_store_si256((__m256i *)&im_block[i * im_stride], res);            \
  }

#define CONVOLVE_HORIZ_FILTER_2TAP                                           \
  for (i = 0; i < im_h; i += 2) {                                            \
    const __m256i row0 =                                                     \
        _mm256_loadu_si256((__m256i *)&src_ptr[i * src_stride + j]);         \
    __m256i row1 = _mm256_set1_epi16(0);                                     \
    if (i + 1 < im_h)                                                        \
      row1 =                                                                 \
          _mm256_loadu_si256((__m256i *)&src_ptr[(i + 1) * src_stride + j]); \
                                                                             \
    const __m256i r0 = _mm256_permute2x128_si256(row0, row1, 0x20);          \
    const __m256i r1 = _mm256_permute2x128_si256(row0, row1, 0x31);          \
                                                                             \
    s[0] = r0;                                                               \
    s[1] = _mm256_alignr_epi8(r1, r0, 2);                                    \
                                                                             \
    __m256i res_0 = _mm256_madd_epi16(s[0], coeffs_x[0]);                    \
    __m256i res_1 = _mm256_madd_epi16(s[1], coeffs_x[0]);                    \
                                                                             \
    res_0 = _mm256_sra_epi32(_mm256_add_epi32(res_0, round_const_x),         \
                             round_shift_x);                                 \
    res_1 = _mm256_sra_epi32(_mm256_add_epi32(res_1, round_const_x),         \
                             round_shift_x);                                 \
                                                                             \
    res_0 = _mm256_packs_epi32(res_0, res_0);                                \
    res_1 = _mm256_packs_epi32(res_1, res_1);                                \
    __m256i res = _mm256_unpacklo_epi16(res_0, res_1);                       \
                                                                             \
    _mm256_store_si256((__m256i *)&im_block[i * im_stride], res);            \
  }

#define CONVOLVE_SR_HORIZONTAL_FILTER_4TAP                                     \
  for (i = 0; i < (im_h - 2); i += 2) {                                        \
    __m256i data = _mm256_castsi128_si256(                                     \
        _mm_loadu_si128((__m128i *)&src_ptr[(i * src_stride) + j]));           \
    data = _mm256_inserti128_si256(                                            \
        data,                                                                  \
        _mm_loadu_si128(                                                       \
            (__m128i *)&src_ptr[(i * src_stride) + j + src_stride]),           \
        1);                                                                    \
    __m256i res = convolve_lowbd_x_4tap(data, coeffs_h + 1, filt);             \
    res =                                                                      \
        _mm256_sra_epi16(_mm256_add_epi16(res, round_const_h), round_shift_h); \
    _mm256_store_si256((__m256i *)&im_block[i * im_stride], res);              \
  }                                                                            \
  __m256i data_1 = _mm256_castsi128_si256(                                     \
      _mm_loadu_si128((__m128i *)&src_ptr[(i * src_stride) + j]));             \
  __m256i res = convolve_lowbd_x_4tap(data_1, coeffs_h + 1, filt);             \
  res = _mm256_sra_epi16(_mm256_add_epi16(res, round_const_h), round_shift_h); \
  _mm256_store_si256((__m256i *)&im_block[i * im_stride], res);

#define CONVOLVE_SR_VERTICAL_FILTER_4TAP                                      \
  __m256i s[6];                                                               \
  __m256i src_0 = _mm256_loadu_si256((__m256i *)(im_block + 0 * im_stride));  \
  __m256i src_1 = _mm256_loadu_si256((__m256i *)(im_block + 1 * im_stride));  \
  __m256i src_2 = _mm256_loadu_si256((__m256i *)(im_block + 2 * im_stride));  \
  __m256i src_3 = _mm256_loadu_si256((__m256i *)(im_block + 3 * im_stride));  \
                                                                              \
  s[0] = _mm256_unpacklo_epi16(src_0, src_1);                                 \
  s[1] = _mm256_unpacklo_epi16(src_2, src_3);                                 \
  s[3] = _mm256_unpackhi_epi16(src_0, src_1);                                 \
  s[4] = _mm256_unpackhi_epi16(src_2, src_3);                                 \
                                                                              \
  for (i = 0; i < h; i += 2) {                                                \
    const int16_t *data = &im_block[i * im_stride];                           \
    const __m256i s4 = _mm256_loadu_si256((__m256i *)(data + 4 * im_stride)); \
    const __m256i s5 = _mm256_loadu_si256((__m256i *)(data + 5 * im_stride)); \
    s[2] = _mm256_unpacklo_epi16(s4, s5);                                     \
    s[5] = _mm256_unpackhi_epi16(s4, s5);                                     \
                                                                              \
    __m256i res_a = convolve_4tap(s, coeffs_v + 1);                           \
    __m256i res_b = convolve_4tap(s + 3, coeffs_v + 1);                       \
                                                                              \
    res_a =                                                                   \
        _mm256_sra_epi32(_mm256_add_epi32(res_a, sum_round_v), sum_shift_v);  \
    res_b =                                                                   \
        _mm256_sra_epi32(_mm256_add_epi32(res_b, sum_round_v), sum_shift_v);  \
    const __m256i res_a_round = _mm256_sra_epi32(                             \
        _mm256_add_epi32(res_a, round_const_v), round_shift_v);               \
    const __m256i res_b_round = _mm256_sra_epi32(                             \
        _mm256_add_epi32(res_b, round_const_v), round_shift_v);               \
    const __m256i res_16bit = _mm256_packs_epi32(res_a_round, res_b_round);   \
    const __m256i res_8b = _mm256_packus_epi16(res_16bit, res_16bit);         \
    const __m128i res_0 = _mm256_castsi256_si128(res_8b);                     \
    const __m128i res_1 = _mm256_extracti128_si256(res_8b, 1);                \
                                                                              \
    __m128i *const p_0 = (__m128i *)&dst[i * dst_stride + j];                 \
    __m128i *const p_1 = (__m128i *)&dst[i * dst_stride + j + dst_stride];    \
    if (w - j > 4) {                                                          \
      _mm_storel_epi64(p_0, res_0);                                           \
      _mm_storel_epi64(p_1, res_1);                                           \
    } else if (w == 4) {                                                      \
      xx_storel_32(p_0, res_0);                                               \
      xx_storel_32(p_1, res_1);                                               \
    } else {                                                                  \
      *(uint16_t *)p_0 = (uint16_t)_mm_cvtsi128_si32(res_0);                  \
      *(uint16_t *)p_1 = (uint16_t)_mm_cvtsi128_si32(res_1);                  \
    }                                                                         \
                                                                              \
    s[0] = s[1];                                                              \
    s[1] = s[2];                                                              \
    s[3] = s[4];                                                              \
    s[4] = s[5];                                                              \
  }

#define CONVOLVE_SR_HORIZONTAL_FILTER_6TAP                                     \
  for (i = 0; i < (im_h - 2); i += 2) {                                        \
    __m256i data = _mm256_castsi128_si256(                                     \
        _mm_loadu_si128((__m128i *)&src_ptr[(i * src_stride) + j]));           \
    data = _mm256_inserti128_si256(                                            \
        data,                                                                  \
        _mm_loadu_si128(                                                       \
            (__m128i *)&src_ptr[(i * src_stride) + j + src_stride]),           \
        1);                                                                    \
                                                                               \
    __m256i res = convolve_lowbd_x_6tap(data, coeffs_h, filt);                 \
    res =                                                                      \
        _mm256_sra_epi16(_mm256_add_epi16(res, round_const_h), round_shift_h); \
    _mm256_store_si256((__m256i *)&im_block[i * im_stride], res);              \
  }                                                                            \
                                                                               \
  __m256i data_1 = _mm256_castsi128_si256(                                     \
      _mm_loadu_si128((__m128i *)&src_ptr[(i * src_stride) + j]));             \
                                                                               \
  __m256i res = convolve_lowbd_x_6tap(data_1, coeffs_h, filt);                 \
                                                                               \
  res = _mm256_sra_epi16(_mm256_add_epi16(res, round_const_h), round_shift_h); \
                                                                               \
  _mm256_store_si256((__m256i *)&im_block[i * im_stride], res);

#define CONVOLVE_SR_VERTICAL_FILTER_6TAP                                      \
  __m256i src_0 = _mm256_loadu_si256((__m256i *)(im_block + 0 * im_stride));  \
  __m256i src_1 = _mm256_loadu_si256((__m256i *)(im_block + 1 * im_stride));  \
  __m256i src_2 = _mm256_loadu_si256((__m256i *)(im_block + 2 * im_stride));  \
  __m256i src_3 = _mm256_loadu_si256((__m256i *)(im_block + 3 * im_stride));  \
                                                                              \
  __m256i s[8];                                                               \
  s[0] = _mm256_unpacklo_epi16(src_0, src_1);                                 \
  s[1] = _mm256_unpacklo_epi16(src_2, src_3);                                 \
                                                                              \
  s[3] = _mm256_unpackhi_epi16(src_0, src_1);                                 \
  s[4] = _mm256_unpackhi_epi16(src_2, src_3);                                 \
                                                                              \
  for (i = 0; i < h; i += 2) {                                                \
    const int16_t *data = &im_block[i * im_stride];                           \
                                                                              \
    const __m256i s6 = _mm256_loadu_si256((__m256i *)(data + 4 * im_stride)); \
    const __m256i s7 = _mm256_loadu_si256((__m256i *)(data + 5 * im_stride)); \
                                                                              \
    s[2] = _mm256_unpacklo_epi16(s6, s7);                                     \
    s[5] = _mm256_unpackhi_epi16(s6, s7);                                     \
                                                                              \
    __m256i res_a = convolve_6tap(s, coeffs_v);                               \
    __m256i res_b = convolve_6tap(s + 3, coeffs_v);                           \
                                                                              \
    res_a =                                                                   \
        _mm256_sra_epi32(_mm256_add_epi32(res_a, sum_round_v), sum_shift_v);  \
    res_b =                                                                   \
        _mm256_sra_epi32(_mm256_add_epi32(res_b, sum_round_v), sum_shift_v);  \
                                                                              \
    const __m256i res_a_round = _mm256_sra_epi32(                             \
        _mm256_add_epi32(res_a, round_const_v), round_shift_v);               \
    const __m256i res_b_round = _mm256_sra_epi32(                             \
        _mm256_add_epi32(res_b, round_const_v), round_shift_v);               \
                                                                              \
    const __m256i res_16bit = _mm256_packs_epi32(res_a_round, res_b_round);   \
    const __m256i res_8b = _mm256_packus_epi16(res_16bit, res_16bit);         \
                                                                              \
    const __m128i res_0 = _mm256_castsi256_si128(res_8b);                     \
    const __m128i res_1 = _mm256_extracti128_si256(res_8b, 1);                \
                                                                              \
    __m128i *const p_0 = (__m128i *)&dst[i * dst_stride + j];                 \
    __m128i *const p_1 = (__m128i *)&dst[i * dst_stride + j + dst_stride];    \
    if (w - j > 4) {                                                          \
      _mm_storel_epi64(p_0, res_0);                                           \
      _mm_storel_epi64(p_1, res_1);                                           \
    } else if (w == 4) {                                                      \
      xx_storel_32(p_0, res_0);                                               \
      xx_storel_32(p_1, res_1);                                               \
    } else {                                                                  \
      *(uint16_t *)p_0 = (uint16_t)_mm_cvtsi128_si32(res_0);                  \
      *(uint16_t *)p_1 = (uint16_t)_mm_cvtsi128_si32(res_1);                  \
    }                                                                         \
                                                                              \
    s[0] = s[1];                                                              \
    s[1] = s[2];                                                              \
                                                                              \
    s[3] = s[4];                                                              \
    s[4] = s[5];                                                              \
  }

#define CONVOLVE_SR_HORIZONTAL_FILTER_8TAP                                     \
  for (i = 0; i < (im_h - 2); i += 2) {                                        \
    __m256i data = _mm256_castsi128_si256(                                     \
        _mm_loadu_si128((__m128i *)&src_ptr[(i * src_stride) + j]));           \
    data = _mm256_inserti128_si256(                                            \
        data,                                                                  \
        _mm_loadu_si128(                                                       \
            (__m128i *)&src_ptr[(i * src_stride) + j + src_stride]),           \
        1);                                                                    \
                                                                               \
    __m256i res = convolve_lowbd_x(data, coeffs_h, filt);                      \
    res =                                                                      \
        _mm256_sra_epi16(_mm256_add_epi16(res, round_const_h), round_shift_h); \
    _mm256_store_si256((__m256i *)&im_block[i * im_stride], res);              \
  }                                                                            \
                                                                               \
  __m256i data_1 = _mm256_castsi128_si256(                                     \
      _mm_loadu_si128((__m128i *)&src_ptr[(i * src_stride) + j]));             \
                                                                               \
  __m256i res = convolve_lowbd_x(data_1, coeffs_h, filt);                      \
                                                                               \
  res = _mm256_sra_epi16(_mm256_add_epi16(res, round_const_h), round_shift_h); \
                                                                               \
  _mm256_store_si256((__m256i *)&im_block[i * im_stride], res);

#define CONVOLVE_SR_VERTICAL_FILTER_8TAP                                      \
  __m256i src_0 = _mm256_loadu_si256((__m256i *)(im_block + 0 * im_stride));  \
  __m256i src_1 = _mm256_loadu_si256((__m256i *)(im_block + 1 * im_stride));  \
  __m256i src_2 = _mm256_loadu_si256((__m256i *)(im_block + 2 * im_stride));  \
  __m256i src_3 = _mm256_loadu_si256((__m256i *)(im_block + 3 * im_stride));  \
  __m256i src_4 = _mm256_loadu_si256((__m256i *)(im_block + 4 * im_stride));  \
  __m256i src_5 = _mm256_loadu_si256((__m256i *)(im_block + 5 * im_stride));  \
                                                                              \
  __m256i s[8];                                                               \
  s[0] = _mm256_unpacklo_epi16(src_0, src_1);                                 \
  s[1] = _mm256_unpacklo_epi16(src_2, src_3);                                 \
  s[2] = _mm256_unpacklo_epi16(src_4, src_5);                                 \
                                                                              \
  s[4] = _mm256_unpackhi_epi16(src_0, src_1);                                 \
  s[5] = _mm256_unpackhi_epi16(src_2, src_3);                                 \
  s[6] = _mm256_unpackhi_epi16(src_4, src_5);                                 \
                                                                              \
  for (i = 0; i < h; i += 2) {                                                \
    const int16_t *data = &im_block[i * im_stride];                           \
                                                                              \
    const __m256i s6 = _mm256_loadu_si256((__m256i *)(data + 6 * im_stride)); \
    const __m256i s7 = _mm256_loadu_si256((__m256i *)(data + 7 * im_stride)); \
                                                                              \
    s[3] = _mm256_unpacklo_epi16(s6, s7);                                     \
    s[7] = _mm256_unpackhi_epi16(s6, s7);                                     \
                                                                              \
    __m256i res_a = convolve(s, coeffs_v);                                    \
    __m256i res_b = convolve(s + 4, coeffs_v);                                \
                                                                              \
    res_a =                                                                   \
        _mm256_sra_epi32(_mm256_add_epi32(res_a, sum_round_v), sum_shift_v);  \
    res_b =                                                                   \
        _mm256_sra_epi32(_mm256_add_epi32(res_b, sum_round_v), sum_shift_v);  \
                                                                              \
    const __m256i res_a_round = _mm256_sra_epi32(                             \
        _mm256_add_epi32(res_a, round_const_v), round_shift_v);               \
    const __m256i res_b_round = _mm256_sra_epi32(                             \
        _mm256_add_epi32(res_b, round_const_v), round_shift_v);               \
                                                                              \
    const __m256i res_16bit = _mm256_packs_epi32(res_a_round, res_b_round);   \
    const __m256i res_8b = _mm256_packus_epi16(res_16bit, res_16bit);         \
                                                                              \
    const __m128i res_0 = _mm256_castsi256_si128(res_8b);                     \
    const __m128i res_1 = _mm256_extracti128_si256(res_8b, 1);                \
                                                                              \
    __m128i *const p_0 = (__m128i *)&dst[i * dst_stride + j];                 \
    __m128i *const p_1 = (__m128i *)&dst[i * dst_stride + j + dst_stride];    \
    if (w - j > 4) {                                                          \
      _mm_storel_epi64(p_0, res_0);                                           \
      _mm_storel_epi64(p_1, res_1);                                           \
    } else if (w == 4) {                                                      \
      xx_storel_32(p_0, res_0);                                               \
      xx_storel_32(p_1, res_1);                                               \
    } else {                                                                  \
      *(uint16_t *)p_0 = (uint16_t)_mm_cvtsi128_si32(res_0);                  \
      *(uint16_t *)p_1 = (uint16_t)_mm_cvtsi128_si32(res_1);                  \
    }                                                                         \
                                                                              \
    s[0] = s[1];                                                              \
    s[1] = s[2];                                                              \
    s[2] = s[3];                                                              \
                                                                              \
    s[4] = s[5];                                                              \
    s[5] = s[6];                                                              \
    s[6] = s[7];                                                              \
  }

#define CONVOLVE_SR_VERT_FILTER_8TAP                                          \
  const __m256i s0 =                                                          \
      _mm256_loadu_si256((__m256i *)(im_block + 0 * im_stride));              \
  const __m256i s1 =                                                          \
      _mm256_loadu_si256((__m256i *)(im_block + 1 * im_stride));              \
  const __m256i s2 =                                                          \
      _mm256_loadu_si256((__m256i *)(im_block + 2 * im_stride));              \
  const __m256i s3 =                                                          \
      _mm256_loadu_si256((__m256i *)(im_block + 3 * im_stride));              \
  const __m256i s4 =                                                          \
      _mm256_loadu_si256((__m256i *)(im_block + 4 * im_stride));              \
  const __m256i s5 =                                                          \
      _mm256_loadu_si256((__m256i *)(im_block + 5 * im_stride));              \
                                                                              \
  s[0] = _mm256_unpacklo_epi16(s0, s1);                                       \
  s[1] = _mm256_unpacklo_epi16(s2, s3);                                       \
  s[2] = _mm256_unpacklo_epi16(s4, s5);                                       \
                                                                              \
  s[4] = _mm256_unpackhi_epi16(s0, s1);                                       \
  s[5] = _mm256_unpackhi_epi16(s2, s3);                                       \
  s[6] = _mm256_unpackhi_epi16(s4, s5);                                       \
                                                                              \
  for (i = 0; i < h; i += 2) {                                                \
    const int16_t *data = &im_block[i * im_stride];                           \
                                                                              \
    const __m256i s6 = _mm256_loadu_si256((__m256i *)(data + 6 * im_stride)); \
    const __m256i s7 = _mm256_loadu_si256((__m256i *)(data + 7 * im_stride)); \
                                                                              \
    s[3] = _mm256_unpacklo_epi16(s6, s7);                                     \
    s[7] = _mm256_unpackhi_epi16(s6, s7);                                     \
                                                                              \
    const __m256i res_a = convolve(s, coeffs_y);                              \
    __m256i res_a_round = _mm256_sra_epi32(                                   \
        _mm256_add_epi32(res_a, round_const_y), round_shift_y);               \
                                                                              \
    if (w - j > 4) {                                                          \
      const __m256i res_b = convolve(s + 4, coeffs_y);                        \
      const __m256i res_b_round = _mm256_sra_epi32(                           \
          _mm256_add_epi32(res_b, round_const_y), round_shift_y);             \
                                                                              \
      __m256i res_16bit = _mm256_packs_epi32(res_a_round, res_b_round);       \
      res_16bit = _mm256_min_epi16(res_16bit, clip_pixel);                    \
      res_16bit = _mm256_max_epi16(res_16bit, zero);                          \
                                                                              \
      _mm_storeu_si128((__m128i *)&dst[i * dst_stride + j],                   \
                       _mm256_castsi256_si128(res_16bit));                    \
      _mm_storeu_si128((__m128i *)&dst[i * dst_stride + j + dst_stride],      \
                       _mm256_extracti128_si256(res_16bit, 1));               \
    } else if (w == 4 || (w - j == 4)) {                                      \
      res_a_round = _mm256_packs_epi32(res_a_round, res_a_round);             \
      res_a_round = _mm256_min_epi16(res_a_round, clip_pixel);                \
      res_a_round = _mm256_max_epi16(res_a_round, zero);                      \
                                                                              \
      _mm_storel_epi64((__m128i *)&dst[i * dst_stride + j],                   \
                       _mm256_castsi256_si128(res_a_round));                  \
      _mm_storel_epi64((__m128i *)&dst[i * dst_stride + j + dst_stride],      \
                       _mm256_extracti128_si256(res_a_round, 1));             \
    } else {                                                                  \
      res_a_round = _mm256_packs_epi32(res_a_round, res_a_round);             \
      res_a_round = _mm256_min_epi16(res_a_round, clip_pixel);                \
      res_a_round = _mm256_max_epi16(res_a_round, zero);                      \
                                                                              \
      xx_storel_32((__m128i *)&dst[i * dst_stride + j],                       \
                   _mm256_castsi256_si128(res_a_round));                      \
      xx_storel_32((__m128i *)&dst[i * dst_stride + j + dst_stride],          \
                   _mm256_extracti128_si256(res_a_round, 1));                 \
    }                                                                         \
                                                                              \
    s[0] = s[1];                                                              \
    s[1] = s[2];                                                              \
    s[2] = s[3];                                                              \
                                                                              \
    s[4] = s[5];                                                              \
    s[5] = s[6];                                                              \
    s[6] = s[7];                                                              \
  }

#define CONVOLVE_SR_VERT_FILTER_6TAP                                          \
  const __m256i s0 =                                                          \
      _mm256_loadu_si256((__m256i *)(im_block + 0 * im_stride));              \
  const __m256i s1 =                                                          \
      _mm256_loadu_si256((__m256i *)(im_block + 1 * im_stride));              \
  const __m256i s2 =                                                          \
      _mm256_loadu_si256((__m256i *)(im_block + 2 * im_stride));              \
  const __m256i s3 =                                                          \
      _mm256_loadu_si256((__m256i *)(im_block + 3 * im_stride));              \
                                                                              \
  s[0] = _mm256_unpacklo_epi16(s0, s1);                                       \
  s[1] = _mm256_unpacklo_epi16(s2, s3);                                       \
                                                                              \
  s[3] = _mm256_unpackhi_epi16(s0, s1);                                       \
  s[4] = _mm256_unpackhi_epi16(s2, s3);                                       \
                                                                              \
  for (i = 0; i < h; i += 2) {                                                \
    const int16_t *data = &im_block[i * im_stride];                           \
                                                                              \
    const __m256i s6 = _mm256_loadu_si256((__m256i *)(data + 4 * im_stride)); \
    const __m256i s7 = _mm256_loadu_si256((__m256i *)(data + 5 * im_stride)); \
                                                                              \
    s[2] = _mm256_unpacklo_epi16(s6, s7);                                     \
    s[5] = _mm256_unpackhi_epi16(s6, s7);                                     \
                                                                              \
    const __m256i res_a = convolve_6tap(s, coeffs_y);                         \
    __m256i res_a_round = _mm256_sra_epi32(                                   \
        _mm256_add_epi32(res_a, round_const_y), round_shift_y);               \
                                                                              \
    if (w - j > 4) {                                                          \
      const __m256i res_b = convolve_6tap(s + 3, coeffs_y);                   \
      const __m256i res_b_round = _mm256_sra_epi32(                           \
          _mm256_add_epi32(res_b, round_const_y), round_shift_y);             \
                                                                              \
      __m256i res_16bit = _mm256_packs_epi32(res_a_round, res_b_round);       \
      res_16bit = _mm256_min_epi16(res_16bit, clip_pixel);                    \
      res_16bit = _mm256_max_epi16(res_16bit, zero);                          \
                                                                              \
      _mm_storeu_si128((__m128i *)&dst[i * dst_stride + j],                   \
                       _mm256_castsi256_si128(res_16bit));                    \
      _mm_storeu_si128((__m128i *)&dst[i * dst_stride + j + dst_stride],      \
                       _mm256_extracti128_si256(res_16bit, 1));               \
    } else if (w == 4 || (w - j == 4)) {                                      \
      res_a_round = _mm256_packs_epi32(res_a_round, res_a_round);             \
      res_a_round = _mm256_min_epi16(res_a_round, clip_pixel);                \
      res_a_round = _mm256_max_epi16(res_a_round, zero);                      \
                                                                              \
      _mm_storel_epi64((__m128i *)&dst[i * dst_stride + j],                   \
                       _mm256_castsi256_si128(res_a_round));                  \
      _mm_storel_epi64((__m128i *)&dst[i * dst_stride + j + dst_stride],      \
                       _mm256_extracti128_si256(res_a_round, 1));             \
    } else {                                                                  \
      res_a_round = _mm256_packs_epi32(res_a_round, res_a_round);             \
      res_a_round = _mm256_min_epi16(res_a_round, clip_pixel);                \
      res_a_round = _mm256_max_epi16(res_a_round, zero);                      \
                                                                              \
      xx_storel_32((__m128i *)&dst[i * dst_stride + j],                       \
                   _mm256_castsi256_si128(res_a_round));                      \
      xx_storel_32((__m128i *)&dst[i * dst_stride + j + dst_stride],          \
                   _mm256_extracti128_si256(res_a_round, 1));                 \
    }                                                                         \
                                                                              \
    s[0] = s[1];                                                              \
    s[1] = s[2];                                                              \
                                                                              \
    s[3] = s[4];                                                              \
    s[4] = s[5];                                                              \
  }

#define CONVOLVE_SR_VERT_FILTER_4TAP                                          \
  const __m256i s0 =                                                          \
      _mm256_loadu_si256((__m256i *)(im_block + 0 * im_stride));              \
  const __m256i s1 =                                                          \
      _mm256_loadu_si256((__m256i *)(im_block + 1 * im_stride));              \
  s[0] = _mm256_unpacklo_epi16(s0, s1);                                       \
  s[2] = _mm256_unpackhi_epi16(s0, s1);                                       \
                                                                              \
  for (i = 0; i < h; i += 2) {                                                \
    const int16_t *data = &im_block[i * im_stride];                           \
                                                                              \
    const __m256i s6 = _mm256_loadu_si256((__m256i *)(data + 2 * im_stride)); \
    const __m256i s7 = _mm256_loadu_si256((__m256i *)(data + 3 * im_stride)); \
                                                                              \
    s[1] = _mm256_unpacklo_epi16(s6, s7);                                     \
    s[3] = _mm256_unpackhi_epi16(s6, s7);                                     \
                                                                              \
    const __m256i res_a = convolve_4tap(s, coeffs_y);                         \
    __m256i res_a_round = _mm256_sra_epi32(                                   \
        _mm256_add_epi32(res_a, round_const_y), round_shift_y);               \
                                                                              \
    if (w - j > 4) {                                                          \
      const __m256i res_b = convolve_4tap(s + 2, coeffs_y);                   \
      const __m256i res_b_round = _mm256_sra_epi32(                           \
          _mm256_add_epi32(res_b, round_const_y), round_shift_y);             \
                                                                              \
      __m256i res_16bit = _mm256_packs_epi32(res_a_round, res_b_round);       \
      res_16bit = _mm256_min_epi16(res_16bit, clip_pixel);                    \
      res_16bit = _mm256_max_epi16(res_16bit, zero);                          \
                                                                              \
      _mm_storeu_si128((__m128i *)&dst[i * dst_stride + j],                   \
                       _mm256_castsi256_si128(res_16bit));                    \
      _mm_storeu_si128((__m128i *)&dst[i * dst_stride + j + dst_stride],      \
                       _mm256_extracti128_si256(res_16bit, 1));               \
    } else if (w == 4 || (w - j == 4)) {                                      \
      res_a_round = _mm256_packs_epi32(res_a_round, res_a_round);             \
      res_a_round = _mm256_min_epi16(res_a_round, clip_pixel);                \
      res_a_round = _mm256_max_epi16(res_a_round, zero);                      \
                                                                              \
      _mm_storel_epi64((__m128i *)&dst[i * dst_stride + j],                   \
                       _mm256_castsi256_si128(res_a_round));                  \
      _mm_storel_epi64((__m128i *)&dst[i * dst_stride + j + dst_stride],      \
                       _mm256_extracti128_si256(res_a_round, 1));             \
    } else {                                                                  \
      res_a_round = _mm256_packs_epi32(res_a_round, res_a_round);             \
      res_a_round = _mm256_min_epi16(res_a_round, clip_pixel);                \
      res_a_round = _mm256_max_epi16(res_a_round, zero);                      \
                                                                              \
      xx_storel_32((__m128i *)&dst[i * dst_stride + j],                       \
                   _mm256_castsi256_si128(res_a_round));                      \
      xx_storel_32((__m128i *)&dst[i * dst_stride + j + dst_stride],          \
                   _mm256_extracti128_si256(res_a_round, 1));                 \
    }                                                                         \
                                                                              \
    s[0] = s[1];                                                              \
    s[2] = s[3];                                                              \
  }

#define CONVOLVE_SR_VERT_FILTER_2TAP                                          \
  for (i = 0; i < h; i += 2) {                                                \
    const int16_t *data = &im_block[i * im_stride];                           \
                                                                              \
    const __m256i s6 = _mm256_loadu_si256((__m256i *)(data + 0 * im_stride)); \
    const __m256i s7 = _mm256_loadu_si256((__m256i *)(data + 1 * im_stride)); \
                                                                              \
    s[0] = _mm256_unpacklo_epi16(s6, s7);                                     \
    s[1] = _mm256_unpackhi_epi16(s6, s7);                                     \
                                                                              \
    const __m256i res_a = _mm256_madd_epi16(s[0], coeffs_y[0]);               \
    __m256i res_a_round = _mm256_sra_epi32(                                   \
        _mm256_add_epi32(res_a, round_const_y), round_shift_y);               \
                                                                              \
    if (w - j > 4) {                                                          \
      const __m256i res_b = _mm256_madd_epi16(s[1], coeffs_y[0]);             \
      const __m256i res_b_round = _mm256_sra_epi32(                           \
          _mm256_add_epi32(res_b, round_const_y), round_shift_y);             \
                                                                              \
      __m256i res_16bit = _mm256_packs_epi32(res_a_round, res_b_round);       \
      res_16bit = _mm256_min_epi16(res_16bit, clip_pixel);                    \
      res_16bit = _mm256_max_epi16(res_16bit, zero);                          \
                                                                              \
      _mm_storeu_si128((__m128i *)&dst[i * dst_stride + j],                   \
                       _mm256_castsi256_si128(res_16bit));                    \
      _mm_storeu_si128((__m128i *)&dst[i * dst_stride + j + dst_stride],      \
                       _mm256_extracti128_si256(res_16bit, 1));               \
    } else if (w == 4 || (w - j == 4)) {                                      \
      res_a_round = _mm256_packs_epi32(res_a_round, res_a_round);             \
      res_a_round = _mm256_min_epi16(res_a_round, clip_pixel);                \
      res_a_round = _mm256_max_epi16(res_a_round, zero);                      \
                                                                              \
      _mm_storel_epi64((__m128i *)&dst[i * dst_stride + j],                   \
                       _mm256_castsi256_si128(res_a_round));                  \
      _mm_storel_epi64((__m128i *)&dst[i * dst_stride + j + dst_stride],      \
                       _mm256_extracti128_si256(res_a_round, 1));             \
    } else {                                                                  \
      res_a_round = _mm256_packs_epi32(res_a_round, res_a_round);             \
      res_a_round = _mm256_min_epi16(res_a_round, clip_pixel);                \
      res_a_round = _mm256_max_epi16(res_a_round, zero);                      \
                                                                              \
      xx_storel_32((__m128i *)&dst[i * dst_stride + j],                       \
                   _mm256_castsi256_si128(res_a_round));                      \
      xx_storel_32((__m128i *)&dst[i * dst_stride + j + dst_stride],          \
                   _mm256_extracti128_si256(res_a_round, 1));                 \
    }                                                                         \
  }

#define DIST_WTD_CONVOLVE_HORIZONTAL_FILTER_8TAP                               \
  for (i = 0; i < im_h; i += 2) {                                              \
    __m256i data = _mm256_castsi128_si256(_mm_loadu_si128((__m128i *)src_h));  \
    if (i + 1 < im_h)                                                          \
      data = _mm256_inserti128_si256(                                          \
          data, _mm_loadu_si128((__m128i *)(src_h + src_stride)), 1);          \
    src_h += (src_stride << 1);                                                \
    __m256i res = convolve_lowbd_x(data, coeffs_x, filt);                      \
                                                                               \
    res =                                                                      \
        _mm256_sra_epi16(_mm256_add_epi16(res, round_const_h), round_shift_h); \
                                                                               \
    _mm256_store_si256((__m256i *)&im_block[i * im_stride], res);              \
  }

// In optical flow MV refinement, unaligned store (_mm_storeu_si128) is used
// in this function to avoid a subblock boundary error.
#define DIST_WTD_CONVOLVE_VERTICAL_FILTER_8TAP                                 \
  __m256i s[8];                                                                \
  __m256i s0 = _mm256_loadu_si256((__m256i *)(im_block + 0 * im_stride));      \
  __m256i s1 = _mm256_loadu_si256((__m256i *)(im_block + 1 * im_stride));      \
  __m256i s2 = _mm256_loadu_si256((__m256i *)(im_block + 2 * im_stride));      \
  __m256i s3 = _mm256_loadu_si256((__m256i *)(im_block + 3 * im_stride));      \
  __m256i s4 = _mm256_loadu_si256((__m256i *)(im_block + 4 * im_stride));      \
  __m256i s5 = _mm256_loadu_si256((__m256i *)(im_block + 5 * im_stride));      \
                                                                               \
  s[0] = _mm256_unpacklo_epi16(s0, s1);                                        \
  s[1] = _mm256_unpacklo_epi16(s2, s3);                                        \
  s[2] = _mm256_unpacklo_epi16(s4, s5);                                        \
                                                                               \
  s[4] = _mm256_unpackhi_epi16(s0, s1);                                        \
  s[5] = _mm256_unpackhi_epi16(s2, s3);                                        \
  s[6] = _mm256_unpackhi_epi16(s4, s5);                                        \
                                                                               \
  for (i = 0; i < h; i += 2) {                                                 \
    const int16_t *data = &im_block[i * im_stride];                            \
                                                                               \
    const __m256i s6 = _mm256_loadu_si256((__m256i *)(data + 6 * im_stride));  \
    const __m256i s7 = _mm256_loadu_si256((__m256i *)(data + 7 * im_stride));  \
                                                                               \
    s[3] = _mm256_unpacklo_epi16(s6, s7);                                      \
    s[7] = _mm256_unpackhi_epi16(s6, s7);                                      \
                                                                               \
    const __m256i res_a = convolve(s, coeffs_y);                               \
    const __m256i res_a_round = _mm256_sra_epi32(                              \
        _mm256_add_epi32(res_a, round_const_v), round_shift_v);                \
                                                                               \
    if (w - j > 4) {                                                           \
      const __m256i res_b = convolve(s + 4, coeffs_y);                         \
      const __m256i res_b_round = _mm256_sra_epi32(                            \
          _mm256_add_epi32(res_b, round_const_v), round_shift_v);              \
      const __m256i res_16b = _mm256_packs_epi32(res_a_round, res_b_round);    \
      const __m256i res_unsigned = _mm256_add_epi16(res_16b, offset_const);    \
                                                                               \
      if (do_average) {                                                        \
        const __m256i data_ref_0 = load_line2_avx2(                            \
            &dst[i * dst_stride + j], &dst[i * dst_stride + j + dst_stride]);  \
        const __m256i comp_avg_res =                                           \
            comp_avg(&data_ref_0, &res_unsigned, &wt, use_wtd_comp_avg);       \
                                                                               \
        const __m256i round_result = convolve_rounding(                        \
            &comp_avg_res, &offset_const, &rounding_const, rounding_shift);    \
                                                                               \
        const __m256i res_8 = _mm256_packus_epi16(round_result, round_result); \
        const __m128i res_0 = _mm256_castsi256_si128(res_8);                   \
        const __m128i res_1 = _mm256_extracti128_si256(res_8, 1);              \
                                                                               \
        _mm_storel_epi64((__m128i *)(&dst0[i * dst_stride0 + j]), res_0);      \
        _mm_storel_epi64(                                                      \
            (__m128i *)((&dst0[i * dst_stride0 + j + dst_stride0])), res_1);   \
      } else {                                                                 \
        const __m128i res_0 = _mm256_castsi256_si128(res_unsigned);            \
        _mm_storeu_si128((__m128i *)(&dst[i * dst_stride + j]), res_0);        \
                                                                               \
        const __m128i res_1 = _mm256_extracti128_si256(res_unsigned, 1);       \
        _mm_storeu_si128((__m128i *)(&dst[i * dst_stride + j + dst_stride]),   \
                         res_1);                                               \
      }                                                                        \
    } else {                                                                   \
      const __m256i res_16b = _mm256_packs_epi32(res_a_round, res_a_round);    \
      const __m256i res_unsigned = _mm256_add_epi16(res_16b, offset_const);    \
                                                                               \
      if (do_average) {                                                        \
        const __m256i data_ref_0 = load_line2_avx2(                            \
            &dst[i * dst_stride + j], &dst[i * dst_stride + j + dst_stride]);  \
                                                                               \
        const __m256i comp_avg_res =                                           \
            comp_avg(&data_ref_0, &res_unsigned, &wt, use_wtd_comp_avg);       \
                                                                               \
        const __m256i round_result = convolve_rounding(                        \
            &comp_avg_res, &offset_const, &rounding_const, rounding_shift);    \
                                                                               \
        const __m256i res_8 = _mm256_packus_epi16(round_result, round_result); \
        const __m128i res_0 = _mm256_castsi256_si128(res_8);                   \
        const __m128i res_1 = _mm256_extracti128_si256(res_8, 1);              \
                                                                               \
        *(uint32_t *)(&dst0[i * dst_stride0 + j]) = _mm_cvtsi128_si32(res_0);  \
        *(uint32_t *)(&dst0[i * dst_stride0 + j + dst_stride0]) =              \
            _mm_cvtsi128_si32(res_1);                                          \
                                                                               \
      } else {                                                                 \
        const __m128i res_0 = _mm256_castsi256_si128(res_unsigned);            \
        _mm_storeu_si128((__m128i *)(&dst[i * dst_stride + j]), res_0);        \
                                                                               \
        const __m128i res_1 = _mm256_extracti128_si256(res_unsigned, 1);       \
        _mm_storeu_si128((__m128i *)(&dst[i * dst_stride + j + dst_stride]),   \
                         res_1);                                               \
      }                                                                        \
    }                                                                          \
                                                                               \
    s[0] = s[1];                                                               \
    s[1] = s[2];                                                               \
    s[2] = s[3];                                                               \
                                                                               \
    s[4] = s[5];                                                               \
    s[5] = s[6];                                                               \
    s[6] = s[7];                                                               \
  }

/* --- TOP-LEVEL PUBLIC INLINE FUNCTIONS THAT USE MACROS --- */

static INLINE __m256i prepare_coeffs_bilinear(
    const InterpFilterParams *const filter_params, const int subpel_q4) {
  const int16_t *filter = av2_get_interp_filter_subpel_kernel(
      filter_params, subpel_q4 & SUBPEL_MASK);
  const int16_t *filter_pos = filter;
  if (filter_params->taps == 8) filter_pos = filter + 3;
  const int filter_coeff = ((filter_pos[1] << 16) | filter_pos[0]);
  return _mm256_set1_epi32(filter_coeff);
}

static INLINE void highbd_dist_wtd_convolve_2d_copy_do_average(
    __m256i data, __m256i zero, __m256i res, const __m256i *offset_const,
    const __m256i *wt0, const __m256i *wt1, int use_wtd_comp_avg,
    const __m256i *const rounding_const, int rounding_shift,
    __m256i clip_pixel_to_bd, __m256i *res_clip) {
  const __m256i data_ref_0_lo = _mm256_unpacklo_epi16(data, zero);
  const __m256i data_ref_0_hi = _mm256_unpackhi_epi16(data, zero);
  const __m256i res_32b_lo = _mm256_unpacklo_epi16(res, zero);
  const __m256i res_unsigned_lo = _mm256_add_epi32(res_32b_lo, *offset_const);
  const __m256i comp_avg_res_lo = highbd_comp_avg(
      &data_ref_0_lo, &res_unsigned_lo, wt0, wt1, use_wtd_comp_avg);
  const __m256i res_32b_hi = _mm256_unpackhi_epi16(res, zero);
  const __m256i res_unsigned_hi = _mm256_add_epi32(res_32b_hi, *offset_const);
  const __m256i comp_avg_res_hi = highbd_comp_avg(
      &data_ref_0_hi, &res_unsigned_hi, wt0, wt1, use_wtd_comp_avg);
  const __m256i round_result_lo = highbd_convolve_rounding(
      &comp_avg_res_lo, offset_const, rounding_const, rounding_shift);
  const __m256i round_result_hi = highbd_convolve_rounding(
      &comp_avg_res_hi, offset_const, rounding_const, rounding_shift);
  const __m256i res_16b = _mm256_packus_epi32(round_result_lo, round_result_hi);
  *res_clip = _mm256_min_epi16(res_16b, clip_pixel_to_bd);
}

static INLINE void dist_wtd_convolve_horiz_w4(
    const uint16_t *src_ptr, int src_stride, const __m256i *const coeffs,
    int im_h, int16_t *im_block, int im_stride, const __m256i *round_const_x,
    const __m128i *round_shift_x) {
  __m256i s[2];
  DECLARE_ALIGNED(32, static const uint8_t,
                  shuffle_mask0_loc[32]) = { 0, 1, 2, 3, 2, 3, 4, 5, 4, 5, 6,
                                             7, 6, 7, 8, 9, 0, 1, 2, 3, 2, 3,
                                             4, 5, 4, 5, 6, 7, 6, 7, 8, 9 };
  DECLARE_ALIGNED(32, static const uint8_t, shuffle_mask1_loc[32]) = {
    4, 5, 6, 7, 6, 7, 8, 9, 8, 9, 10, 11, 10, 11, 12, 13,
    4, 5, 6, 7, 6, 7, 8, 9, 8, 9, 10, 11, 10, 11, 12, 13
  };
  for (int i = 0; i < im_h; i += 2) {
    const __m256i row0 =
        _mm256_loadu_si256((__m256i *)&src_ptr[i * src_stride]);
    __m256i row1 = _mm256_set1_epi16(0);
    if (i + 1 < im_h)
      row1 = _mm256_loadu_si256((__m256i *)&src_ptr[(i + 1) * src_stride]);
    const __m256i r0 = _mm256_permute2x128_si256(row0, row1, 0x20);
    s[0] = _mm256_shuffle_epi8(r0,
                               _mm256_load_si256((__m256i *)shuffle_mask0_loc));
    s[1] = _mm256_shuffle_epi8(r0,
                               _mm256_load_si256((__m256i *)shuffle_mask1_loc));
    const __m256i res_0 = _mm256_madd_epi16(s[0], coeffs[0]);
    const __m256i res_1 = _mm256_madd_epi16(s[1], coeffs[1]);
    __m256i res = _mm256_add_epi32(res_0, res_1);
    res =
        _mm256_sra_epi32(_mm256_add_epi32(res, *round_const_x), *round_shift_x);
    _mm256_store_si256((__m256i *)(im_block + i * im_stride),
                       _mm256_packs_epi32(res, res));
  }
}

static INLINE void av2_highbd_convolve_2d_sr_specialized_avx2(
    const uint16_t *src, int src_stride, uint16_t *dst, int dst_stride, int w,
    int h, const InterpFilterParams *filter_params_x,
    const InterpFilterParams *filter_params_y, const int subpel_x_qn,
    const int subpel_y_qn, ConvolveParams *conv_params, int bd) {
  const int32_t tap_x = get_filter_tap(filter_params_x, subpel_x_qn);
  const int32_t tap_y = get_filter_tap(filter_params_y, subpel_y_qn);
  DECLARE_ALIGNED(32, int16_t, im_block[(MAX_SB_SIZE + MAX_FILTER_TAP) * 8]);
  const int im_h = h + tap_y - 1;
  const int im_stride = 8;
  const int fo_vert = tap_y / 2 - 1;
  const int fo_horiz = tap_x / 2 - 1;
  const uint16_t *const src_ptr = src - fo_vert * src_stride - fo_horiz;
  int i, j;
  assert(bd + FILTER_BITS + 2 - conv_params->round_0 <= 16);
  __m256i s[8];
  __m256i coeffs_y[4] = { 0 };
  __m256i coeffs_x[4] = { 0 };
  const __m256i round_const_x = _mm256_set1_epi32(
      ((1 << conv_params->round_0) >> 1) + (1 << (bd + FILTER_BITS - 1)));
  const __m128i round_shift_x = _mm_cvtsi32_si128(conv_params->round_0);
  const __m256i round_const_y = _mm256_set1_epi32(
      ((1 << conv_params->round_1) >> 1) -
      (1 << (bd + 2 * FILTER_BITS - conv_params->round_0 - 1)));
  const __m128i round_shift_y = _mm_cvtsi32_si128(conv_params->round_1);
  const __m256i clip_pixel = _mm256_set1_epi16((1 << bd) - 1);
  const __m256i zero = _mm256_setzero_si256();
  if (tap_x == 8)
    prepare_coeffs(filter_params_x, subpel_x_qn, coeffs_x);
  else if (tap_x == 6)
    prepare_coeffs_6t(filter_params_x, subpel_x_qn, coeffs_x);
  else if (tap_x == 4)
    prepare_coeffs_4t(filter_params_x, subpel_x_qn, coeffs_x);
  else
    coeffs_x[0] = prepare_coeffs_bilinear(filter_params_x, subpel_x_qn);
  if (tap_y == 8)
    prepare_coeffs(filter_params_y, subpel_y_qn, coeffs_y);
  else if (tap_y == 6)
    prepare_coeffs_6t(filter_params_y, subpel_y_qn, coeffs_y);
  else if (tap_y == 4)
    prepare_coeffs_4t(filter_params_y, subpel_y_qn, coeffs_y);
  else
    coeffs_y[0] = prepare_coeffs_bilinear(filter_params_y, subpel_y_qn);
  for (j = 0; j < w; j += 8) {
    if (tap_x == 8) {
      CONVOLVE_HORIZ_FILTER_8TAP
    } else if (tap_x == 6) {
      CONVOLVE_HORIZ_FILTER_6TAP
    } else if (tap_x == 4) {
      CONVOLVE_HORIZ_FILTER_4TAP
    } else {
      CONVOLVE_HORIZ_FILTER_2TAP
    }
    if (tap_y == 8) {
      CONVOLVE_SR_VERT_FILTER_8TAP
    } else if (tap_y == 6) {
      CONVOLVE_SR_VERT_FILTER_6TAP
    } else if (tap_y == 4) {
      CONVOLVE_SR_VERT_FILTER_4TAP
    } else {
      CONVOLVE_SR_VERT_FILTER_2TAP
    }
  }
}

static INLINE void av2_highbd_convolve_2d_sr_bilinear_avx2(
    const uint16_t *src, int src_stride, uint16_t *dst, int dst_stride, int w,
    int h, const InterpFilterParams *filter_params_x,
    const InterpFilterParams *filter_params_y, const int subpel_x_qn,
    const int subpel_y_qn, ConvolveParams *conv_params, int bd) {
  if (h % 2 != 0 || w < 4) {
    av2_highbd_convolve_2d_sr_specialized_avx2(
        src, src_stride, dst, dst_stride, w, h, filter_params_x,
        filter_params_y, subpel_x_qn, subpel_y_qn, conv_params, bd);
    return;
  }
  assert(bd + FILTER_BITS + 2 - conv_params->round_0 <= 16);
  assert((FILTER_BITS * 2 - conv_params->round_0 - conv_params->round_1) == 0);
  const int im_h = h + 2 - 1;
  const __m256i round_const_x = _mm256_set1_epi32(
      ((1 << conv_params->round_0) >> 1) + (1 << (bd + FILTER_BITS - 1)));
  const __m128i round_shift_x = _mm_cvtsi32_si128(conv_params->round_0);
  const __m256i round_const_y = _mm256_set1_epi32(
      ((1 << conv_params->round_1) >> 1) -
      (1 << (bd + 2 * FILTER_BITS - conv_params->round_0 - 1)));
  const __m128i round_shift_y = _mm_cvtsi32_si128(conv_params->round_1);
  const __m256i clip_pixel = _mm256_set1_epi16((1 << bd) - 1);
  const __m256i zero = _mm256_setzero_si256();
  const __m256i coeffs_x_bilinear =
      prepare_coeffs_bilinear(filter_params_x, subpel_x_qn);
  const __m256i coeffs_y_bilinear =
      prepare_coeffs_bilinear(filter_params_y, subpel_y_qn);
  const int8_t reorder_pixels[32] = { 0,  1,  8, 9,  2,  3,  10, 11, 4,  5, 12,
                                      13, 6,  7, 14, 15, 0,  1,  8,  9,  2, 3,
                                      10, 11, 4, 5,  12, 13, 6,  7,  14, 15 };
  const __m256i shuffle_mask0 = _mm256_loadu_si256((__m256i *)reorder_pixels);
  int remain_wd = w;
  for (int j = 0; (j < w) && (remain_wd > 7); j += 8) {
    __m256i row[2];
    const __m256i a = _mm256_loadu_si256((__m256i *)&src[0 * src_stride + j]);
    const __m256i b = _mm256_loadu_si256((__m256i *)&src[1 * src_stride + j]);
    __m256i rw0 = _mm256_permute2x128_si256(a, b, 0x20);
    __m256i rw1 = _mm256_permute2x128_si256(a, b, 0x31);
    rw1 = _mm256_alignr_epi8(rw1, rw0, 2);
    __m256i res0 = _mm256_madd_epi16(rw0, coeffs_x_bilinear);
    __m256i res1 = _mm256_madd_epi16(rw1, coeffs_x_bilinear);
    res0 =
        _mm256_sra_epi32(_mm256_add_epi32(res0, round_const_x), round_shift_x);
    res1 =
        _mm256_sra_epi32(_mm256_add_epi32(res1, round_const_x), round_shift_x);
    row[0] = _mm256_packs_epi32(res0, res1);
    for (int i = 2; i < im_h; i += 2) {
      const __m256i row0 =
          _mm256_loadu_si256((__m256i *)&src[i * src_stride + j]);
      __m256i row1 = _mm256_setzero_si256();
      if (i + 1 < im_h)
        row1 = _mm256_loadu_si256((__m256i *)&src[(i + 1) * src_stride + j]);
      const __m256i r0 = _mm256_permute2x128_si256(row0, row1, 0x20);
      __m256i r1 = _mm256_permute2x128_si256(row0, row1, 0x31);
      r1 = _mm256_alignr_epi8(r1, r0, 2);
      __m256i res_0 = _mm256_madd_epi16(r0, coeffs_x_bilinear);
      __m256i res_1 = _mm256_madd_epi16(r1, coeffs_x_bilinear);
      res_0 = _mm256_sra_epi32(_mm256_add_epi32(res_0, round_const_x),
                               round_shift_x);
      res_1 = _mm256_sra_epi32(_mm256_add_epi32(res_1, round_const_x),
                               round_shift_x);
      row[1] = _mm256_packs_epi32(res_0, res_1);
      const __m256i row2 = _mm256_permute2x128_si256(row[0], row[1], 0x21);
      const __m256i odd = _mm256_unpacklo_epi16(row[0], row2);
      const __m256i even = _mm256_unpackhi_epi16(row[0], row2);
      const __m256i res_0_v = _mm256_madd_epi16(odd, coeffs_y_bilinear);
      const __m256i res_1_v = _mm256_madd_epi16(even, coeffs_y_bilinear);
      const __m256i res_a_round = _mm256_sra_epi32(
          _mm256_add_epi32(res_0_v, round_const_y), round_shift_y);
      const __m256i res_b_round = _mm256_sra_epi32(
          _mm256_add_epi32(res_1_v, round_const_y), round_shift_y);
      __m256i res_16bit = _mm256_packs_epi32(res_a_round, res_b_round);
      res_16bit = _mm256_shuffle_epi8(res_16bit, shuffle_mask0);
      res_16bit = _mm256_min_epi16(res_16bit, clip_pixel);
      res_16bit = _mm256_max_epi16(res_16bit, zero);
      _mm_storeu_si128((__m128i *)&dst[(i - 2) * dst_stride + j],
                       _mm256_castsi256_si128(res_16bit));
      _mm_storeu_si128((__m128i *)&dst[(i - 1) * dst_stride + j],
                       _mm256_extracti128_si256(res_16bit, 1));
      row[0] = row[1];
    }
    remain_wd -= 8;
  }
  if (remain_wd == 0) return;
  if (remain_wd == 4) {
    __m256i row[2];
    const __m128i a =
        _mm_loadu_si128((__m128i *)&src[0 * src_stride + (w - remain_wd)]);
    const __m128i b =
        _mm_loadu_si128((__m128i *)&src[1 * src_stride + (w - remain_wd)]);
    __m256i read = _mm256_insertf128_si256(_mm256_castsi128_si256(a), b, 0x1);
    DECLARE_ALIGNED(32, static const uint8_t, shuffle_mask1_loc_wd4[32]) = {
      0, 1, 2, 3, 2, 3, 4, 5, 4, 5, 6, 7, 6, 7, 8, 9,
      0, 1, 2, 3, 2, 3, 4, 5, 4, 5, 6, 7, 6, 7, 8, 9
    };
    read = _mm256_shuffle_epi8(
        read, _mm256_loadu_si256((__m256i *)shuffle_mask1_loc_wd4));
    __m256i res0 = _mm256_madd_epi16(read, coeffs_x_bilinear);
    res0 =
        _mm256_sra_epi32(_mm256_add_epi32(res0, round_const_x), round_shift_x);
    row[0] = _mm256_packs_epi32(res0, res0);
    for (int i = 2; i < im_h; i += 2) {
      const __m128i row0 =
          _mm_loadu_si128((__m128i *)&src[i * src_stride + (w - remain_wd)]);
      __m128i row1 = _mm_setzero_si128();
      if (i + 1 < im_h)
        row1 = _mm_loadu_si128(
            (__m128i *)&src[(i + 1) * src_stride + (w - remain_wd)]);
      __m256i read1 =
          _mm256_insertf128_si256(_mm256_castsi128_si256(row0), row1, 0x1);
      read1 = _mm256_shuffle_epi8(
          read1, _mm256_loadu_si256((__m256i *)shuffle_mask1_loc_wd4));
      __m256i res = _mm256_madd_epi16(read1, coeffs_x_bilinear);
      res =
          _mm256_sra_epi32(_mm256_add_epi32(res, round_const_x), round_shift_x);
      row[1] = _mm256_packs_epi32(res, res);
      const __m256i s = _mm256_unpacklo_epi16(
          row[0], _mm256_permute2x128_si256(row[0], row[1], 0x21));
      const __m256i res_0 = _mm256_madd_epi16(s, coeffs_y_bilinear);
      const __m256i res_a_round = _mm256_sra_epi32(
          _mm256_add_epi32(res_0, round_const_y), round_shift_y);
      __m256i res_16bit = _mm256_packs_epi32(res_a_round, res_a_round);
      res_16bit = _mm256_min_epi16(res_16bit, clip_pixel);
      res_16bit = _mm256_max_epi16(res_16bit, zero);
      _mm_storel_epi64((__m128i *)&dst[(i - 2) * dst_stride + (w - remain_wd)],
                       _mm256_castsi256_si128(res_16bit));
      _mm_storel_epi64((__m128i *)&dst[(i - 1) * dst_stride + (w - remain_wd)],
                       _mm256_extracti128_si256(res_16bit, 1));
      row[0] = row[1];
    }
  } else {
    assert(0);
  }
}

#endif  // AVM_AV2_COMMON_X86_AV2_CONVOLVE_AVX2_H_
