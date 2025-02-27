#define HWY_WANT_SSE2 0
#define HWY_WANT_SSSE3 0
#define HWY_WANT_SSE4 0

#define HWY_TARGET_INCLUDE "aom_dsp/sad_hwy.cc"

#include "third_party/highway/hwy/aligned_allocator.h"
#include "third_party/highway/hwy/highway.h"

#include "config/aom_config.h"

HWY_BEFORE_NAMESPACE();

namespace {
namespace HWY_NAMESPACE {

namespace hn = hwy::HWY_NAMESPACE;

HWY_INLINE unsigned int SumOfAbsoluteDiff64xN(const uint8_t *src_ptr,
                                              int src_stride,
                                              const uint8_t *ref_ptr,
                                              int ref_stride, int h) {
  hn::CappedTag<uint8_t, 32> pixel_tag;
  auto sum_sad = hn::Zero(pixel_tag);
  for (int i = 0; i < h; i++) {
    auto ref_vec_1 = hn::Load(pixel_tag, ref_ptr);
    auto ref_vec_2 = hn::Load(pixel_tag, ref_ptr + 32);
    auto sad_1 = hn::AbsDiff(ref_vec_1, hn::Load(pixel_tag, src_ptr));
    auto sad_2 = hn::AbsDiff(ref_vec_2, hn::Load(pixel_tag, src_ptr + 32));
    sum_sad = hn::Add(sum_sad, hn::Add(sad_1, sad_2));
    src_ptr += src_stride;
    ref_ptr += ref_stride;
  }

  unsigned int res = hn::GetLane(hn::SumOfLanes(pixel_tag, sum_sad));
  return res;
}
}  // namespace HWY_NAMESPACE
}  // namespace
HWY_AFTER_NAMESPACE();

#define FSAD64_H(h)                                                           \
  extern "C" unsigned int SumOfAbsoluteDiff64x##h##_avx2(                     \
      const uint8_t *src_ptr, int src_stride, const uint8_t *ref_ptr,         \
      int ref_stride) {                                                       \
    return HWY_NAMESPACE::SumOfAbsoluteDiff64xN(src_ptr, src_stride, ref_ptr, \
                                                ref_stride, h);               \
  }

FSAD64_H(32)
FSAD64_H(64)

#undef FSAD64_H
