#ifndef AVM_AVM_DSP_BLEND_H_
#define AVM_AVM_DSP_BLEND_H_

#include "aom_dsp/blend.h"
#include "avm/avm_integer.h"

#define AVM_BLEND_A64_ROUND_BITS 6
#define AVM_BLEND_A64_MAX_ALPHA (1 << AVM_BLEND_A64_ROUND_BITS)  // 64

#define AVM_BLEND_A64(a, v0, v1)                                          \
  ROUND_POWER_OF_TWO((a) * (v0) + (AVM_BLEND_A64_MAX_ALPHA - (a)) * (v1), \
                      AVM_BLEND_A64_ROUND_BITS)

#define AVM_BLEND_A256_ROUND_BITS 8
#define AVM_BLEND_A256_MAX_ALPHA (1 << AVM_BLEND_A256_ROUND_BITS)  // 256

#define AVM_BLEND_A256(a, v0, v1)                                          \
  ROUND_POWER_OF_TWO((a) * (v0) + (AVM_BLEND_A256_MAX_ALPHA - (a)) * (v1), \
                      AVM_BLEND_A256_ROUND_BITS)

#define AVM_BLEND_AVG(v0, v1) ROUND_POWER_OF_TWO((v0) + (v1), 1)

#define DIFF_FACTOR_LOG2 4
#define DIFF_FACTOR (1 << DIFF_FACTOR_LOG2)

#endif  // AVM_AVM_DSP_BLEND_H_
