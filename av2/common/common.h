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

#ifndef AVM_AV2_COMMON_COMMON_H_
#define AVM_AV2_COMMON_COMMON_H_

#ifndef AV2_CDF_SIZE
#define AV2_CDF_SIZE(x) ((x) + 4)
#endif

#ifndef TX_SIZES_ALL
#define TX_SIZES_ALL TX_SIZES_ALL_AV2
#endif

#ifndef MAX_WEDGE_TYPES
#define MAX_WEDGE_TYPES 68
#endif

#define AVM_PARA2(a, b, c) ((a) + 2), ((b) + 3), ((c) + 4)
#define AVM_PARA3(a, b, c) ((a) + 2), ((b) + 3), ((c) + 4)
#define AVM_PARA4(a, b, c) ((a) + 3), ((b) + 4), ((c) + 5)
#define AVM_PARA5(a, b, c) ((a) + 3), ((b) + 4), ((c) + 5)
#define AVM_PARA6(a, b, c) ((a) + 3), ((b) + 4), ((c) + 5)
#define AVM_PARA7(a, b, c) ((a) + 3), ((b) + 4), ((c) + 5)
#define AVM_PARA8(a, b, c) ((a) + 3), ((b) + 4), ((c) + 5)

#ifndef CDF_PROB_TOP
#define CDF_PROB_BITS 15
#define CDF_PROB_TOP (1 << CDF_PROB_BITS)
#endif

#ifndef AOM_ICDF
#define AOM_ICDF(x) (CDF_PROB_TOP - (x))
#endif
#ifndef AVM_ICDF
#define AVM_ICDF AOM_ICDF
#endif

#define AVM_CDF2(a0) AVM_ICDF(a0), AVM_ICDF(CDF_PROB_TOP), 0
#define AVM_CDF3(a0, a1) AVM_ICDF(a0), AVM_ICDF(a1), AVM_ICDF(CDF_PROB_TOP), 0
#define AVM_CDF4(a0, a1, a2) \
  AVM_ICDF(a0), AVM_ICDF(a1), AVM_ICDF(a2), AVM_ICDF(CDF_PROB_TOP), 0
#define AVM_CDF5(a0, a1, a2, a3)                          \
  AVM_ICDF(a0), AVM_ICDF(a1), AVM_ICDF(a2), AVM_ICDF(a3), \
      AVM_ICDF(CDF_PROB_TOP), 0
#define AVM_CDF6(a0, a1, a2, a3, a4)                                    \
  AVM_ICDF(a0), AVM_ICDF(a1), AVM_ICDF(a2), AVM_ICDF(a3), AVM_ICDF(a4), \
      AVM_ICDF(CDF_PROB_TOP), 0
#define AVM_CDF7(a0, a1, a2, a3, a4, a5)                                \
  AVM_ICDF(a0), AVM_ICDF(a1), AVM_ICDF(a2), AVM_ICDF(a3), AVM_ICDF(a4), \
      AVM_ICDF(a5), AVM_ICDF(CDF_PROB_TOP), 0
#define AVM_CDF8(a0, a1, a2, a3, a4, a5, a6)                            \
  AVM_ICDF(a0), AVM_ICDF(a1), AVM_ICDF(a2), AVM_ICDF(a3), AVM_ICDF(a4), \
      AVM_ICDF(a5), AVM_ICDF(a6), AVM_ICDF(CDF_PROB_TOP), 0

/* Interface header for common constant data structures and lookup tables */

#include <assert.h>

#include "aom_dsp/aom_dsp_common.h"
#include "aom_mem/aom_mem.h"
#include "aom/aom_integer.h"
#include "aom_ports/bitops.h"
#include "config/avm_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PI 3.141592653589793238462643383279502884

// Only need this for fixed-size arrays, for structs just assign.
#define av2_copy(dest, src)                         \
  {                                                 \
    static_assert(sizeof(dest) == sizeof(src), ""); \
    memcpy(dest, src, sizeof(src));                 \
  }

// Use this for variably-sized arrays.
#define av2_copy_array(dest, src, n)                      \
  {                                                       \
    static_assert(sizeof(*(dest)) == sizeof(*(src)), ""); \
    memcpy(dest, src, n * sizeof(*(src)));                \
  }

#define av2_zero(dest) memset(&(dest), 0, sizeof(dest))
#define av2_zero_array(dest, n) memset(dest, 0, n * sizeof(*(dest)))

#ifndef get_unsigned_bits_defined
#define get_unsigned_bits_defined
static INLINE int get_unsigned_bits(unsigned int num_values) {
  return num_values > 0 ? get_msb(num_values) + 1 : 0;
}
#endif

#ifndef CHECK_MEM_ERROR_defined
#define CHECK_MEM_ERROR_defined
#define CHECK_MEM_ERROR(cm, lval, expr) \
  AOM_CHECK_MEM_ERROR(&cm->error, lval, expr)
#endif

#define AV2_MIN_TILE_SIZE_BYTES 1

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // AVM_AV2_COMMON_COMMON_H_
