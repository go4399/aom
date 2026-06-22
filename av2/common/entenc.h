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

#ifndef AV2_AV2_COMMON_ENTENC_H_
#define AV2_AV2_COMMON_ENTENC_H_

#include "config/aom_config.h"
#include "aom_dsp/bitwriter.h"
#include "aom_dsp/entenc.h"
#include "av2/common/entcode.h"

#ifdef __cplusplus
extern "C" {
#endif

void av2_od_ec_encode_bool_q15(od_ec_enc *enc, int val, unsigned f_q15)
    OD_ARG_NONNULL(1);
void av2_od_ec_encode_cdf_q15(od_ec_enc *enc, int s, const uint16_t *cdf,
                              int nsyms) OD_ARG_NONNULL(1) OD_ARG_NONNULL(3);

void av2_od_ec_encode_literal_bypass(od_ec_enc *enc, int val, int n_bits)
    OD_ARG_NONNULL(1);

static INLINE void av2_write(aom_writer *w, int bit, int probability) {
  int p = (0x7FFFFF - (probability << 15) + probability) >> 8;
  av2_od_ec_encode_bool_q15(&w->ec, bit, p);
}

static INLINE void av2_write_bit(aom_writer *w, int bit) {
  av2_od_ec_encode_literal_bypass(&w->ec, bit, 1);
}

static INLINE void av2_write_literal(aom_writer *w, int data, int bits) {
  int n_bits = bits;
  int n;
  while (n_bits > 0) {
    n = n_bits >= 8 ? 8 : n_bits;
    av2_od_ec_encode_literal_bypass(&w->ec, (data >> (bits - n)) & ((1 << n) - 1), n);
    n_bits -= n;
  }
}


static INLINE void av2_write_cdf(aom_writer *w, int symb,
                                 const aom_cdf_prob *cdf, int nsymbs) {
  av2_od_ec_encode_cdf_q15(&w->ec, symb, cdf, nsymbs);
}

static INLINE void av2_write_symbol(aom_writer *w, int symb, aom_cdf_prob *cdf,
                                    int nsymbs) {
  av2_write_cdf(w, symb, cdf, nsymbs);
  if (w->allow_update_cdf) av2_update_cdf(cdf, symb, nsymbs);
}

#undef aom_write
#undef aom_write_bit
#undef aom_write_literal
#undef aom_write_cdf
#undef aom_write_symbol

#define aom_write av2_write
#define aom_write_bit av2_write_bit
#define aom_write_literal av2_write_literal
#define aom_write_cdf av2_write_cdf
#define aom_write_symbol av2_write_symbol

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // AV2_AV2_COMMON_ENTENC_H_
