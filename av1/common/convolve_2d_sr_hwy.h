/*
 * Copyright (c) 2026, Alliance for Open Media. All rights reserved.
 *
 * This source code is subject to the terms of the BSD 2 Clause License and
 * the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
 * was not distributed with this source code in the LICENSE file, you can
 * obtain it at www.aomedia.org/license/software. If the Alliance for Open
 * Media Patent License 1.0 was not distributed with this source code in the
 * PATENTS file, you can obtain it at www.aomedia.org/license/patent.
 */

#ifndef AOM_AV1_COMMON_CONVOLVE_2D_SR_HWY_H_
#define AOM_AV1_COMMON_CONVOLVE_2D_SR_HWY_H_

#include "av1/common/convolve.h"
#include "av1/common/filter.h"
#include "config/aom_config.h"
#include "config/av1_rtcd.h"
#include "third_party/highway/hwy/highway.h"

HWY_BEFORE_NAMESPACE();

namespace {
namespace HWY_NAMESPACE {

namespace hn = hwy::HWY_NAMESPACE;

HWY_ALIGN constexpr uint8_t kFilt1[16] = { 0, 1, 1, 2, 2, 3, 3, 4,
                                           4, 5, 5, 6, 6, 7, 7, 8 };
HWY_ALIGN constexpr uint8_t kFilt2[16] = { 2, 3, 3, 4, 4, 5, 5, 6,
                                           6, 7, 7, 8, 8, 9, 9, 10 };
HWY_ALIGN constexpr uint8_t kFilt3[16] = { 4, 5, 5, 6,  6,  7,  7,  8,
                                           8, 9, 9, 10, 10, 11, 11, 12 };
HWY_ALIGN constexpr uint8_t kFilt4[16] = { 6,  7,  7,  8,  8,  9,  9,  10,
                                           10, 11, 11, 12, 12, 13, 13, 14 };

// Horizontal convolve helpers
template <int taps, class D16, class V8, class VI8>
HWY_ATTR HWY_INLINE hn::VFromD<D16> ConvolveLowbdX(D16 d16, V8 data,
                                                   const V8 *masks,
                                                   const VI8 *coeffs) {
  constexpr int num_coeffs = taps / 2;
  hn::VFromD<D16> prods[4];
  for (int k = 0; k < num_coeffs; ++k) {
    auto s = hn::TableLookupBytes(data, masks[k]);
    prods[k] = hn::SatWidenMulPairwiseAdd(d16, s, coeffs[k]);
  }

  HWY_IF_CONSTEXPR(num_coeffs == 1) { return prods[0]; }
  HWY_IF_CONSTEXPR(num_coeffs == 2) { return hn::Add(prods[0], prods[1]); }
  HWY_IF_CONSTEXPR(num_coeffs == 3) {
    return hn::Add(hn::Add(prods[0], prods[1]), prods[2]);
  }
  HWY_IF_CONSTEXPR(num_coeffs == 4) {
    return hn::Add(hn::Add(prods[0], prods[1]), hn::Add(prods[2], prods[3]));
  }
}

template <int taps, class D16, class V8, class VI8>
HWY_ATTR HWY_INLINE hn::VFromD<D16> ConvolveLowbdXShuffleFree(
    D16 d16, V8 data, const VI8 *coeffs) {
  using D8 = hn::Repartition<uint8_t, D16>;
  const D8 d8;
  constexpr int num_coeffs = taps / 2;

  if (hn::Lanes(d16) > 8) {
    using D8_twice = hn::DFromV<V8>;
    const D8_twice d8_twice;

    hn::VFromD<D16> prods[4];
    auto mask1 = hn::LoadDup128(d8_twice, kFilt1);
    prods[0] = hn::SatWidenMulPairwiseAdd(
        d16, hn::TableLookupBytes(data, mask1), coeffs[0]);

    HWY_IF_CONSTEXPR(num_coeffs >= 2) {
      auto mask2 = hn::LoadDup128(d8_twice, kFilt2);
      prods[1] = hn::SatWidenMulPairwiseAdd(
          d16, hn::TableLookupBytes(data, mask2), coeffs[1]);
    }
    HWY_IF_CONSTEXPR(num_coeffs >= 3) {
      auto mask3 = hn::LoadDup128(d8_twice, kFilt3);
      prods[2] = hn::SatWidenMulPairwiseAdd(
          d16, hn::TableLookupBytes(data, mask3), coeffs[2]);
    }
    HWY_IF_CONSTEXPR(num_coeffs >= 4) {
      auto mask4 = hn::LoadDup128(d8_twice, kFilt4);
      prods[3] = hn::SatWidenMulPairwiseAdd(
          d16, hn::TableLookupBytes(data, mask4), coeffs[3]);
    }

    HWY_IF_CONSTEXPR(num_coeffs == 1) return prods[0];
    HWY_IF_CONSTEXPR(num_coeffs == 2) return hn::Add(prods[0], prods[1]);
    HWY_IF_CONSTEXPR(num_coeffs == 3) {
      return hn::Add(hn::Add(prods[0], prods[1]), prods[2]);
    }
    HWY_IF_CONSTEXPR(num_coeffs == 4) {
      return hn::Add(hn::Add(prods[0], prods[1]), hn::Add(prods[2], prods[3]));
    }
  }

  hn::VFromD<D16> prods[4];
  prods[0] = hn::SatWidenMulPairwiseAdd(
      d16, hn::InterleaveLower(d8, data, hn::ShiftRightBytes<1>(d8, data)),
      coeffs[0]);
  HWY_IF_CONSTEXPR(num_coeffs >= 2) {
    prods[1] = hn::SatWidenMulPairwiseAdd(
        d16,
        hn::InterleaveLower(d8, hn::ShiftRightBytes<2>(d8, data),
                            hn::ShiftRightBytes<3>(d8, data)),
        coeffs[1]);
  }
  HWY_IF_CONSTEXPR(num_coeffs >= 3) {
    prods[2] = hn::SatWidenMulPairwiseAdd(
        d16,
        hn::InterleaveLower(d8, hn::ShiftRightBytes<4>(d8, data),
                            hn::ShiftRightBytes<5>(d8, data)),
        coeffs[2]);
  }
  HWY_IF_CONSTEXPR(num_coeffs >= 4) {
    prods[3] = hn::SatWidenMulPairwiseAdd(
        d16,
        hn::InterleaveLower(d8, hn::ShiftRightBytes<6>(d8, data),
                            hn::ShiftRightBytes<7>(d8, data)),
        coeffs[3]);
  }

  HWY_IF_CONSTEXPR(num_coeffs == 1) return prods[0];
  HWY_IF_CONSTEXPR(num_coeffs == 2) return hn::Add(prods[0], prods[1]);
  HWY_IF_CONSTEXPR(num_coeffs == 3) {
    return hn::Add(hn::Add(prods[0], prods[1]), prods[2]);
  }
  HWY_IF_CONSTEXPR(num_coeffs == 4) {
    return hn::Add(hn::Add(prods[0], prods[1]), hn::Add(prods[2], prods[3]));
  }
}

template <class D16, class V8, class V16_coeff>
HWY_ATTR HWY_INLINE hn::VFromD<D16> ConvolveLowbdX12TapFromS(
    D16 d16, V8 s0, V8 s1, V8 s2, V8 s3, V8 s4, V8 s5, V16_coeff coeff0,
    V16_coeff coeff1, V16_coeff coeff2, V16_coeff coeff3, V16_coeff coeff4,
    V16_coeff coeff5) {
  using D8_twice = hn::DFromV<V8>;
  using D8 = hn::Half<D8_twice>;
  using D16_half = hn::Half<D16>;
  using D32 = hn::Rebind<int32_t, D16_half>;
  const D8 d8;
  const D32 d32;
  const D16_half d16_half;

  auto s0_low = hn::LowerHalf(s0);
  auto s0_high = hn::UpperHalf(d8, s0);
  auto s1_low = hn::LowerHalf(s1);
  auto s1_high = hn::UpperHalf(d8, s1);
  auto s2_low = hn::LowerHalf(s2);
  auto s2_high = hn::UpperHalf(d8, s2);
  auto s3_low = hn::LowerHalf(s3);
  auto s3_high = hn::UpperHalf(d8, s3);
  auto s4_low = hn::LowerHalf(s4);
  auto s4_high = hn::UpperHalf(d8, s4);
  auto s5_low = hn::LowerHalf(s5);
  auto s5_high = hn::UpperHalf(d8, s5);

  auto s0_low_16 = hn::PromoteTo(d16, s0_low);
  auto s0_high_16 = hn::PromoteTo(d16, s0_high);
  auto s1_low_16 = hn::PromoteTo(d16, s1_low);
  auto s1_high_16 = hn::PromoteTo(d16, s1_high);
  auto s2_low_16 = hn::PromoteTo(d16, s2_low);
  auto s2_high_16 = hn::PromoteTo(d16, s2_high);
  auto s3_low_16 = hn::PromoteTo(d16, s3_low);
  auto s3_high_16 = hn::PromoteTo(d16, s3_high);
  auto s4_low_16 = hn::PromoteTo(d16, s4_low);
  auto s4_high_16 = hn::PromoteTo(d16, s4_high);
  auto s5_low_16 = hn::PromoteTo(d16, s5_low);
  auto s5_high_16 = hn::PromoteTo(d16, s5_high);

  auto res0_low = hn::WidenMulPairwiseAdd(d32, s0_low_16, coeff0);
  auto res1_low = hn::WidenMulPairwiseAdd(d32, s1_low_16, coeff1);
  auto res2_low = hn::WidenMulPairwiseAdd(d32, s2_low_16, coeff2);
  auto res3_low = hn::WidenMulPairwiseAdd(d32, s3_low_16, coeff3);
  auto res4_low = hn::WidenMulPairwiseAdd(d32, s4_low_16, coeff4);
  auto res5_low = hn::WidenMulPairwiseAdd(d32, s5_low_16, coeff5);

  auto sum0_low = hn::Add(res0_low, res1_low);
  auto sum1_low = hn::Add(res2_low, res3_low);
  auto sum2_low = hn::Add(res4_low, res5_low);
  auto sum_low = hn::Add(hn::Add(sum0_low, sum1_low), sum2_low);

  auto res0_high = hn::WidenMulPairwiseAdd(d32, s0_high_16, coeff0);
  auto res1_high = hn::WidenMulPairwiseAdd(d32, s1_high_16, coeff1);
  auto res2_high = hn::WidenMulPairwiseAdd(d32, s2_high_16, coeff2);
  auto res3_high = hn::WidenMulPairwiseAdd(d32, s3_high_16, coeff3);
  auto res4_high = hn::WidenMulPairwiseAdd(d32, s4_high_16, coeff4);
  auto res5_high = hn::WidenMulPairwiseAdd(d32, s5_high_16, coeff5);

  auto sum0_high = hn::Add(res0_high, res1_high);
  auto sum1_high = hn::Add(res2_high, res3_high);
  auto sum2_high = hn::Add(res4_high, res5_high);
  auto sum_high = hn::Add(hn::Add(sum0_high, sum1_high), sum2_high);

  auto shifted_low = hn::ShiftRightSame(sum_low, 1);
  auto shifted_high = hn::ShiftRightSame(sum_high, 1);
  auto res_low_16 = hn::DemoteTo(d16_half, shifted_low);
  auto res_high_16 = hn::DemoteTo(d16_half, shifted_high);

  return hn::Combine(d16, res_high_16, res_low_16);
}

template <class D16, class V8, class V8_mask, class V16_coeff>
HWY_ATTR HWY_INLINE hn::VFromD<D16> ConvolveLowbdX12Tap(
    D16 d16, V8 data1, V8 data2, V8_mask mask1, V8_mask mask2, V8_mask mask3,
    V16_coeff coeff0, V16_coeff coeff1, V16_coeff coeff2, V16_coeff coeff3,
    V16_coeff coeff4, V16_coeff coeff5) {
  auto s0 = hn::TableLookupBytes(data1, mask1);
  auto s1 = hn::TableLookupBytes(data1, mask2);
  auto s2 = hn::TableLookupBytes(data1, mask3);
  auto s3 = hn::TableLookupBytes(data2, mask1);
  auto s4 = hn::TableLookupBytes(data2, mask2);
  auto s5 = hn::TableLookupBytes(data2, mask3);
  return ConvolveLowbdX12TapFromS(d16, s0, s1, s2, s3, s4, s5, coeff0, coeff1,
                                  coeff2, coeff3, coeff4, coeff5);
}

template <class D16, class V8, class V16_coeff>
HWY_ATTR HWY_INLINE hn::VFromD<D16> ConvolveLowbdX12TapShuffleFree(
    D16 d16, V8 data1, V8 data2, V16_coeff coeff0, V16_coeff coeff1,
    V16_coeff coeff2, V16_coeff coeff3, V16_coeff coeff4, V16_coeff coeff5) {
  if (hn::Lanes(d16) > 8) {
    using D8_twice = hn::DFromV<V8>;
    const D8_twice d8_twice;

    auto mask1 = hn::LoadDup128(d8_twice, kFilt1);
    auto mask2 = hn::LoadDup128(d8_twice, kFilt2);
    auto mask3 = hn::LoadDup128(d8_twice, kFilt3);
    return ConvolveLowbdX12Tap(d16, data1, data2, mask1, mask2, mask3, coeff0,
                               coeff1, coeff2, coeff3, coeff4, coeff5);
  }

  using D8_twice = hn::DFromV<V8>;
  const D8_twice d8_twice;
  auto s0 = hn::InterleaveLower(d8_twice, data1,
                                hn::ShiftRightBytes<1>(d8_twice, data1));
  auto s1 =
      hn::InterleaveLower(d8_twice, hn::ShiftRightBytes<2>(d8_twice, data1),
                          hn::ShiftRightBytes<3>(d8_twice, data1));
  auto s2 =
      hn::InterleaveLower(d8_twice, hn::ShiftRightBytes<4>(d8_twice, data1),
                          hn::ShiftRightBytes<5>(d8_twice, data1));
  auto s3 = hn::InterleaveLower(d8_twice, data2,
                                hn::ShiftRightBytes<1>(d8_twice, data2));
  auto s4 =
      hn::InterleaveLower(d8_twice, hn::ShiftRightBytes<2>(d8_twice, data2),
                          hn::ShiftRightBytes<3>(d8_twice, data2));
  auto s5 =
      hn::InterleaveLower(d8_twice, hn::ShiftRightBytes<4>(d8_twice, data2),
                          hn::ShiftRightBytes<5>(d8_twice, data2));
  return ConvolveLowbdX12TapFromS(d16, s0, s1, s2, s3, s4, s5, coeff0, coeff1,
                                  coeff2, coeff3, coeff4, coeff5);
}

// Vertical convolve helper
template <int taps_y, class D32, class V16, class V16_coeff>
HWY_ATTR HWY_INLINE hn::VFromD<D32> ConvolveVertical(
    D32 d32, const V16 *z_arr, const V16_coeff *coeffs_v,
    hn::VFromD<D32> round_const_y) {
  constexpr int num_coeffs = taps_y / 2;
  auto sum = hn::SatWidenMulPairwiseAccumulate(d32, z_arr[0], coeffs_v[0],
                                               round_const_y);
  for (int k = 1; k < num_coeffs; ++k) {
    sum = hn::SatWidenMulPairwiseAccumulate(d32, z_arr[k], coeffs_v[k], sum);
  }
  return sum;
}

template <class DI8>
HWY_ATTR HWY_INLINE void PrepareCoeffsH(DI8 di8, const int16_t *filter,
                                        int taps, hn::VFromD<DI8> *coeffs) {
  int start_idx = (taps == 12) ? 0 : (4 - taps / 2);
  int num_coeffs = taps / 2;
  for (int k = 0; k < num_coeffs; ++k) {
    auto c0 = static_cast<int8_t>(filter[start_idx + k * 2] >> 1);
    auto c1 = static_cast<int8_t>(filter[start_idx + k * 2 + 1] >> 1);
    HWY_ALIGN int8_t coeff_arr[16] = { c0, c1, c0, c1, c0, c1, c0, c1,
                                       c0, c1, c0, c1, c0, c1, c0, c1 };
    coeffs[k] = hn::LoadDup128(di8, coeff_arr);
  }
}

template <class D16>
HWY_ATTR HWY_INLINE void PrepareCoeffsV(D16 d16, const int16_t *filter,
                                        int taps, hn::VFromD<D16> *coeffs) {
  int start_idx = (taps == 12) ? 0 : (4 - taps / 2);
  int num_coeffs = taps / 2;
  for (int k = 0; k < num_coeffs; ++k) {
    int16_t c0 = filter[start_idx + k * 2];
    int16_t c1 = filter[start_idx + k * 2 + 1];
    HWY_ALIGN int16_t coeff_arr[8] = { c0, c1, c0, c1, c0, c1, c0, c1 };
    coeffs[k] = hn::LoadDup128(d16, coeff_arr);
  }
}

template <class D16>
HWY_ATTR HWY_INLINE void PrepareCoeffs12(D16 d16, const int16_t *filter,
                                         hn::VFromD<D16> *coeffs) {
  for (int k = 0; k < 6; ++k) {
    int16_t c0 = filter[k * 2];
    int16_t c1 = filter[k * 2 + 1];
    HWY_ALIGN int16_t coeff_arr[8] = { c0, c1, c0, c1, c0, c1, c0, c1 };
    coeffs[k] = hn::LoadDup128(d16, coeff_arr);
  }
}

template <int num_rows, class D, class D128>
HWY_ATTR HWY_INLINE hn::VFromD<D> LoadAndCombine(D d, D128 d128,
                                                 const uint8_t *src,
                                                 int src_stride) {
  HWY_IF_CONSTEXPR(num_rows == 4) {
    auto r0 = hn::LoadU(d128, src + 0 * src_stride);
    auto r1 = hn::LoadU(d128, src + 1 * src_stride);
    auto r2 = hn::LoadU(d128, src + 2 * src_stride);
    auto r3 = hn::LoadU(d128, src + 3 * src_stride);

    using D256 = hn::Half<D>;
    const D256 d256;
    using D128_dyn = hn::Half<D256>;

    auto r01 = hn::Combine(d256, hn::BitCast(D128_dyn(), r1),
                           hn::BitCast(D128_dyn(), r0));
    auto r23 = hn::Combine(d256, hn::BitCast(D128_dyn(), r3),
                           hn::BitCast(D128_dyn(), r2));
    return hn::Combine(d, r23, r01);
  }
  HWY_IF_CONSTEXPR(num_rows == 2) {
    auto r0 = hn::LoadU(d128, src + 0 * src_stride);
    auto r1 = hn::LoadU(d128, src + 1 * src_stride);

    using D128_dyn = hn::Half<D>;
    return hn::Combine(d, hn::BitCast(D128_dyn(), r1),
                       hn::BitCast(D128_dyn(), r0));
  }
  return hn::ResizeBitCast(d, hn::LoadU(d128, src));
}

template <int taps_x_const = 0, int taps_y_const = 0, int round_0_const = 0,
          int round_1_const = 0>
HWY_ATTR HWY_INLINE void Convolve2DSRHwyImpl(
    const uint8_t *src, int src_stride, uint8_t *dst, int dst_stride, int w,
    int h, const InterpFilterParams *filter_params_x,
    const InterpFilterParams *filter_params_y, const int subpel_x_qn,
    const int subpel_y_qn, ConvolveParams *conv_params) {
  const int taps_x = taps_x_const > 0 ? taps_x_const : filter_params_x->taps;
  const int taps_y = taps_y_const > 0 ? taps_y_const : filter_params_y->taps;
  const int round_0 = round_0_const > 0 ? round_0_const : conv_params->round_0;
  const int round_1 = round_1_const > 0 ? round_1_const : conv_params->round_1;

  const bool is_taps_x_12 =
      (taps_x_const == 12) || (taps_x_const == 0 && taps_x == 12);
  const bool is_taps_y_12 =
      (taps_y_const == 12) || (taps_y_const == 0 && taps_y == 12);

  const int fo_vert = taps_y / 2 - 1;
  const int fo_horiz = taps_x / 2 - 1;

  constexpr int kNumRows = 4;
  HWY_ALIGN_MAX int16_t strip_im_buf[(MAX_SB_SIZE + MAX_FILTER_TAP + 8) * 8];

  const int16_t *x_filter = av1_get_interp_filter_subpel_kernel(
      filter_params_x, subpel_x_qn & SUBPEL_MASK);
  const int16_t *y_filter = av1_get_interp_filter_subpel_kernel(
      filter_params_y, subpel_y_qn & SUBPEL_MASK);

  const hn::ScalableTag<int16_t> d16_h;
  using D8_h = hn::Repartition<uint8_t, decltype(d16_h)>;
  using DI8_h = hn::Repartition<int8_t, decltype(d16_h)>;

  hn::VFromD<DI8_h> coeffs_h[6] = { hn::Zero(DI8_h()) };
  hn::VFromD<decltype(d16_h)> coeffs_h_12[6];
  if (is_taps_x_12) {
    PrepareCoeffsV(d16_h, x_filter, taps_x, coeffs_h_12);
  } else {
    PrepareCoeffsH(DI8_h(), x_filter, taps_x, coeffs_h);
  }

  decltype(hn::LoadDup128(D8_h(), kFilt1)) mask1, mask2, mask3, mask4;
  HWY_IF_CONSTEXPR(taps_x_const != 8 && taps_x_const != 12) {
    mask1 = hn::LoadDup128(D8_h(), kFilt1);
    mask2 = hn::LoadDup128(D8_h(), kFilt2);
    mask3 = hn::LoadDup128(D8_h(), kFilt3);
    mask4 = hn::LoadDup128(D8_h(), kFilt4);
  }

  const auto round_const_h = hn::Set(d16_h, 1 << (round_0 - 2));

  const int im_h = h + taps_y - 1;
  const uint8_t *const src_ptr = src - fo_vert * src_stride - fo_horiz;

  auto convolve_horizontal = [&](auto data_vec) {
    hn::VFromD<decltype(d16_h)> res;
    HWY_IF_CONSTEXPR(taps_x_const == 2 || taps_x_const == 4 ||
                     taps_x_const == 6 || taps_x_const == 8) {
      res = ConvolveLowbdXShuffleFree<taps_x_const>(d16_h, data_vec, coeffs_h);
    }
    else HWY_IF_CONSTEXPR(taps_x_const == 0) {
      if (taps_x == 2) {
        res = ConvolveLowbdX<2>(d16_h, data_vec, &mask1, coeffs_h);
      } else if (taps_x == 4) {
        const decltype(mask1) masks[] = { mask1, mask2 };
        res = ConvolveLowbdX<4>(d16_h, data_vec, masks, coeffs_h);
      } else if (taps_x == 6) {
        const decltype(mask1) masks[] = { mask1, mask2, mask3 };
        res = ConvolveLowbdX<6>(d16_h, data_vec, masks, coeffs_h);
      } else {
        const decltype(mask1) masks[] = { mask1, mask2, mask3, mask4 };
        res = ConvolveLowbdX<8>(d16_h, data_vec, masks, coeffs_h);
      }
    }
    return res;
  };

  for (int j = 0; j < w; j += 8) {
    const bool skip_strip_im_buf =
        (is_taps_y_12 || is_taps_x_12) ? false : (h == 4 || h == 8);

    if (!skip_strip_im_buf) {
      for (int i = 0; i < im_h; i += kNumRows) {
        const hn::CappedTag<uint8_t, 16> d128;

        hn::VFromD<decltype(d16_h)> res;
        if (is_taps_x_12) {
          auto data1 = LoadAndCombine<kNumRows>(
              D8_h(), d128, src_ptr + i * src_stride + j + 0, src_stride);
          auto data2 = LoadAndCombine<kNumRows>(
              D8_h(), d128, src_ptr + i * src_stride + j + 6, src_stride);
          HWY_IF_CONSTEXPR(taps_x_const == 12) {
            res = ConvolveLowbdX12TapShuffleFree(
                d16_h, data1, data2, coeffs_h_12[0], coeffs_h_12[1],
                coeffs_h_12[2], coeffs_h_12[3], coeffs_h_12[4], coeffs_h_12[5]);
          }
          else {
            res = ConvolveLowbdX12Tap(d16_h, data1, data2, mask1, mask2, mask3,
                                      coeffs_h_12[0], coeffs_h_12[1],
                                      coeffs_h_12[2], coeffs_h_12[3],
                                      coeffs_h_12[4], coeffs_h_12[5]);
          }
        } else {
          auto data = LoadAndCombine<kNumRows>(
              D8_h(), d128, src_ptr + i * src_stride + j, src_stride);
          res = convolve_horizontal(data);
        }

        auto shifted_res =
            hn::ShiftRightSame(hn::Add(res, round_const_h), round_0 - 1);
        hn::StoreU(shifted_res, d16_h, strip_im_buf + i * 8);
      }
    }

    const int bits = FILTER_BITS * 2 - round_0 - round_1;

    const hn::ScalableTag<int16_t> d16_32;
    const hn::ScalableTag<int32_t> d32_16;
    const hn::CappedTag<uint8_t, 16> d8_16;
    const hn::CappedTag<uint8_t, 8> d8_8;
    const hn::ScalableTag<uint32_t> du32_16;
    const auto zero_32 = hn::Zero(d32_16);

    hn::VFromD<decltype(d16_32)> coeffs_v[6];
    HWY_IF_CONSTEXPR(taps_y_const > 0) {
      PrepareCoeffsV(d16_32, y_filter, taps_y_const, coeffs_v);
    }
    else {
      PrepareCoeffsV(d16_32, y_filter, taps_y, coeffs_v);
    }

    const auto round_const_y = [&]() {
      const int round_const_y_val =
          ((1 << round_1) >> 1) +
          ((bits > 0) ? (1 << (bits - 1 + round_1)) : 0);
      return hn::Set(d32_16, round_const_y_val);
    }();

    HWY_ALIGN_MAX static constexpr int16_t idx_arr[32] = {
      0, 8,  1, 9,  2,  10, 3,  11, 4,  12, 5,  13, 6,  14, 7,  15,
      8, 16, 9, 17, 10, 18, 11, 19, 12, 20, 13, 21, 14, 22, 15, 23
    };
    auto idx_tbl = hn::SetTableIndices(d16_32, idx_arr);

    uint8_t *dst_ptr = dst + j;

    auto round_and_store = [&](hn::VFromD<decltype(d32_16)> sum, int y) {
      auto res_32_A = hn::ShiftRightSame(sum, round_1);
      HWY_IF_CONSTEXPR(round_0_const > 0 && round_1_const > 0) {
        constexpr int bits_const =
            FILTER_BITS * 2 - round_0_const - round_1_const;
        HWY_IF_CONSTEXPR(bits_const > 0) {
          res_32_A = hn::ShiftRightSame(res_32_A, bits_const);
        }
      }
      else {
        if (bits > 0) {
          res_32_A = hn::ShiftRightSame(res_32_A, bits);
        }
      }

      auto max_32 = hn::Max(res_32_A, zero_32);
      auto res_8_A = hn::DemoteTo(d8_16, hn::BitCast(du32_16, max_32));

      if (w == 4) {
        uint8_t tmpA[16];
        hn::StoreU(hn::LowerHalf(d8_8, res_8_A), d8_8, tmpA);
        hn::StoreU(hn::UpperHalf(d8_8, res_8_A), d8_8, tmpA + 8);
        __builtin_memcpy(dst_ptr + (y + 0) * dst_stride, tmpA, 4);
        __builtin_memcpy(dst_ptr + (y + 1) * dst_stride, tmpA + 8, 4);
      } else {
        hn::StoreU(hn::LowerHalf(d8_8, res_8_A), d8_8,
                   dst_ptr + (y + 0) * dst_stride);
        hn::StoreU(hn::UpperHalf(d8_8, res_8_A), d8_8,
                   dst_ptr + (y + 1) * dst_stride);
      }
    };

    auto compute_h_row_block = [&](int row_idx) {
      const hn::CappedTag<uint8_t, 16> d128;
      auto data = LoadAndCombine<4>(
          D8_h(), d128, src_ptr + row_idx * src_stride + j, src_stride);
      auto res = convolve_horizontal(data);
      return hn::ShiftRightSame(hn::Add(res, round_const_h), round_0 - 1);
    };

    if (h == 4 && !is_taps_x_12) {
      auto H0 = compute_h_row_block(0);
      auto H1 = compute_h_row_block(4);
      hn::Half<decltype(d16_32)> d16_16;

      auto L0 = H0;
      auto L2 = hn::Combine(d16_32, hn::LowerHalf(d16_16, H1),
                            hn::UpperHalf(d16_16, H0));

      HWY_IF_CONSTEXPR(taps_y_const > 0) {
        constexpr int num_coeffs = taps_y_const / 2;
        constexpr int num_z = 4 / 2 + num_coeffs - 1;
        constexpr int num_h = num_z / 2 + 1;

        hn::VFromD<decltype(d16_h)> H[num_h];
        if (skip_strip_im_buf) {
          for (int k = 0; k < num_h; ++k) {
            H[k] = compute_h_row_block(k * 4);
          }
        } else {
          for (int k = 0; k < num_h; ++k) {
            H[k] = hn::LoadU(d16_h, strip_im_buf + k * 32);
          }
        }

        hn::VFromD<decltype(d16_32)> Z[num_z];
        for (int z_idx = 0; z_idx < num_z; ++z_idx) {
          hn::VFromD<decltype(d16_32)> L;
          if (z_idx % 2 == 0) {
            L = H[z_idx / 2];
          } else {
            L = hn::Combine(d16_32, hn::LowerHalf(d16_16, H[z_idx / 2 + 1]),
                            hn::UpperHalf(d16_16, H[z_idx / 2]));
          }
          Z[z_idx] = hn::TableLookupLanes(L, idx_tbl);
        }

        for (int i = 0; i < 4 / 2; ++i) {
          auto sum = ConvolveVertical<taps_y_const>(d32_16, &Z[i], coeffs_v,
                                                    round_const_y);
          round_and_store(sum, 2 * i);
        }
      }
      else {
        if (taps_y == 2) {
          auto z01_12 = hn::TableLookupLanes(L0, idx_tbl);
          auto z23_34 = hn::TableLookupLanes(L2, idx_tbl);

          auto sumA = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z01_12, coeffs_v[0], round_const_y);
          auto sumB = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z23_34, coeffs_v[0], round_const_y);

          round_and_store(sumA, 0);
          round_and_store(sumB, 2);
        } else if (taps_y == 4) {
          auto L4 = H1;
          auto z01_12 = hn::TableLookupLanes(L0, idx_tbl);
          auto z23_34 = hn::TableLookupLanes(L2, idx_tbl);
          auto z45_56 = hn::TableLookupLanes(L4, idx_tbl);

          auto sumA = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z01_12, coeffs_v[0], round_const_y);
          sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z23_34, coeffs_v[1],
                                                   sumA);

          auto sumB = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z23_34, coeffs_v[0], round_const_y);
          sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56, coeffs_v[1],
                                                   sumB);

          round_and_store(sumA, 0);
          round_and_store(sumB, 2);
        } else if (taps_y == 6) {
          auto H2 = compute_h_row_block(8);
          auto L4 = H1;
          auto L6 = hn::Combine(d16_32, hn::LowerHalf(d16_16, H2),
                                hn::UpperHalf(d16_16, H1));

          auto z01_12 = hn::TableLookupLanes(L0, idx_tbl);
          auto z23_34 = hn::TableLookupLanes(L2, idx_tbl);
          auto z45_56 = hn::TableLookupLanes(L4, idx_tbl);
          auto z67_78 = hn::TableLookupLanes(L6, idx_tbl);

          auto sumA = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z01_12, coeffs_v[0], round_const_y);
          sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z23_34, coeffs_v[1],
                                                   sumA);
          sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56, coeffs_v[2],
                                                   sumA);

          auto sumB = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z23_34, coeffs_v[0], round_const_y);
          sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56, coeffs_v[1],
                                                   sumB);
          sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78, coeffs_v[2],
                                                   sumB);

          round_and_store(sumA, 0);
          round_and_store(sumB, 2);
        } else {
          auto H2 = compute_h_row_block(8);
          auto L4 = H1;
          auto L6 = hn::Combine(d16_32, hn::LowerHalf(d16_16, H2),
                                hn::UpperHalf(d16_16, H1));
          auto L8 = H2;

          auto z01_12 = hn::TableLookupLanes(L0, idx_tbl);
          auto z23_34 = hn::TableLookupLanes(L2, idx_tbl);
          auto z45_56 = hn::TableLookupLanes(L4, idx_tbl);
          auto z67_78 = hn::TableLookupLanes(L6, idx_tbl);
          auto z89_910 = hn::TableLookupLanes(L8, idx_tbl);

          auto sumA = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z01_12, coeffs_v[0], round_const_y);
          sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z23_34, coeffs_v[1],
                                                   sumA);
          sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56, coeffs_v[2],
                                                   sumA);
          sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78, coeffs_v[3],
                                                   sumA);

          auto sumB = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z23_34, coeffs_v[0], round_const_y);
          sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56, coeffs_v[1],
                                                   sumB);
          sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78, coeffs_v[2],
                                                   sumB);
          sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z89_910, coeffs_v[3],
                                                   sumB);

          round_and_store(sumA, 0);
          round_and_store(sumB, 2);
        }
      }
    } else if (h == 8 && !is_taps_x_12) {
      auto H0 = compute_h_row_block(0);
      auto H1 = compute_h_row_block(4);
      auto H2 = compute_h_row_block(8);
      hn::Half<decltype(d16_32)> d16_16;

      auto L0 = H0;
      auto L2 = hn::Combine(d16_32, hn::LowerHalf(d16_16, H1),
                            hn::UpperHalf(d16_16, H0));
      auto L4 = H1;
      auto L6 = hn::Combine(d16_32, hn::LowerHalf(d16_16, H2),
                            hn::UpperHalf(d16_16, H1));

      HWY_IF_CONSTEXPR(taps_y_const > 0) {
        constexpr int num_coeffs = taps_y_const / 2;
        constexpr int num_z = 8 / 2 + num_coeffs - 1;
        constexpr int num_h = num_z / 2 + 1;

        hn::VFromD<decltype(d16_h)> H[num_h];
        if (skip_strip_im_buf) {
          for (int k = 0; k < num_h; ++k) {
            H[k] = compute_h_row_block(k * 4);
          }
        } else {
          for (int k = 0; k < num_h; ++k) {
            H[k] = hn::LoadU(d16_h, strip_im_buf + k * 32);
          }
        }

        hn::VFromD<decltype(d16_32)> Z[num_z];
        for (int z_idx = 0; z_idx < num_z; ++z_idx) {
          hn::VFromD<decltype(d16_32)> L;
          if (z_idx % 2 == 0) {
            L = H[z_idx / 2];
          } else {
            L = hn::Combine(d16_32, hn::LowerHalf(d16_16, H[z_idx / 2 + 1]),
                            hn::UpperHalf(d16_16, H[z_idx / 2]));
          }
          Z[z_idx] = hn::TableLookupLanes(L, idx_tbl);
        }

        for (int i = 0; i < 8 / 2; ++i) {
          auto sum = ConvolveVertical<taps_y_const>(d32_16, &Z[i], coeffs_v,
                                                    round_const_y);
          round_and_store(sum, 2 * i);
        }
      }
      else {
        if (taps_y == 2) {
          auto z01_12 = hn::TableLookupLanes(L0, idx_tbl);
          auto z23_34 = hn::TableLookupLanes(L2, idx_tbl);
          auto z45_56 = hn::TableLookupLanes(L4, idx_tbl);
          auto z67_78 = hn::TableLookupLanes(L6, idx_tbl);

          auto sumA = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z01_12, coeffs_v[0], round_const_y);
          auto sumB = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z23_34, coeffs_v[0], round_const_y);
          auto sumC = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z45_56, coeffs_v[0], round_const_y);
          auto sumD = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z67_78, coeffs_v[0], round_const_y);

          round_and_store(sumA, 0);
          round_and_store(sumB, 2);
          round_and_store(sumC, 4);
          round_and_store(sumD, 6);
        } else if (taps_y == 4) {
          auto L8 = H2;
          auto z01_12 = hn::TableLookupLanes(L0, idx_tbl);
          auto z23_34 = hn::TableLookupLanes(L2, idx_tbl);
          auto z45_56 = hn::TableLookupLanes(L4, idx_tbl);
          auto z67_78 = hn::TableLookupLanes(L6, idx_tbl);
          auto z89_910 = hn::TableLookupLanes(L8, idx_tbl);

          auto sumA = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z01_12, coeffs_v[0], round_const_y);
          sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z23_34, coeffs_v[1],
                                                   sumA);

          auto sumB = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z23_34, coeffs_v[0], round_const_y);
          sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56, coeffs_v[1],
                                                   sumB);

          auto sumC = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z45_56, coeffs_v[0], round_const_y);
          sumC = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78, coeffs_v[1],
                                                   sumC);

          auto sumD = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z67_78, coeffs_v[0], round_const_y);
          sumD = hn::SatWidenMulPairwiseAccumulate(d32_16, z89_910, coeffs_v[1],
                                                   sumD);

          round_and_store(sumA, 0);
          round_and_store(sumB, 2);
          round_and_store(sumC, 4);
          round_and_store(sumD, 6);
        } else if (taps_y == 6) {
          auto H3 = compute_h_row_block(12);
          auto L8 = H2;
          auto L10 = hn::Combine(d16_32, hn::LowerHalf(d16_16, H3),
                                 hn::UpperHalf(d16_16, H2));

          auto z01_12 = hn::TableLookupLanes(L0, idx_tbl);
          auto z23_34 = hn::TableLookupLanes(L2, idx_tbl);
          auto z45_56 = hn::TableLookupLanes(L4, idx_tbl);
          auto z67_78 = hn::TableLookupLanes(L6, idx_tbl);
          auto z89_910 = hn::TableLookupLanes(L8, idx_tbl);
          auto z1011_1112 = hn::TableLookupLanes(L10, idx_tbl);

          auto sumA = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z01_12, coeffs_v[0], round_const_y);
          sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z23_34, coeffs_v[1],
                                                   sumA);
          sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56, coeffs_v[2],
                                                   sumA);

          auto sumB = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z23_34, coeffs_v[0], round_const_y);
          sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56, coeffs_v[1],
                                                   sumB);
          sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78, coeffs_v[2],
                                                   sumB);

          auto sumC = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z45_56, coeffs_v[0], round_const_y);
          sumC = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78, coeffs_v[1],
                                                   sumC);
          sumC = hn::SatWidenMulPairwiseAccumulate(d32_16, z89_910, coeffs_v[2],
                                                   sumC);

          auto sumD = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z67_78, coeffs_v[0], round_const_y);
          sumD = hn::SatWidenMulPairwiseAccumulate(d32_16, z89_910, coeffs_v[1],
                                                   sumD);
          sumD = hn::SatWidenMulPairwiseAccumulate(d32_16, z1011_1112,
                                                   coeffs_v[2], sumD);

          round_and_store(sumA, 0);
          round_and_store(sumB, 2);
          round_and_store(sumC, 4);
          round_and_store(sumD, 6);
        } else {
          auto H3 = compute_h_row_block(12);
          auto L8 = H2;
          auto L10 = hn::Combine(d16_32, hn::LowerHalf(d16_16, H3),
                                 hn::UpperHalf(d16_16, H2));
          auto L12 = H3;

          auto z01_12 = hn::TableLookupLanes(L0, idx_tbl);
          auto z23_34 = hn::TableLookupLanes(L2, idx_tbl);
          auto z45_56 = hn::TableLookupLanes(L4, idx_tbl);
          auto z67_78 = hn::TableLookupLanes(L6, idx_tbl);
          auto z89_910 = hn::TableLookupLanes(L8, idx_tbl);
          auto z1011_1112 = hn::TableLookupLanes(L10, idx_tbl);
          auto z1213_1314 = hn::TableLookupLanes(L12, idx_tbl);

          auto sumA = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z01_12, coeffs_v[0], round_const_y);
          sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z23_34, coeffs_v[1],
                                                   sumA);
          sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56, coeffs_v[2],
                                                   sumA);
          sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78, coeffs_v[3],
                                                   sumA);

          auto sumB = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z23_34, coeffs_v[0], round_const_y);
          sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56, coeffs_v[1],
                                                   sumB);
          sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78, coeffs_v[2],
                                                   sumB);
          sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z89_910, coeffs_v[3],
                                                   sumB);

          auto sumC = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z45_56, coeffs_v[0], round_const_y);
          sumC = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78, coeffs_v[1],
                                                   sumC);
          sumC = hn::SatWidenMulPairwiseAccumulate(d32_16, z89_910, coeffs_v[2],
                                                   sumC);
          sumC = hn::SatWidenMulPairwiseAccumulate(d32_16, z1011_1112,
                                                   coeffs_v[3], sumC);

          auto sumD = hn::SatWidenMulPairwiseAccumulate(
              d32_16, z67_78, coeffs_v[0], round_const_y);
          sumD = hn::SatWidenMulPairwiseAccumulate(d32_16, z89_910, coeffs_v[1],
                                                   sumD);
          sumD = hn::SatWidenMulPairwiseAccumulate(d32_16, z1011_1112,
                                                   coeffs_v[2], sumD);
          sumD = hn::SatWidenMulPairwiseAccumulate(d32_16, z1213_1314,
                                                   coeffs_v[3], sumD);

          round_and_store(sumA, 0);
          round_and_store(sumB, 2);
          round_and_store(sumC, 4);
          round_and_store(sumD, 6);
        }
      }
    } else {
      if (h >= 8) {
        if (is_taps_y_12) {
          auto L0 = hn::LoadU(d16_32, strip_im_buf + 0 * 8);
          auto L2 = hn::LoadU(d16_32, strip_im_buf + 2 * 8);
          auto L4 = hn::LoadU(d16_32, strip_im_buf + 4 * 8);
          auto L6 = hn::LoadU(d16_32, strip_im_buf + 6 * 8);
          auto L8 = hn::LoadU(d16_32, strip_im_buf + 8 * 8);

          auto z01_12 = hn::TableLookupLanes(L0, idx_tbl);
          auto z23_34 = hn::TableLookupLanes(L2, idx_tbl);
          auto z45_56 = hn::TableLookupLanes(L4, idx_tbl);
          auto z67_78 = hn::TableLookupLanes(L6, idx_tbl);
          auto z89_910 = hn::TableLookupLanes(L8, idx_tbl);

          for (int y = 0; y < h; y += 8) {
            auto L10 = hn::LoadU(d16_32, strip_im_buf + (y + 10) * 8);
            auto L12 = hn::LoadU(d16_32, strip_im_buf + (y + 12) * 8);
            auto L14 = hn::LoadU(d16_32, strip_im_buf + (y + 14) * 8);
            auto L16 = hn::LoadU(d16_32, strip_im_buf + (y + 16) * 8);

            auto z1011_1112 = hn::TableLookupLanes(L10, idx_tbl);
            auto z1213_1314 = hn::TableLookupLanes(L12, idx_tbl);
            auto z1415_1516 = hn::TableLookupLanes(L14, idx_tbl);
            auto z1617_1718 = hn::TableLookupLanes(L16, idx_tbl);

            auto sumA = hn::SatWidenMulPairwiseAccumulate(
                d32_16, z01_12, coeffs_v[0], round_const_y);
            sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z23_34,
                                                     coeffs_v[1], sumA);
            sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56,
                                                     coeffs_v[2], sumA);
            sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78,
                                                     coeffs_v[3], sumA);
            sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z89_910,
                                                     coeffs_v[4], sumA);
            sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z1011_1112,
                                                     coeffs_v[5], sumA);

            auto sumB = hn::SatWidenMulPairwiseAccumulate(
                d32_16, z23_34, coeffs_v[0], round_const_y);
            sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56,
                                                     coeffs_v[1], sumB);
            sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78,
                                                     coeffs_v[2], sumB);
            sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z89_910,
                                                     coeffs_v[3], sumB);
            sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z1011_1112,
                                                     coeffs_v[4], sumB);
            sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z1213_1314,
                                                     coeffs_v[5], sumB);

            auto sumC = hn::SatWidenMulPairwiseAccumulate(
                d32_16, z45_56, coeffs_v[0], round_const_y);
            sumC = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78,
                                                     coeffs_v[1], sumC);
            sumC = hn::SatWidenMulPairwiseAccumulate(d32_16, z89_910,
                                                     coeffs_v[2], sumC);
            sumC = hn::SatWidenMulPairwiseAccumulate(d32_16, z1011_1112,
                                                     coeffs_v[3], sumC);
            sumC = hn::SatWidenMulPairwiseAccumulate(d32_16, z1213_1314,
                                                     coeffs_v[4], sumC);
            sumC = hn::SatWidenMulPairwiseAccumulate(d32_16, z1415_1516,
                                                     coeffs_v[5], sumC);

            auto sumD = hn::SatWidenMulPairwiseAccumulate(
                d32_16, z67_78, coeffs_v[0], round_const_y);
            sumD = hn::SatWidenMulPairwiseAccumulate(d32_16, z89_910,
                                                     coeffs_v[1], sumD);
            sumD = hn::SatWidenMulPairwiseAccumulate(d32_16, z1011_1112,
                                                     coeffs_v[2], sumD);
            sumD = hn::SatWidenMulPairwiseAccumulate(d32_16, z1213_1314,
                                                     coeffs_v[3], sumD);
            sumD = hn::SatWidenMulPairwiseAccumulate(d32_16, z1415_1516,
                                                     coeffs_v[4], sumD);
            sumD = hn::SatWidenMulPairwiseAccumulate(d32_16, z1617_1718,
                                                     coeffs_v[5], sumD);

            round_and_store(sumA, y);
            round_and_store(sumB, y + 2);
            round_and_store(sumC, y + 4);
            round_and_store(sumD, y + 6);

            z01_12 = z89_910;
            z23_34 = z1011_1112;
            z45_56 = z1213_1314;
            z67_78 = z1415_1516;
            z89_910 = z1617_1718;
          }
        } else {
          HWY_IF_CONSTEXPR(taps_y_const > 0) {
            HWY_IF_CONSTEXPR(taps_y_const == 2) {
              hn::Half<decltype(d16_32)> d16_16;
              auto L0 = hn::LoadU(d16_32, strip_im_buf + 0 * 8);
              for (int y = 0; y < h; y += 8) {
                auto L4 = hn::LoadU(d16_32, strip_im_buf + (y + 4) * 8);
                auto L8 = hn::LoadU(d16_32, strip_im_buf + (y + 8) * 8);
                auto L2 = hn::Combine(d16_32, hn::LowerHalf(d16_16, L4),
                                      hn::UpperHalf(d16_16, L0));
                auto L6 = hn::Combine(d16_32, hn::LowerHalf(d16_16, L8),
                                      hn::UpperHalf(d16_16, L4));

                auto z01_12 = hn::TableLookupLanes(L0, idx_tbl);
                auto z23_34 = hn::TableLookupLanes(L2, idx_tbl);
                auto z45_56 = hn::TableLookupLanes(L4, idx_tbl);
                auto z67_78 = hn::TableLookupLanes(L6, idx_tbl);

                auto sumA = hn::SatWidenMulPairwiseAccumulate(
                    d32_16, z01_12, coeffs_v[0], round_const_y);
                auto sumB = hn::SatWidenMulPairwiseAccumulate(
                    d32_16, z23_34, coeffs_v[0], round_const_y);
                auto sumC = hn::SatWidenMulPairwiseAccumulate(
                    d32_16, z45_56, coeffs_v[0], round_const_y);
                auto sumD = hn::SatWidenMulPairwiseAccumulate(
                    d32_16, z67_78, coeffs_v[0], round_const_y);

                round_and_store(sumA, y);
                round_and_store(sumB, y + 2);
                round_and_store(sumC, y + 4);
                round_and_store(sumD, y + 6);

                L0 = L8;
              }
            }
            else HWY_IF_CONSTEXPR(taps_y_const == 4) {
              auto L0 = hn::LoadU(d16_32, strip_im_buf + 0 * 8);
              auto z01_12 = hn::TableLookupLanes(L0, idx_tbl);
              auto L2 = hn::LoadU(d16_32, strip_im_buf + 2 * 8);
              auto z23_34 = hn::TableLookupLanes(L2, idx_tbl);
              hn::Half<decltype(d16_32)> d16_16;

              for (int y = 0; y < h; y += 8) {
                auto L4 = hn::LoadU(d16_32, strip_im_buf + (y + 4) * 8);
                auto L6 = hn::LoadU(d16_32, strip_im_buf + (y + 6) * 8);
                auto L10 = hn::LoadU(d16_32, strip_im_buf + (y + 10) * 8);
                auto L8 = hn::Combine(d16_32, hn::LowerHalf(d16_16, L10),
                                      hn::UpperHalf(d16_16, L6));

                auto z45_56 = hn::TableLookupLanes(L4, idx_tbl);
                auto z67_78 = hn::TableLookupLanes(L6, idx_tbl);
                auto z89_910 = hn::TableLookupLanes(L8, idx_tbl);
                auto z1011_1112 = hn::TableLookupLanes(L10, idx_tbl);

                auto sumA = hn::SatWidenMulPairwiseAccumulate(
                    d32_16, z01_12, coeffs_v[0], round_const_y);
                sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z23_34,
                                                         coeffs_v[1], sumA);

                auto sumB = hn::SatWidenMulPairwiseAccumulate(
                    d32_16, z23_34, coeffs_v[0], round_const_y);
                sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56,
                                                         coeffs_v[1], sumB);

                auto sumC = hn::SatWidenMulPairwiseAccumulate(
                    d32_16, z45_56, coeffs_v[0], round_const_y);
                sumC = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78,
                                                         coeffs_v[1], sumC);

                auto sumD = hn::SatWidenMulPairwiseAccumulate(
                    d32_16, z67_78, coeffs_v[0], round_const_y);
                sumD = hn::SatWidenMulPairwiseAccumulate(d32_16, z89_910,
                                                         coeffs_v[1], sumD);

                round_and_store(sumA, y);
                round_and_store(sumB, y + 2);
                round_and_store(sumC, y + 4);
                round_and_store(sumD, y + 6);

                z01_12 = z89_910;
                z23_34 = z1011_1112;
              }
            }
            else HWY_IF_CONSTEXPR(taps_y_const == 6) {
              auto L0 = hn::LoadU(d16_32, strip_im_buf + 0 * 8);
              auto z01_12 = hn::TableLookupLanes(L0, idx_tbl);
              auto L2 = hn::LoadU(d16_32, strip_im_buf + 2 * 8);
              auto z23_34 = hn::TableLookupLanes(L2, idx_tbl);
              auto L4 = hn::LoadU(d16_32, strip_im_buf + 4 * 8);
              auto z45_56 = hn::TableLookupLanes(L4, idx_tbl);
              hn::Half<decltype(d16_32)> d16_16;

              for (int y = 0; y < h; y += 8) {
                auto L6 = hn::LoadU(d16_32, strip_im_buf + (y + 6) * 8);
                auto L10 = hn::LoadU(d16_32, strip_im_buf + (y + 10) * 8);
                auto L14 = hn::LoadU(d16_32, strip_im_buf + (y + 14) * 8);

                auto L8 = hn::Combine(d16_32, hn::LowerHalf(d16_16, L10),
                                      hn::UpperHalf(d16_16, L6));
                auto L12 = hn::Combine(d16_32, hn::LowerHalf(d16_16, L14),
                                       hn::UpperHalf(d16_16, L10));

                auto z67_78 = hn::TableLookupLanes(L6, idx_tbl);
                auto z89_910 = hn::TableLookupLanes(L8, idx_tbl);
                auto z1011_1112 = hn::TableLookupLanes(L10, idx_tbl);
                auto z1213_1314 = hn::TableLookupLanes(L12, idx_tbl);

                auto sumA = hn::SatWidenMulPairwiseAccumulate(
                    d32_16, z01_12, coeffs_v[0], round_const_y);
                sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z23_34,
                                                         coeffs_v[1], sumA);
                sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56,
                                                         coeffs_v[2], sumA);

                auto sumB = hn::SatWidenMulPairwiseAccumulate(
                    d32_16, z23_34, coeffs_v[0], round_const_y);
                sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56,
                                                         coeffs_v[1], sumB);
                sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78,
                                                         coeffs_v[2], sumB);

                auto sumC = hn::SatWidenMulPairwiseAccumulate(
                    d32_16, z45_56, coeffs_v[0], round_const_y);
                sumC = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78,
                                                         coeffs_v[1], sumC);
                sumC = hn::SatWidenMulPairwiseAccumulate(d32_16, z89_910,
                                                         coeffs_v[2], sumC);

                auto sumD = hn::SatWidenMulPairwiseAccumulate(
                    d32_16, z67_78, coeffs_v[0], round_const_y);
                sumD = hn::SatWidenMulPairwiseAccumulate(d32_16, z89_910,
                                                         coeffs_v[1], sumD);
                sumD = hn::SatWidenMulPairwiseAccumulate(d32_16, z1011_1112,
                                                         coeffs_v[2], sumD);

                round_and_store(sumA, y);
                round_and_store(sumB, y + 2);
                round_and_store(sumC, y + 4);
                round_and_store(sumD, y + 6);

                z01_12 = z89_910;
                z23_34 = z1011_1112;
                z45_56 = z1213_1314;
              }
            }
            else HWY_IF_CONSTEXPR(taps_y_const == 8) {
              auto L0 = hn::LoadU(d16_32, strip_im_buf + 0 * 8);
              auto z01_12 = hn::TableLookupLanes(L0, idx_tbl);
              auto L2 = hn::LoadU(d16_32, strip_im_buf + 2 * 8);
              auto z23_34 = hn::TableLookupLanes(L2, idx_tbl);
              auto L4 = hn::LoadU(d16_32, strip_im_buf + 4 * 8);
              auto z45_56 = hn::TableLookupLanes(L4, idx_tbl);

              for (int y = 0; y < h; y += 8) {
                auto L6 = hn::LoadU(d16_32, strip_im_buf + (y + 6) * 8);
                auto L10 = hn::LoadU(d16_32, strip_im_buf + (y + 10) * 8);
                auto L12 = hn::LoadU(d16_32, strip_im_buf + (y + 12) * 8);

                hn::Half<decltype(d16_32)> d16_16;
                auto L8 = hn::Combine(d16_32, hn::LowerHalf(d16_16, L10),
                                      hn::UpperHalf(d16_16, L6));

                auto z67_78 = hn::TableLookupLanes(L6, idx_tbl);
                auto z89_910 = hn::TableLookupLanes(L8, idx_tbl);
                auto z1011_1112 = hn::TableLookupLanes(L10, idx_tbl);
                auto z1213_1314 = hn::TableLookupLanes(L12, idx_tbl);

                auto sumA = hn::SatWidenMulPairwiseAccumulate(
                    d32_16, z01_12, coeffs_v[0], round_const_y);
                sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z23_34,
                                                         coeffs_v[1], sumA);
                sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56,
                                                         coeffs_v[2], sumA);
                sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78,
                                                         coeffs_v[3], sumA);

                auto sumB = hn::SatWidenMulPairwiseAccumulate(
                    d32_16, z23_34, coeffs_v[0], round_const_y);
                sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56,
                                                         coeffs_v[1], sumB);
                sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78,
                                                         coeffs_v[2], sumB);
                sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z89_910,
                                                         coeffs_v[3], sumB);

                auto sumC = hn::SatWidenMulPairwiseAccumulate(
                    d32_16, z45_56, coeffs_v[0], round_const_y);
                sumC = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78,
                                                         coeffs_v[1], sumC);
                sumC = hn::SatWidenMulPairwiseAccumulate(d32_16, z89_910,
                                                         coeffs_v[2], sumC);
                sumC = hn::SatWidenMulPairwiseAccumulate(d32_16, z1011_1112,
                                                         coeffs_v[3], sumC);

                auto sumD = hn::SatWidenMulPairwiseAccumulate(
                    d32_16, z67_78, coeffs_v[0], round_const_y);
                sumD = hn::SatWidenMulPairwiseAccumulate(d32_16, z89_910,
                                                         coeffs_v[1], sumD);
                sumD = hn::SatWidenMulPairwiseAccumulate(d32_16, z1011_1112,
                                                         coeffs_v[2], sumD);
                sumD = hn::SatWidenMulPairwiseAccumulate(d32_16, z1213_1314,
                                                         coeffs_v[3], sumD);

                round_and_store(sumA, y);
                round_and_store(sumB, y + 2);
                round_and_store(sumC, y + 4);
                round_and_store(sumD, y + 6);

                z01_12 = z89_910;
                z23_34 = z1011_1112;
                z45_56 = z1213_1314;
              }
            }
            else {
              for (int y = 0; y < h; y += 8) {
                auto L0 = hn::LoadU(d16_32, strip_im_buf + (y + 0) * 8);
                auto L2 = hn::LoadU(d16_32, strip_im_buf + (y + 2) * 8);
                auto L4 = hn::LoadU(d16_32, strip_im_buf + (y + 4) * 8);
                auto L6 = hn::LoadU(d16_32, strip_im_buf + (y + 6) * 8);

                auto z01_12 = hn::TableLookupLanes(L0, idx_tbl);
                auto z23_34 = hn::TableLookupLanes(L2, idx_tbl);
                auto z45_56 = hn::TableLookupLanes(L4, idx_tbl);
                auto z67_78 = hn::TableLookupLanes(L6, idx_tbl);

                auto sumA = hn::SatWidenMulPairwiseAccumulate(
                    d32_16, z01_12, coeffs_v[0], round_const_y);
                auto sumB = hn::SatWidenMulPairwiseAccumulate(
                    d32_16, z23_34, coeffs_v[0], round_const_y);
                auto sumC = hn::SatWidenMulPairwiseAccumulate(
                    d32_16, z45_56, coeffs_v[0], round_const_y);
                auto sumD = hn::SatWidenMulPairwiseAccumulate(
                    d32_16, z67_78, coeffs_v[0], round_const_y);

                round_and_store(sumA, y);
                round_and_store(sumB, y + 2);
                round_and_store(sumC, y + 4);
                round_and_store(sumD, y + 6);
              }
            }
          }
          else {
            for (int y = 0; y < h; y += 8) {
              auto L0 = hn::LoadU(d16_32, strip_im_buf + (y + 0) * 8);
              auto L2 = hn::LoadU(d16_32, strip_im_buf + (y + 2) * 8);
              auto L4 = hn::LoadU(d16_32, strip_im_buf + (y + 4) * 8);
              auto L6 = hn::LoadU(d16_32, strip_im_buf + (y + 6) * 8);
              auto z01_12 = hn::TableLookupLanes(L0, idx_tbl);
              auto z23_34 = hn::TableLookupLanes(L2, idx_tbl);
              auto z45_56 = hn::TableLookupLanes(L4, idx_tbl);
              auto z67_78 = hn::TableLookupLanes(L6, idx_tbl);

              auto sumA = hn::SatWidenMulPairwiseAccumulate(
                  d32_16, z01_12, coeffs_v[0], round_const_y);
              auto sumB = hn::SatWidenMulPairwiseAccumulate(
                  d32_16, z23_34, coeffs_v[0], round_const_y);
              auto sumC = hn::SatWidenMulPairwiseAccumulate(
                  d32_16, z45_56, coeffs_v[0], round_const_y);
              auto sumD = hn::SatWidenMulPairwiseAccumulate(
                  d32_16, z67_78, coeffs_v[0], round_const_y);

              round_and_store(sumA, y);
              round_and_store(sumB, y + 2);
              round_and_store(sumC, y + 4);
              round_and_store(sumD, y + 6);
            }
          }
        }
      } else {
        if (taps_y == 2) {
          for (int y = 0; y < h; y += 4) {
            auto L0 = hn::LoadU(d16_32, strip_im_buf + (y + 0) * 8);
            auto L2 = hn::LoadU(d16_32, strip_im_buf + (y + 2) * 8);
            auto z01_12 = hn::TableLookupLanes(L0, idx_tbl);
            auto z23_34 = hn::TableLookupLanes(L2, idx_tbl);

            auto sumA = hn::SatWidenMulPairwiseAccumulate(
                d32_16, z01_12, coeffs_v[0], round_const_y);
            auto sumB = hn::SatWidenMulPairwiseAccumulate(
                d32_16, z23_34, coeffs_v[0], round_const_y);

            round_and_store(sumA, y);
            round_and_store(sumB, y + 2);
          }
        } else if (taps_y == 4) {
          for (int y = 0; y < h; y += 4) {
            auto L0 = hn::LoadU(d16_32, strip_im_buf + (y + 0) * 8);
            auto L2 = hn::LoadU(d16_32, strip_im_buf + (y + 2) * 8);
            auto L4 = hn::LoadU(d16_32, strip_im_buf + (y + 4) * 8);

            auto z01_12 = hn::TableLookupLanes(L0, idx_tbl);
            auto z23_34 = hn::TableLookupLanes(L2, idx_tbl);
            auto z45_56 = hn::TableLookupLanes(L4, idx_tbl);

            auto sumA = hn::SatWidenMulPairwiseAccumulate(
                d32_16, z01_12, coeffs_v[0], round_const_y);
            sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z23_34,
                                                     coeffs_v[1], sumA);

            auto sumB = hn::SatWidenMulPairwiseAccumulate(
                d32_16, z23_34, coeffs_v[0], round_const_y);
            sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56,
                                                     coeffs_v[1], sumB);

            round_and_store(sumA, y);
            round_and_store(sumB, y + 2);
          }
        } else if (taps_y == 6) {
          for (int y = 0; y < h; y += 4) {
            auto L0 = hn::LoadU(d16_32, strip_im_buf + (y + 0) * 8);
            auto L2 = hn::LoadU(d16_32, strip_im_buf + (y + 2) * 8);
            auto L4 = hn::LoadU(d16_32, strip_im_buf + (y + 4) * 8);
            auto L6 = hn::LoadU(d16_32, strip_im_buf + (y + 6) * 8);

            auto z01_12 = hn::TableLookupLanes(L0, idx_tbl);
            auto z23_34 = hn::TableLookupLanes(L2, idx_tbl);
            auto z45_56 = hn::TableLookupLanes(L4, idx_tbl);
            auto z67_78 = hn::TableLookupLanes(L6, idx_tbl);

            auto sumA = hn::SatWidenMulPairwiseAccumulate(
                d32_16, z01_12, coeffs_v[0], round_const_y);
            sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z23_34,
                                                     coeffs_v[1], sumA);
            sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56,
                                                     coeffs_v[2], sumA);

            auto sumB = hn::SatWidenMulPairwiseAccumulate(
                d32_16, z23_34, coeffs_v[0], round_const_y);
            sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56,
                                                     coeffs_v[1], sumB);
            sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78,
                                                     coeffs_v[2], sumB);

            round_and_store(sumA, y);
            round_and_store(sumB, y + 2);
          }
        } else if (taps_y == 8) {
          for (int y = 0; y < h; y += 4) {
            auto L0 = hn::LoadU(d16_32, strip_im_buf + (y + 0) * 8);
            auto L2 = hn::LoadU(d16_32, strip_im_buf + (y + 2) * 8);
            auto L4 = hn::LoadU(d16_32, strip_im_buf + (y + 4) * 8);
            auto L6 = hn::LoadU(d16_32, strip_im_buf + (y + 6) * 8);
            auto L8 = hn::LoadU(d16_32, strip_im_buf + (y + 8) * 8);

            auto z01_12 = hn::TableLookupLanes(L0, idx_tbl);
            auto z23_34 = hn::TableLookupLanes(L2, idx_tbl);
            auto z45_56 = hn::TableLookupLanes(L4, idx_tbl);
            auto z67_78 = hn::TableLookupLanes(L6, idx_tbl);
            auto z89_910 = hn::TableLookupLanes(L8, idx_tbl);

            auto sumA = hn::SatWidenMulPairwiseAccumulate(
                d32_16, z01_12, coeffs_v[0], round_const_y);
            sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z23_34,
                                                     coeffs_v[1], sumA);
            sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56,
                                                     coeffs_v[2], sumA);
            sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78,
                                                     coeffs_v[3], sumA);

            auto sumB = hn::SatWidenMulPairwiseAccumulate(
                d32_16, z23_34, coeffs_v[0], round_const_y);
            sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56,
                                                     coeffs_v[1], sumB);
            sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78,
                                                     coeffs_v[2], sumB);
            sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z89_910,
                                                     coeffs_v[3], sumB);

            round_and_store(sumA, y);
            round_and_store(sumB, y + 2);
          }
        } else if (is_taps_y_12) {
          for (int y = 0; y < h; y += 4) {
            auto L0 = hn::LoadU(d16_32, strip_im_buf + (y + 0) * 8);
            auto L2 = hn::LoadU(d16_32, strip_im_buf + (y + 2) * 8);
            auto L4 = hn::LoadU(d16_32, strip_im_buf + (y + 4) * 8);
            auto L6 = hn::LoadU(d16_32, strip_im_buf + (y + 6) * 8);
            auto L8 = hn::LoadU(d16_32, strip_im_buf + (y + 8) * 8);
            auto L10 = hn::LoadU(d16_32, strip_im_buf + (y + 10) * 8);
            auto L12 = hn::LoadU(d16_32, strip_im_buf + (y + 12) * 8);

            auto z01_12 = hn::TableLookupLanes(L0, idx_tbl);
            auto z23_34 = hn::TableLookupLanes(L2, idx_tbl);
            auto z45_56 = hn::TableLookupLanes(L4, idx_tbl);
            auto z67_78 = hn::TableLookupLanes(L6, idx_tbl);
            auto z89_910 = hn::TableLookupLanes(L8, idx_tbl);
            auto z1011_1112 = hn::TableLookupLanes(L10, idx_tbl);
            auto z1213_1314 = hn::TableLookupLanes(L12, idx_tbl);

            auto sumA = hn::SatWidenMulPairwiseAccumulate(
                d32_16, z01_12, coeffs_v[0], round_const_y);
            sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z23_34,
                                                     coeffs_v[1], sumA);
            sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56,
                                                     coeffs_v[2], sumA);
            sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78,
                                                     coeffs_v[3], sumA);
            sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z89_910,
                                                     coeffs_v[4], sumA);
            sumA = hn::SatWidenMulPairwiseAccumulate(d32_16, z1011_1112,
                                                     coeffs_v[5], sumA);

            auto sumB = hn::SatWidenMulPairwiseAccumulate(
                d32_16, z23_34, coeffs_v[0], round_const_y);
            sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z45_56,
                                                     coeffs_v[1], sumB);
            sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z67_78,
                                                     coeffs_v[2], sumB);
            sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z89_910,
                                                     coeffs_v[3], sumB);
            sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z1011_1112,
                                                     coeffs_v[4], sumB);
            sumB = hn::SatWidenMulPairwiseAccumulate(d32_16, z1213_1314,
                                                     coeffs_v[5], sumB);

            round_and_store(sumA, y);
            round_and_store(sumB, y + 2);
          }
        }
      }
    }
  }
}

template <int tx, int ty, int r0, int r1>
HWY_ATTR void Convolve2DSRRun(const uint8_t *src, int src_stride, uint8_t *dst,
                              int dst_stride, int w, int h,
                              const InterpFilterParams *filter_params_x,
                              const InterpFilterParams *filter_params_y,
                              const int subpel_x_qn, const int subpel_y_qn,
                              ConvolveParams *conv_params) {
  Convolve2DSRHwyImpl<tx, ty, r0, r1>(src, src_stride, dst, dst_stride, w, h,
                                      filter_params_x, filter_params_y,
                                      subpel_x_qn, subpel_y_qn, conv_params);
}

HWY_ATTR inline void Convolve2DSR(const uint8_t *src, int src_stride,
                                  uint8_t *dst, int dst_stride, int w, int h,
                                  const InterpFilterParams *filter_params_x,
                                  const InterpFilterParams *filter_params_y,
                                  const int subpel_x_qn, const int subpel_y_qn,
                                  ConvolveParams *conv_params) {
  const int round_0 = conv_params->round_0;
  const int round_1 = conv_params->round_1;

  if (round_0 == 3 && round_1 == 11) {
    const int tap_x = get_filter_tap(filter_params_x, subpel_x_qn);
    const int tap_y = get_filter_tap(filter_params_y, subpel_y_qn);
    switch (tap_x) {
      case 2:
        switch (tap_y) {
          case 2:
            Convolve2DSRRun<2, 2, 3, 11>(src, src_stride, dst, dst_stride, w, h,
                                         filter_params_x, filter_params_y,
                                         subpel_x_qn, subpel_y_qn, conv_params);
            return;
          case 4:
            Convolve2DSRRun<2, 4, 3, 11>(src, src_stride, dst, dst_stride, w, h,
                                         filter_params_x, filter_params_y,
                                         subpel_x_qn, subpel_y_qn, conv_params);
            return;
          case 6:
            Convolve2DSRRun<2, 6, 3, 11>(src, src_stride, dst, dst_stride, w, h,
                                         filter_params_x, filter_params_y,
                                         subpel_x_qn, subpel_y_qn, conv_params);
            return;
          case 8:
            Convolve2DSRRun<2, 8, 3, 11>(src, src_stride, dst, dst_stride, w, h,
                                         filter_params_x, filter_params_y,
                                         subpel_x_qn, subpel_y_qn, conv_params);
            return;
          case 12:
            Convolve2DSRRun<2, 12, 3, 11>(
                src, src_stride, dst, dst_stride, w, h, filter_params_x,
                filter_params_y, subpel_x_qn, subpel_y_qn, conv_params);
            return;
        }
        break;
      case 4:
        switch (tap_y) {
          case 2:
            Convolve2DSRRun<4, 2, 3, 11>(src, src_stride, dst, dst_stride, w, h,
                                         filter_params_x, filter_params_y,
                                         subpel_x_qn, subpel_y_qn, conv_params);
            return;
          case 4:
            Convolve2DSRRun<4, 4, 3, 11>(src, src_stride, dst, dst_stride, w, h,
                                         filter_params_x, filter_params_y,
                                         subpel_x_qn, subpel_y_qn, conv_params);
            return;
          case 6:
            Convolve2DSRRun<4, 6, 3, 11>(src, src_stride, dst, dst_stride, w, h,
                                         filter_params_x, filter_params_y,
                                         subpel_x_qn, subpel_y_qn, conv_params);
            return;
          case 8:
            Convolve2DSRRun<4, 8, 3, 11>(src, src_stride, dst, dst_stride, w, h,
                                         filter_params_x, filter_params_y,
                                         subpel_x_qn, subpel_y_qn, conv_params);
            return;
          case 12:
            Convolve2DSRRun<4, 12, 3, 11>(
                src, src_stride, dst, dst_stride, w, h, filter_params_x,
                filter_params_y, subpel_x_qn, subpel_y_qn, conv_params);
            return;
        }
        break;
      case 6:
        switch (tap_y) {
          case 2:
            Convolve2DSRRun<6, 2, 3, 11>(src, src_stride, dst, dst_stride, w, h,
                                         filter_params_x, filter_params_y,
                                         subpel_x_qn, subpel_y_qn, conv_params);
            return;
          case 4:
            Convolve2DSRRun<6, 4, 3, 11>(src, src_stride, dst, dst_stride, w, h,
                                         filter_params_x, filter_params_y,
                                         subpel_x_qn, subpel_y_qn, conv_params);
            return;
          case 6:
            Convolve2DSRRun<6, 6, 3, 11>(src, src_stride, dst, dst_stride, w, h,
                                         filter_params_x, filter_params_y,
                                         subpel_x_qn, subpel_y_qn, conv_params);
            return;
          case 8:
            Convolve2DSRRun<6, 8, 3, 11>(src, src_stride, dst, dst_stride, w, h,
                                         filter_params_x, filter_params_y,
                                         subpel_x_qn, subpel_y_qn, conv_params);
            return;
          case 12:
            Convolve2DSRRun<6, 12, 3, 11>(
                src, src_stride, dst, dst_stride, w, h, filter_params_x,
                filter_params_y, subpel_x_qn, subpel_y_qn, conv_params);
            return;
        }
        break;
      case 8:
        switch (tap_y) {
          case 2:
            Convolve2DSRRun<8, 2, 3, 11>(src, src_stride, dst, dst_stride, w, h,
                                         filter_params_x, filter_params_y,
                                         subpel_x_qn, subpel_y_qn, conv_params);
            return;
          case 4:
            Convolve2DSRRun<8, 4, 3, 11>(src, src_stride, dst, dst_stride, w, h,
                                         filter_params_x, filter_params_y,
                                         subpel_x_qn, subpel_y_qn, conv_params);
            return;
          case 6:
            Convolve2DSRRun<8, 6, 3, 11>(src, src_stride, dst, dst_stride, w, h,
                                         filter_params_x, filter_params_y,
                                         subpel_x_qn, subpel_y_qn, conv_params);
            return;
          case 8:
            Convolve2DSRRun<8, 8, 3, 11>(src, src_stride, dst, dst_stride, w, h,
                                         filter_params_x, filter_params_y,
                                         subpel_x_qn, subpel_y_qn, conv_params);
            return;
          case 12:
            Convolve2DSRRun<8, 12, 3, 11>(
                src, src_stride, dst, dst_stride, w, h, filter_params_x,
                filter_params_y, subpel_x_qn, subpel_y_qn, conv_params);
            return;
        }
        break;
      case 12:
        switch (tap_y) {
          case 2:
            Convolve2DSRRun<12, 2, 3, 11>(
                src, src_stride, dst, dst_stride, w, h, filter_params_x,
                filter_params_y, subpel_x_qn, subpel_y_qn, conv_params);
            return;
          case 4:
            Convolve2DSRRun<12, 4, 3, 11>(
                src, src_stride, dst, dst_stride, w, h, filter_params_x,
                filter_params_y, subpel_x_qn, subpel_y_qn, conv_params);
            return;
          case 6:
            Convolve2DSRRun<12, 6, 3, 11>(
                src, src_stride, dst, dst_stride, w, h, filter_params_x,
                filter_params_y, subpel_x_qn, subpel_y_qn, conv_params);
            return;
          case 8:
            Convolve2DSRRun<12, 8, 3, 11>(
                src, src_stride, dst, dst_stride, w, h, filter_params_x,
                filter_params_y, subpel_x_qn, subpel_y_qn, conv_params);
            return;
          case 12:
            Convolve2DSRRun<12, 12, 3, 11>(
                src, src_stride, dst, dst_stride, w, h, filter_params_x,
                filter_params_y, subpel_x_qn, subpel_y_qn, conv_params);
            return;
        }
        break;
    }
  }

  Convolve2DSRRun<0, 0, 0, 0>(src, src_stride, dst, dst_stride, w, h,
                              filter_params_x, filter_params_y, subpel_x_qn,
                              subpel_y_qn, conv_params);
}

}  // namespace HWY_NAMESPACE
}  // namespace

HWY_AFTER_NAMESPACE();

#define MAKE_CONVOLVE_2D_SR(suffix)                                            \
  extern "C" void av1_convolve_2d_sr_##suffix(                                 \
      const uint8_t *src, int src_stride, uint8_t *dst, int dst_stride, int w, \
      int h, const InterpFilterParams *filter_params_x,                        \
      const InterpFilterParams *filter_params_y, const int subpel_x_qn,        \
      const int subpel_y_qn, ConvolveParams *conv_params);                     \
  HWY_ATTR void av1_convolve_2d_sr_##suffix(                                   \
      const uint8_t *src, int src_stride, uint8_t *dst, int dst_stride, int w, \
      int h, const InterpFilterParams *filter_params_x,                        \
      const InterpFilterParams *filter_params_y, const int subpel_x_qn,        \
      const int subpel_y_qn, ConvolveParams *conv_params) {                    \
    if (w < 4) {                                                               \
      av1_convolve_2d_sr_avx2(src, src_stride, dst, dst_stride, w, h,          \
                              filter_params_x, filter_params_y, subpel_x_qn,   \
                              subpel_y_qn, conv_params);                       \
      return;                                                                  \
    }                                                                          \
    HWY_NAMESPACE::Convolve2DSR(src, src_stride, dst, dst_stride, w, h,        \
                                filter_params_x, filter_params_y, subpel_x_qn, \
                                subpel_y_qn, conv_params);                     \
  }

#endif  // AOM_AV1_COMMON_CONVOLVE_2D_SR_HWY_H_
