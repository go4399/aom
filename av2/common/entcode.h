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

#ifndef AV2_AV2_COMMON_ENTCODE_H_
#define AV2_AV2_COMMON_ENTCODE_H_

#include <assert.h>
#include "config/aom_config.h"
#include "aom_dsp/entcode.h"
#include "aom_dsp/prob.h"
#include "av2/common/common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AV2_EC_PROB_SHIFT 7

extern const uint16_t av2_prob_inc_tbl[15][16];

// Scale the CDF to match the range value stored in the entropy decoder.
static INLINE unsigned av2_od_ec_prob_scale(uint16_t p, unsigned r, int n,
                                            int nsym) {
  int rr = r >> 8;
  int pp = p >> AV2_EC_PROB_SHIFT;
  pp <<= 4;
  pp += av2_prob_inc_tbl[nsym - 2][n];
  return ((rr * pp >> (7 - AV2_EC_PROB_SHIFT + 1 + 6)) << 3);
}

// Adjust probability to more closely match the scaled prob used in
// av2_od_ec_prob_scale()
static INLINE unsigned av2_get_adjusted_prob(uint16_t p, int n, int nsym) {
  int adj_prob = (p >> AV2_EC_PROB_SHIFT) << AV2_EC_PROB_SHIFT;
  int inc = av2_prob_inc_tbl[nsym - 2][n];
  adj_prob += inc << (AV2_EC_PROB_SHIFT - 4);
  return adj_prob;
}

static INLINE void av2_update_cdf(aom_cdf_prob *cdf, int8_t val, int nsymbs) {
  int i, tmp;
  assert(nsymbs < 17);
  const int time_interval = cdf[nsymbs] > 31 ? 2 : (cdf[nsymbs] > 15 ? 1 : 0);
  const int rate = 2 + cdf[nsymbs + 1 + time_interval];
  tmp = AOM_ICDF(0);

  // Single loop (faster)
  for (i = 0; i < nsymbs - 1; ++i) {
    tmp = (i == val) ? 0 : tmp;
    if (tmp < cdf[i]) {
      cdf[i] -= ((cdf[i] - tmp) >> rate);
    } else {
      cdf[i] += ((tmp - cdf[i]) >> rate);
    }
  }
  cdf[nsymbs] += (cdf[nsymbs] < 32);
}

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // AV2_AV2_COMMON_ENTCODE_H_
