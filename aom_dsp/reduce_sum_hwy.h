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
#ifndef AOM_AOM_DSP_REDUCE_SUM_HWY_H_
#define AOM_AOM_DSP_REDUCE_SUM_HWY_H_

#include "third_party/highway/hwy/highway.h"

HWY_BEFORE_NAMESPACE();

namespace {
namespace HWY_NAMESPACE {

namespace hn = hwy::HWY_NAMESPACE;

template <size_t NumBlocks>
struct BlockReduceTraits {};

template <typename D>
HWY_ATTR hn::Vec<hn::BlockDFromD<D>> BlockReduceSum(D int_tag, hn::VFromD<D> v);

template <>
struct BlockReduceTraits<1> {
  template <typename D>
  HWY_ATTR HWY_INLINE static hn::VFromD<D> ReduceSum(D int_tag,
                                                     hn::VFromD<D> v) {
    (void)int_tag;
    return v;
  }
};

template <>
struct BlockReduceTraits<2> {
  template <typename D>
  HWY_ATTR HWY_INLINE static hn::VFromD<hn::BlockDFromD<D>> ReduceSum(
      D int_tag, hn::VFromD<D> v) {
    (void)int_tag;
    constexpr hn::BlockDFromD<D> block_tag;
    return hn::Add(hn::LowerHalf(block_tag, v), hn::UpperHalf(block_tag, v));
  }
};

template <>
struct BlockReduceTraits<4> {
  template <typename D>
  HWY_ATTR HWY_INLINE static hn::VFromD<hn::BlockDFromD<D>> ReduceSum(
      D int_tag, hn::VFromD<D> v) {
    (void)int_tag;
    constexpr hn::Half<D> half_tag;
    constexpr hn::BlockDFromD<D> block_tag;
    auto vh = hn::Add(hn::LowerHalf(half_tag, v), hn::UpperHalf(half_tag, v));
    return hn::Add(hn::LowerHalf(block_tag, vh), hn::UpperHalf(block_tag, vh));
  }
};

template <typename D>
HWY_ATTR hn::Vec<hn::BlockDFromD<D>> BlockReduceSum(D int_tag,
                                                    hn::VFromD<D> v) {
  return BlockReduceTraits<int_tag.MaxBlocks()>::ReduceSum(int_tag, v);
}

}  // namespace HWY_NAMESPACE
}  // namespace

HWY_AFTER_NAMESPACE();

#endif  // AOM_AOM_DSP_REDUCE_SUM_HWY_H_
