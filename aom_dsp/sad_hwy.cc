#define HWY_TARGETS HWY_AVX2

#define HWY_WANT_SSE2 0
#define HWY_WANT_SSSE3 0
#define HWY_WANT_SSE4 0

#define HWY_TARGET_INCLUDE "aom_dsp/sad_hwy.cc"

#include "third_party/highway/hwy/aligned_allocator.h"
#include "third_party/highway/hwy/foreach_target.h"
#include "third_party/highway/hwy/highway.h"
#include "third_party/highway/hwy/print-inl.h"

#include "config/aom_config.h"

HWY_BEFORE_NAMESPACE();

namespace {
namespace HWY_NAMESPACE {

namespace hn = hwy::HWY_NAMESPACE;

unsigned int SumOfAbsoluteDiff64xN(const uint8_t *src_ptr, int src_stride,
                                   const uint8_t *ref_ptr, int ref_stride,
                                   int h) {
  hn::CappedTag<uint8_t, 32> pixel_tag;
  hn::CappedTag<uint64_t, 4> intermediate_sum_tag;
  auto sum_sad = hn::Zero(intermediate_sum_tag);
  for (int i = 0; i < h; i++) {
    auto ref_vec_1 = hn::LoadU(pixel_tag, ref_ptr);
    auto ref_vec_2 = hn::LoadU(pixel_tag, ref_ptr + 32);
    auto sad_1 = hn::SumsOf8AbsDiff(ref_vec_1, hn::LoadU(pixel_tag, src_ptr));
    auto sad_2 =
        hn::SumsOf8AbsDiff(ref_vec_2, hn::LoadU(pixel_tag, src_ptr + 32));
    sum_sad = hn::Add(sum_sad, hn::Add(sad_1, sad_2));
    src_ptr += src_stride;
    ref_ptr += ref_stride;
  }

  unsigned int res =
      static_cast<unsigned int>(hn::ReduceSum(intermediate_sum_tag, sum_sad));
  return res;
}
}  // namespace HWY_NAMESPACE
}  // namespace
HWY_AFTER_NAMESPACE();

#if HWY_TARGET == HWY_AVX2
#define FSAD64_H(h)                                                           \
  extern "C" unsigned int SumOfAbsoluteDiff64x##h##_avx2(                     \
      const uint8_t *src_ptr, int src_stride, const uint8_t *ref_ptr,         \
      int ref_stride) {                                                       \
    return HWY_NAMESPACE::SumOfAbsoluteDiff64xN(src_ptr, src_stride, ref_ptr, \
                                                ref_stride, h);               \
  }

FSAD64_H(32)
FSAD64_H(64)
#endif

#undef FSAD64_H
