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

#ifndef AV2_AV2_COMMON_ENTDEC_H_
#define AV2_AV2_COMMON_ENTDEC_H_

#include "config/aom_config.h"
#include "aom_dsp/bitreader.h"
#include "aom_dsp/entdec.h"
#include "av2/common/entcode.h"

#ifdef __cplusplus
extern "C" {
#endif

OD_WARN_UNUSED_RESULT int av2_od_ec_decode_bool_q15(od_ec_dec *dec, unsigned f)
    OD_ARG_NONNULL(1);
OD_WARN_UNUSED_RESULT int av2_od_ec_decode_cdf_q15(od_ec_dec *dec,
                                                   const uint16_t *cdf,
                                                   int nsyms)
    OD_ARG_NONNULL(1) OD_ARG_NONNULL(2);

OD_WARN_UNUSED_RESULT int av2_od_ec_decode_literal_bypass(od_ec_dec *dec,
                                                          int n_bits)
    OD_ARG_NONNULL(1);

static INLINE int av2_read_(aom_reader *r, int prob ACCT_STR_PARAM) {
  int p = (0x7FFFFF - (prob << 15) + prob) >> 8;
  int bit = av2_od_ec_decode_bool_q15(&r->ec, p);
#if CONFIG_ACCOUNTING
  if (ACCT_STR_NAME) aom_process_accounting(r, ACCT_STR_NAME);
  aom_update_symb_counts(r, 1);
#endif
  return bit;
}

static INLINE int av2_read_bit_(aom_reader *r ACCT_STR_PARAM) {
  int ret = av2_od_ec_decode_literal_bypass(&r->ec, 1);
#if CONFIG_ACCOUNTING
  if (ACCT_STR_NAME) aom_process_accounting(r, ACCT_STR_NAME);
#endif
  return ret;
}

static INLINE int av2_read_literal_(aom_reader *r, int bits ACCT_STR_PARAM) {
  int literal = 0;
  int n_bits = bits;
  int n;
  while (n_bits > 0) {
    n = n_bits >= 8 ? 8 : n_bits;
    literal <<= n;
    literal += av2_od_ec_decode_literal_bypass(&r->ec, n);
    n_bits -= n;
  }
#if CONFIG_ACCOUNTING
  if (ACCT_STR_NAME) aom_process_accounting(r, ACCT_STR_NAME);
#endif
  return literal;
}


static INLINE int av2_read_cdf_(aom_reader *r, const aom_cdf_prob *cdf,
                                int nsymbs ACCT_STR_PARAM) {
  int symb;
  assert(cdf != NULL);
  symb = av2_od_ec_decode_cdf_q15(&r->ec, cdf, nsymbs);
#if CONFIG_ACCOUNTING
  if (ACCT_STR_NAME) aom_process_accounting(r, ACCT_STR_NAME);
  aom_update_symb_counts(r, (nsymbs == 2));
#endif
  return symb;
}

static INLINE int av2_read_symbol_(aom_reader *r, aom_cdf_prob *cdf,
                                   int nsymbs ACCT_STR_PARAM) {
  int ret;
  ret = av2_read_cdf_(r, cdf, nsymbs ACCT_STR_ARG(ACCT_STR_NAME));
  if (r->allow_update_cdf) av2_update_cdf(cdf, ret, nsymbs);
  return ret;
}

#undef aom_read
#undef aom_read_bit
#undef aom_read_literal
#undef aom_read_cdf
#undef aom_read_symbol

#define aom_read(r, prob, ACCT_STR_NAME) \
  av2_read_(r, prob ACCT_STR_ARG(ACCT_STR_NAME))
#define aom_read_bit(r, ACCT_STR_NAME) \
  av2_read_bit_(r ACCT_STR_ARG(ACCT_STR_NAME))
#define aom_read_literal(r, bits, ACCT_STR_NAME) \
  av2_read_literal_(r, bits ACCT_STR_ARG(ACCT_STR_NAME))
#define aom_read_cdf(r, cdf, nsymbs, ACCT_STR_NAME) \
  av2_read_cdf_(r, cdf, nsymbs ACCT_STR_ARG(ACCT_STR_NAME))
#define aom_read_symbol(r, cdf, nsymbs, ACCT_STR_NAME) \
  av2_read_symbol_(r, cdf, nsymbs ACCT_STR_ARG(ACCT_STR_NAME))

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // AV2_AV2_COMMON_ENTDEC_H_
