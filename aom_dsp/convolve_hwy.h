/*
 * Copyright (c) 2025, Alliance for Open Media. All rights reserved.
 *
 * This source code is subject to the terms of the BSD 2 Clause License and
 * the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
 * was not distributed with this source code in the LICENSE file, you can
 * obtain it at www.aomedia.org/license/software. If the Alliance for Open
 * Media Patent License 1.0 was not distributed with this source code in the
 * PATENTS file, you can obtain it at www.aomedia.org/license/patent.
 */

#ifndef AOM_AOM_DSP_CONVOLVE_HWY_H_
#define AOM_AOM_DSP_CONVOLVE_HWY_H_

#include "aom_dsp/aom_filter.h"

#include "third_party/highway/hwy/highway.h"
#include <cassert>

HWY_BEFORE_NAMESPACE();

namespace {
namespace HWY_NAMESPACE {

namespace hn = hwy::HWY_NAMESPACE;

static inline int get_filter_taps_convolve8(const int16_t *filter) {
  if (filter[0] | filter[7]) {
    return 8;
  }
  if (filter[1] | filter[6]) {
    return 6;
  }
  if (filter[2] | filter[5]) {
    return 4;
  }
  return 2;
}

template <typename D>
HWY_ATTR HWY_INLINE hn::VFromD<D> LoadUnaligned4x4(D tag16, const uint8_t *buf,
                                                   ptrdiff_t stride) {
  uint16_t buf_to_array[16];
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      buf_to_array[i * 4 + j] = buf[j];
    }
    buf += stride;
  }
  return hn::LoadU(tag16, buf_to_array);
}

template <typename D>
HWY_ATTR HWY_INLINE void StoreUnaligned4x4(D tag16, uint8_t *buf,
                                           ptrdiff_t stride,
                                           hn::VFromD<D> &vec) {
  (void)tag16;
  hn::Rebind<uint8_t, D> tag8;
  auto vec_demoted = hn::DemoteTo(tag8, vec);
  constexpr hn::Half<decltype(tag8)> half_tag;
  constexpr hn::Half<decltype(half_tag)> quarter_tag;
  auto vec1_2 = hn::UpperHalf(half_tag, vec_demoted);
  auto vec2_2 = hn::LowerHalf(half_tag, vec_demoted);
  auto vec1_4 = hn::UpperHalf(quarter_tag, vec1_2);
  auto vec2_4 = hn::LowerHalf(quarter_tag, vec1_2);
  auto vec3_4 = hn::UpperHalf(quarter_tag, vec2_2);
  auto vec4_4 = hn::LowerHalf(quarter_tag, vec2_2);
  hn::StoreU(vec1_4, quarter_tag, buf);
  hn::StoreU(vec2_4, quarter_tag, buf + stride);
  hn::StoreU(vec3_4, quarter_tag, buf + 2 * stride);
  hn::StoreU(vec4_4, quarter_tag, buf + 3 * stride);
}

template <typename D>
HWY_ATTR HWY_INLINE hn::VFromD<D> LoadUnaligned2x8(D tag16, const uint8_t *buf,
                                                   ptrdiff_t stride) {
  hn::Rebind<uint8_t, D> tag8;
  constexpr hn::Half<decltype(tag8)> half_tag8;
  auto first_half = hn::LoadU(half_tag8, buf);
  buf += stride;
  auto second_half = hn::LoadU(half_tag8, buf);
  return hn::PromoteTo(tag16, hn::Combine(tag8, first_half, second_half));
}

template <typename D>
HWY_ATTR HWY_INLINE void StoreUnaligned2x8(D tag, uint8_t *buf,
                                           ptrdiff_t stride,
                                           hn::VFromD<D> &vec) {
  (void)tag;
  hn::Rebind<uint8_t, D> tag8;
  auto vec_demoted = hn::DemoteTo(tag8, vec);
  constexpr hn::Half<decltype(tag8)> half_tag8;
  auto vec1_2 = hn::UpperHalf(half_tag8, vec_demoted);
  auto vec2_2 = hn::LowerHalf(half_tag8, vec_demoted);
  hn::StoreU(vec1_2, half_tag8, buf);
  hn::StoreU(vec2_2, half_tag8, buf + stride);
}

HWY_ATTR HWY_INLINE void ConvolveHoriz2Tap(const uint8_t *src,
                                           ptrdiff_t src_stride, uint8_t *dst,
                                           ptrdiff_t dst_stride,
                                           const int16_t *filter_x, int w,
                                           int h) {
  hn::CappedTag<uint16_t, 16> tag16;
  auto filter1 = hn::Set(tag16, filter_x[3]);
  auto filter2 = hn::Set(tag16, filter_x[4]);
  if (w == 4) {
    do {
      // Draft: assume we're loading a 4x4 block
      auto src1 = LoadUnaligned4x4(tag16, src, src_stride);
      auto src2 = LoadUnaligned4x4(tag16, src + 1, src_stride);
      auto mulv =
          hn::RoundingShiftRight<FILTER_BITS>(src1 * filter1 + src2 * filter2);
      StoreUnaligned4x4(tag16, dst, dst_stride, mulv);
      src += 4 * src_stride;
      dst += 4 * dst_stride;
      h -= 4;
    } while (h > 0);
  } else if (w == 8) {
    do {
      // Draft: we're loading a 2x8 block
      auto src1 = LoadUnaligned2x8(tag16, src, src_stride);
      auto src2 = LoadUnaligned2x8(tag16, src + 1, src_stride);
      auto mulv =
          hn::RoundingShiftRight<FILTER_BITS>(src1 * filter1 + src2 * filter2);
      StoreUnaligned2x8(tag16, dst, dst_stride, mulv);
      src += 2 * src_stride;
      dst += 2 * dst_stride;
      h -= 2;
    } while (h > 0);
  } else if (w == 16) {
    do {
      // Draft: we're loading a 1x16 block
      hn::CappedTag<uint8_t, 16> narrow_tag;
      auto src1 = hn::LoadU(narrow_tag, src);
      auto src2 = hn::LoadU(narrow_tag, src + 1);
      auto src1_promo = hn::PromoteTo(tag16, src1);
      auto src2_promo = hn::PromoteTo(tag16, src2);
      auto mulv = hn::RoundingShiftRight<FILTER_BITS>(src1_promo * filter1 +
                                                      src2_promo * filter2);
      hn::StoreU(hn::DemoteTo(narrow_tag, mulv), narrow_tag, dst);
      src += src_stride;
      dst += dst_stride;
      h--;
    } while (h > 0);
  } else {
    // Tag used for multiply result
    hn::ScalableTag<uint16_t> mul_tag;
    // Tag used for loading, same number of lanes as mul_tag
    hn::Rebind<uint8_t, decltype(mul_tag)> pixel_tag;
    auto vw = hn::Lanes(mul_tag);
    auto filter_0 = hn::Set(mul_tag, filter_x[3]);
    auto filter_1 = hn::Set(mul_tag, filter_x[4]);
    for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; j += vw) {
        auto src0 = hn::PromoteTo(mul_tag, hn::LoadU(pixel_tag, src));
        auto src1 = hn::PromoteTo(mul_tag, hn::LoadU(pixel_tag, src + 1));
        auto mulv = hn::RoundingShiftRight<FILTER_BITS>(src0 * filter_0 +
                                                        src1 * filter_1);
        auto mulv_demoted = hn::DemoteTo(pixel_tag, mulv);
        hn::StoreU(mulv_demoted, pixel_tag, dst);
      }
      src += src_stride;
      dst += dst_stride;
    }
  }
}

template <typename D, typename DFilter>
HWY_ATTR HWY_INLINE hn::VFromD<D> Convolve4_8(
    D tag16, DFilter tag_filter, hn::VFromD<D> &s0, hn::VFromD<D> &s1,
    hn::VFromD<D> &s2, hn::VFromD<D> &s3, hn::VFromD<DFilter> &filter) {
  (void)tag_filter;
  auto mul0 = s0 * hn::Set(tag16, hn::ExtractLane(filter, 0));
  auto mul1 = s1 * hn::Set(tag16, hn::ExtractLane(filter, 1));
  auto mul2 = s2 * hn::Set(tag16, hn::ExtractLane(filter, 2));
  auto mul3 = s3 * hn::Set(tag16, hn::ExtractLane(filter, 3));

  auto res = mul0 + mul1 + mul2 + mul3;
  // Shift (FILTER_BITS - 1) because filter values were halved.
  return hn::RoundingShiftRight<FILTER_BITS - 1>(res);
}

HWY_ATTR HWY_INLINE void ConvolveHoriz4Tap(const uint8_t *src,
                                           ptrdiff_t src_stride, uint8_t *dst,
                                           ptrdiff_t dst_stride,
                                           const int16_t *filter_x, int w,
                                           int h) {
  hn::CappedTag<uint16_t, 16> tag16;
  hn::CappedTag<int16_t, 4> filter_tag;
  auto f_vec = hn::LoadU(filter_tag, filter_x + 2);
  // All filter values are even, halve to reduce intermediate precision
  // requirements.
  f_vec = hn::ShiftRight<1>(f_vec);

  if (w == 4) {
    // Each iteration processes a 4x4 block
    do {
      auto src0 = LoadUnaligned4x4(tag16, src, src_stride);
      auto src1 = LoadUnaligned4x4(tag16, src + 1, src_stride);
      auto src2 = LoadUnaligned4x4(tag16, src + 2, src_stride);
      auto src3 = LoadUnaligned4x4(tag16, src + 3, src_stride);
      auto result =
          Convolve4_8(tag16, filter_tag, src0, src1, src2, src3, f_vec);
      StoreUnaligned4x4(tag16, dst, dst_stride, result);
      h -= 4;
      src += 4 * src_stride;
      dst += 4 * dst_stride;
    } while (h > 0);
  } else if (w == 8) {
    // Each iteration processes a 2x8 block
    do {
      auto src0 = LoadUnaligned2x8(tag16, src, src_stride);
      auto src1 = LoadUnaligned2x8(tag16, src + 1, src_stride);
      auto src2 = LoadUnaligned2x8(tag16, src + 2, src_stride);
      auto src3 = LoadUnaligned2x8(tag16, src + 3, src_stride);
      auto result =
          Convolve4_8(tag16, filter_tag, src0, src1, src2, src3, f_vec);
      StoreUnaligned2x8(tag16, dst, dst_stride, result);
      h -= 2;
      src += 2 * src_stride;
      dst += 2 * dst_stride;
    } while (h > 0);
  } else if (w == 16) {
    // One 1x16 block a time
    do {
      hn::Rebind<uint8_t, decltype(tag16)> tag8;
      auto src0 = hn::PromoteTo(tag16, hn::LoadU(tag8, src));
      auto src1 = hn::PromoteTo(tag16, hn::LoadU(tag8, src + 1));
      auto src2 = hn::PromoteTo(tag16, hn::LoadU(tag8, src + 2));
      auto src3 = hn::PromoteTo(tag16, hn::LoadU(tag8, src + 3));
      auto result =
          Convolve4_8(tag16, filter_tag, src0, src1, src2, src3, f_vec);
      hn::StoreU(hn::DemoteTo(tag8, result), tag8, dst);
      h--;
      src += src_stride;
      dst += dst_stride;
    } while (h > 0);
  } else {
    hn::ScalableTag<uint16_t> mul_tag;
    hn::Rebind<uint8_t, decltype(mul_tag)> pixel_tag;
    auto vw = hn::Lanes(mul_tag);
    for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; j += vw) {
        auto src0 = hn::PromoteTo(mul_tag, hn::LoadU(pixel_tag, src));
        auto src1 = hn::PromoteTo(mul_tag, hn::LoadU(pixel_tag, src + 1));
        auto src2 = hn::PromoteTo(mul_tag, hn::LoadU(pixel_tag, src + 2));
        auto src3 = hn::PromoteTo(mul_tag, hn::LoadU(pixel_tag, src + 3));
        auto result =
            Convolve4_8(mul_tag, filter_tag, src0, src1, src2, src3, f_vec);
        hn::StoreU(hn::DemoteTo(pixel_tag, result), pixel_tag, dst);
      }
      src += src_stride;
      dst += dst_stride;
    }
  }
}

template <typename D, typename DFilter>
HWY_ATTR HWY_INLINE hn::VFromD<D> Convolve8_8(
    D tag16, DFilter tag_filter, hn::VFromD<D> &s0, hn::VFromD<D> &s1,
    hn::VFromD<D> &s2, hn::VFromD<D> &s3, hn::VFromD<D> &s4, hn::VFromD<D> &s5,
    hn::VFromD<D> &s6, hn::VFromD<D> &s7, hn::VFromD<DFilter> &filter) {
  (void)tag_filter;
  auto mul0 = s0 * hn::Set(tag16, hn::ExtractLane(filter, 0));
  auto mul1 = s1 * hn::Set(tag16, hn::ExtractLane(filter, 1));
  auto mul2 = s2 * hn::Set(tag16, hn::ExtractLane(filter, 2));
  auto mul3 = s3 * hn::Set(tag16, hn::ExtractLane(filter, 3));
  auto mul4 = s4 * hn::Set(tag16, hn::ExtractLane(filter, 4));
  auto mul5 = s5 * hn::Set(tag16, hn::ExtractLane(filter, 5));
  auto mul6 = s6 * hn::Set(tag16, hn::ExtractLane(filter, 6));
  auto mul7 = s7 * hn::Set(tag16, hn::ExtractLane(filter, 7));

  auto res = mul0 + mul1 + mul2 + mul3 + mul4 + mul5 + mul6 + mul7;
  // Shift (FILTER_BITS - 1) because filter values were halved.
  return hn::RoundingShiftRight<FILTER_BITS - 1>(res);
}

HWY_ATTR HWY_INLINE void ConvolveHoriz8Tap(const uint8_t *src,
                                           ptrdiff_t src_stride, uint8_t *dst,
                                           ptrdiff_t dst_stride,
                                           const int16_t *filter_x, int w,
                                           int h) {
  hn::CappedTag<uint16_t, 16> tag16;
  hn::CappedTag<int16_t, 8> filter_tag;
  auto f_vec = hn::LoadU(filter_tag, filter_x);
  // All filter values are even, halve to reduce intermediate precision
  // requirements.
  f_vec = hn::ShiftRight<1>(f_vec);

  if (w == 4) {
    do {
      auto src0 = LoadUnaligned4x4(tag16, src, src_stride);
      auto src1 = LoadUnaligned4x4(tag16, src + 1, src_stride);
      auto src2 = LoadUnaligned4x4(tag16, src + 2, src_stride);
      auto src3 = LoadUnaligned4x4(tag16, src + 3, src_stride);
      auto src4 = LoadUnaligned4x4(tag16, src + 4, src_stride);
      auto src5 = LoadUnaligned4x4(tag16, src + 5, src_stride);
      auto src6 = LoadUnaligned4x4(tag16, src + 6, src_stride);
      auto src7 = LoadUnaligned4x4(tag16, src + 7, src_stride);
      auto result = Convolve8_8(tag16, filter_tag, src0, src1, src2, src3, src4,
                                src5, src6, src7, f_vec);
      StoreUnaligned4x4(tag16, dst, dst_stride, result);
      h -= 4;
      src += 4 * src_stride;
      dst += 4 * dst_stride;
    } while (h > 0);
  } else if (w == 8) {
    // Each iteration processes a 2x8 block
    do {
      auto src0 = LoadUnaligned2x8(tag16, src, src_stride);
      auto src1 = LoadUnaligned2x8(tag16, src + 1, src_stride);
      auto src2 = LoadUnaligned2x8(tag16, src + 2, src_stride);
      auto src3 = LoadUnaligned2x8(tag16, src + 3, src_stride);
      auto src4 = LoadUnaligned2x8(tag16, src + 4, src_stride);
      auto src5 = LoadUnaligned2x8(tag16, src + 5, src_stride);
      auto src6 = LoadUnaligned2x8(tag16, src + 6, src_stride);
      auto src7 = LoadUnaligned2x8(tag16, src + 7, src_stride);
      auto result = Convolve8_8(tag16, filter_tag, src0, src1, src2, src3, src4,
                                src5, src6, src7, f_vec);
      StoreUnaligned2x8(tag16, dst, dst_stride, result);
      h -= 2;
      src += 2 * src_stride;
      dst += 2 * dst_stride;
    } while (h > 0);
  } else if (w == 16) {
    // One 1x16 block a time
    do {
      hn::Rebind<uint8_t, decltype(tag16)> tag8;
      auto src0 = hn::PromoteTo(tag16, hn::LoadU(tag8, src));
      auto src1 = hn::PromoteTo(tag16, hn::LoadU(tag8, src + 1));
      auto src2 = hn::PromoteTo(tag16, hn::LoadU(tag8, src + 2));
      auto src3 = hn::PromoteTo(tag16, hn::LoadU(tag8, src + 3));
      auto src4 = hn::PromoteTo(tag16, hn::LoadU(tag8, src + 4));
      auto src5 = hn::PromoteTo(tag16, hn::LoadU(tag8, src + 5));
      auto src6 = hn::PromoteTo(tag16, hn::LoadU(tag8, src + 6));
      auto src7 = hn::PromoteTo(tag16, hn::LoadU(tag8, src + 7));
      auto result = Convolve8_8(tag16, filter_tag, src0, src1, src2, src3, src4,
                                src5, src6, src7, f_vec);
      hn::StoreU(hn::DemoteTo(tag8, result), tag8, dst);
      h--;
      src += src_stride;
      dst += dst_stride;
    } while (h > 0);
  } else {
    // This tag will have 32 lanes (for avx512) or 16 lanes (for avx2)
    hn::ScalableTag<uint16_t> mul_tag;
    hn::Rebind<uint8_t, decltype(mul_tag)> pixel_tag;
    auto vw = hn::Lanes(mul_tag);
    for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; j += vw) {
        auto src0 = hn::PromoteTo(mul_tag, hn::LoadU(pixel_tag, src));
        auto src1 = hn::PromoteTo(mul_tag, hn::LoadU(pixel_tag, src + 1));
        auto src2 = hn::PromoteTo(mul_tag, hn::LoadU(pixel_tag, src + 2));
        auto src3 = hn::PromoteTo(mul_tag, hn::LoadU(pixel_tag, src + 3));
        auto src4 = hn::PromoteTo(mul_tag, hn::LoadU(pixel_tag, src + 4));
        auto src5 = hn::PromoteTo(mul_tag, hn::LoadU(pixel_tag, src + 5));
        auto src6 = hn::PromoteTo(mul_tag, hn::LoadU(pixel_tag, src + 6));
        auto src7 = hn::PromoteTo(mul_tag, hn::LoadU(pixel_tag, src + 7));
        auto result = Convolve8_8(mul_tag, filter_tag, src0, src1, src2, src3,
                                  src4, src5, src6, src7, f_vec);
        hn::StoreU(hn::DemoteTo(pixel_tag, result), pixel_tag, dst);
      }
      src += src_stride;
      dst += dst_stride;
    }
  }
}

HWY_MAYBE_UNUSED void Convolve8Horiz(const uint8_t *src, ptrdiff_t src_stride,
                                     uint8_t *dst, ptrdiff_t dst_stride,
                                     const int16_t *filter_x, int x_step_q4,
                                     const int16_t *filter_y, int y_step_q4,
                                     int w, int h) {
  assert((intptr_t)dst % 4 == 0);
  assert(dst_stride % 4 == 0);

  (void)x_step_q4;
  (void)filter_y;
  (void)y_step_q4;

  src -= ((SUBPEL_TAPS / 2) - 1);
  int filter_taps = get_filter_taps_convolve8(filter_x);
  if (filter_taps == 2) {
    ConvolveHoriz2Tap(src + 3, src_stride, dst, dst_stride, filter_x, w, h);
  } else if (filter_taps == 4) {
    ConvolveHoriz4Tap(src + 2, src_stride, dst, dst_stride, filter_x, w, h);
  } else {
    // filter_taps = 8
    ConvolveHoriz8Tap(src, src_stride, dst, dst_stride, filter_x, w, h);
  }
}

}  // namespace HWY_NAMESPACE
}  // namespace

#define CONVOLVE8HORIZ(suffix)                                                \
  extern "C" HWY_ATTR void aom_convolve8_horiz_##suffix(                      \
      const uint8_t *src, ptrdiff_t src_stride, uint8_t *dst,                 \
      ptrdiff_t dst_stride, const int16_t *filter_x, int x_step_q4,           \
      const int16_t *filter_y, int y_step_q4, int w, int h) {                 \
    HWY_NAMESPACE::Convolve8Horiz(src, src_stride, dst, dst_stride, filter_x, \
                                  x_step_q4, filter_y, y_step_q4, w, h);      \
  }

HWY_AFTER_NAMESPACE();

#endif  // AOM_AOM_DSP_CONVOLVE_HWY_H_