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

#include "av2/common/entdec.h"
#include <assert.h>

#define OD_EC_LOTS_OF_BITS (0x4000)

static void av2_od_ec_dec_refill(od_ec_dec *dec) {
  int s;
  od_ec_window dif;
  int16_t cnt;
  const unsigned char *bptr;
  const unsigned char *end;
  dif = dec->dif;
  cnt = dec->cnt;
  bptr = dec->bptr;
  end = dec->end;
  s = OD_EC_WINDOW_SIZE - 9 - (cnt + 15);
  for (; s >= 0 && bptr < end; s -= 8, bptr++) {
    assert(s <= OD_EC_WINDOW_SIZE - 8);
    dif ^= (od_ec_window)bptr[0] << s;
    cnt += 8;
  }
  if (bptr >= end) {
    dec->tell_offs += OD_EC_LOTS_OF_BITS - cnt;
    cnt = OD_EC_LOTS_OF_BITS;
  }
  dec->dif = dif;
  dec->cnt = cnt;
  dec->bptr = bptr;
}

static int av2_od_ec_dec_normalize(od_ec_dec *dec, od_ec_window dif,
                                   unsigned rng, int ret) {
  int d;
  assert(rng <= 65535U);
  d = 16 - OD_ILOG_NZ(rng);
  dec->cnt -= d;
  dec->dif = ((dif + 1) << d) - 1;
  dec->rng = rng << d;
  if (dec->cnt < 0) av2_od_ec_dec_refill(dec);
  return ret;
}

int av2_od_ec_decode_bool_q15(od_ec_dec *dec, unsigned f) {
  od_ec_window dif;
  od_ec_window vw;
  unsigned r;
  unsigned r_new;
  unsigned v;
  int ret;
  assert(0 < f);
  assert(f < 32768U);
  dif = dec->dif;
  r = dec->rng;
  assert(dif >> (OD_EC_WINDOW_SIZE - 16) < r);
  assert(32768U <= r);
  v = av2_od_ec_prob_scale(f, r, 0, 2);
  vw = (od_ec_window)v << (OD_EC_WINDOW_SIZE - 16);
  ret = 1;
  r_new = v;
  if (dif >= vw) {
    r_new = r - v;
    dif -= vw;
    ret = 0;
  }
  return av2_od_ec_dec_normalize(dec, dif, r_new, ret);
}

int av2_od_ec_decode_cdf_q15(od_ec_dec *dec, const uint16_t *icdf, int nsyms) {
  od_ec_window dif;
  unsigned r;
  unsigned c;
  unsigned u;
  unsigned v;
  int ret;
  (void)nsyms;
  dif = dec->dif;
  r = dec->rng;

  assert(dif >> (OD_EC_WINDOW_SIZE - 16) < r);
  assert(icdf[nsyms - 1] == AOM_ICDF(CDF_PROB_TOP));
  assert(32768U <= r);
  assert(7 - AV2_EC_PROB_SHIFT >= 0);
  c = (unsigned)(dif >> (OD_EC_WINDOW_SIZE - 16));
  v = r;
  ret = -1;
  do {
    u = v;
    ret++;
    v = av2_od_ec_prob_scale(icdf[ret], r, ret, nsyms);
  } while (c < v);
  assert(v < u);
  assert(u <= r);
  r = u - v;
  dif -= (od_ec_window)v << (OD_EC_WINDOW_SIZE - 16);
  return av2_od_ec_dec_normalize(dec, dif, r, ret);
}
