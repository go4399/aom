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

#include "av2/common/entenc.h"
#include <stdlib.h>
#include <assert.h>

static void av2_od_ec_enc_normalize(od_ec_enc *enc, od_ec_enc_window low,
                                    unsigned rng, int n_bypass) {
  int d;
  int c;
  int s;
  if (enc->error) return;
  if (n_bypass > 0) {
    c = enc->cnt + n_bypass;
    d = 0;
  } else {
    c = enc->cnt;
    assert(rng <= 65535U);
    d = 16 - OD_ILOG_NZ(rng);
  }
  s = c + d;

  if (s >= 0) {  // 56 - 16
    unsigned char *out = enc->buf;
    uint32_t storage = enc->storage;
    uint32_t offs = enc->offs;
    if (offs + 8 > storage) {
      storage = 2 * storage + 8;
      out = (unsigned char *)realloc(out, sizeof(*out) * storage);
      if (out == NULL) {
        enc->error = -1;
        return;
      }
      enc->buf = out;
      enc->storage = storage;
    }
    uint8_t num_bytes_ready = (s >> 3) + 1;
    c += 24 - (num_bytes_ready << 3);
    uint64_t output = low >> c;
    low = low & (((uint64_t)1 << c) - 1);

    uint64_t mask = (uint64_t)1 << (num_bytes_ready << 3);
    uint64_t carry = output & mask;

    mask = mask - 0x01;
    output = output & mask;

    write_enc_data_to_out_buf(out, offs, output, carry, &enc->offs,
                              num_bytes_ready);

    s = c + d - 24;
  }
  enc->low = low << d;
  enc->rng = rng << d;
  enc->cnt = s;
}


static void av2_od_ec_encode_q15(od_ec_enc *enc, unsigned fl, unsigned fh,
                                 int s, int nsyms) {
  od_ec_enc_window l;
  unsigned r;
  unsigned u;
  unsigned v;
  l = enc->low;
  r = enc->rng;
  assert(32768U <= r);
  assert(fh <= fl);
  assert(fl <= 32768U);
  assert(7 - AV2_EC_PROB_SHIFT >= 0);
  if (fl < CDF_PROB_TOP) {
    u = av2_od_ec_prob_scale(fl, r, s - 1, nsyms);
    v = av2_od_ec_prob_scale(fh, r, s, nsyms);
    l += r - u;
    r = u - v;
  } else {
    v = av2_od_ec_prob_scale(fh, r, s, nsyms);
    r -= v;
  }
  av2_od_ec_enc_normalize(enc, l, r, 0);
}

void av2_od_ec_encode_bool_q15(od_ec_enc *enc, int val, unsigned f) {
  od_ec_enc_window l;
  unsigned r;
  unsigned v;
  assert(0 < f);
  assert(f < 32768U);
  l = enc->low;
  r = enc->rng;
  assert(32768U <= r);
  v = av2_od_ec_prob_scale(f, r, 0, 2);
  if (val) l += r - v;
  r = val ? v : r - v;
  av2_od_ec_enc_normalize(enc, l, r, 0);
}

void av2_od_ec_encode_cdf_q15(od_ec_enc *enc, int s, const uint16_t *icdf,
                              int nsyms) {
  assert(s >= 0);
  assert(s < nsyms);
  assert(icdf[nsyms - 1] == AOM_ICDF(CDF_PROB_TOP));
  av2_od_ec_encode_q15(enc, s > 0 ? icdf[s - 1] : AOM_ICDF(0), icdf[s], s,
                       nsyms);
}

void av2_od_ec_encode_literal_bypass(od_ec_enc *enc, int val, int n_bits) {
  od_ec_enc_window l;
  unsigned r;
  l = enc->low;
  r = enc->rng;
  assert(32768U <= r);
  assert((r & 1) == 0);
  l <<= n_bits;
  l += r * val;
  av2_od_ec_enc_normalize(enc, l, r, n_bits);
}


