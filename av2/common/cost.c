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
#include <assert.h>

#include "av2/common/cost.h"
#include "av2/common/entropy.h"

static const uint16_t av2_prob_inc_tbl[15][16] = {
  { 8, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 10, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 12, 8, 4, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 12, 9, 6, 3, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 13, 10, 8, 5, 2, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 13, 11, 9, 6, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 14, 12, 10, 8, 6, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 14, 12, 10, 8, 7, 5, 3, 1, 0, -1, -1, -1, -1, -1, -1, -1 },
  { 14, 12, 11, 9, 8, 6, 4, 3, 1, 0, -1, -1, -1, -1, -1, -1 },
  { 14, 13, 11, 10, 8, 7, 5, 4, 2, 1, 0, -1, -1, -1, -1, -1 },
  { 14, 13, 12, 10, 9, 8, 6, 5, 4, 2, 1, 0, -1, -1, -1, -1 },
  { 14, 13, 12, 11, 9, 8, 7, 6, 4, 3, 2, 1, 0, -1, -1, -1 },
  { 14, 13, 12, 11, 10, 9, 8, 6, 5, 4, 3, 2, 1, 0, -1, -1 },
  { 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1 },
  { 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 }
};

#define EC_PROB_SHIFT 6

static INLINE unsigned get_adjusted_prob(uint16_t p, int n, int nsym) {
  int adj_prob = (p >> EC_PROB_SHIFT) << EC_PROB_SHIFT;
  int inc = av2_prob_inc_tbl[nsym - 2][n];
  adj_prob += inc << (EC_PROB_SHIFT - 4);
  return adj_prob;
}

// round(-log2(i/256.) * (1 << AV2_PROB_COST_SHIFT)); i = 128~255.
const uint16_t av2_prob_cost[128] = {
  512, 506, 501, 495, 489, 484, 478, 473, 467, 462, 456, 451, 446, 441, 435,
  430, 425, 420, 415, 410, 405, 400, 395, 390, 385, 380, 375, 371, 366, 361,
  356, 352, 347, 343, 338, 333, 329, 324, 320, 316, 311, 307, 302, 298, 294,
  289, 285, 281, 277, 273, 268, 264, 260, 256, 252, 248, 244, 240, 236, 232,
  228, 224, 220, 216, 212, 209, 205, 201, 197, 194, 190, 186, 182, 179, 175,
  171, 168, 164, 161, 157, 153, 150, 146, 143, 139, 136, 132, 129, 125, 122,
  119, 115, 112, 109, 105, 102, 99,  95,  92,  89,  86,  82,  79,  76,  73,
  70,  66,  63,  60,  57,  54,  51,  48,  45,  42,  38,  35,  32,  29,  26,
  23,  20,  18,  15,  12,  9,   6,   3,
};

void av2_cost_tokens_from_cdf(int *costs, const aom_cdf_prob *cdf,
                              const int nsym, const int *inv_map) {
  aom_cdf_prob p0 = CDF_PROB_TOP;
  for (int i = 0; i < nsym; i++) {
    aom_cdf_prob p1 = get_adjusted_prob(cdf[i], i, nsym);
    aom_cdf_prob p15 = p0 - p1;
    p0 = p1;
    if (inv_map)
      costs[inv_map[i]] = av2_cost_symbol(p15);
    else
      costs[i] = av2_cost_symbol(p15);
  }
}
