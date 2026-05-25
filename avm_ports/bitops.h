#ifndef AVM_PORTS_BITOPS_H_
#define AVM_PORTS_BITOPS_H_

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "aom_ports/bitops.h"

#ifdef __cplusplus
extern "C" {
#endif

static INLINE int get_msb_signed(int32_t n) {
  return n == 0 ? 0 : get_msb((unsigned int)abs(n));
}

static INLINE int get_msb_signed_64(int64_t n) {
  uint64_t n_abs = (uint64_t)llabs(n);
  unsigned int high32 = n_abs >> 32;
  unsigned int low32 = n_abs & 0x00000000ffffffffULL;
  if (high32 != 0) return 32 + get_msb(high32);
  return low32 == 0 ? 0 : get_msb((unsigned int)low32);
}

static inline int avm_ceil_log2(int n) {
  return aom_ceil_log2(n);
}

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // AVM_PORTS_BITOPS_H_
