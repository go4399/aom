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

#include "gtest/gtest.h"

#include <cstdlib>
#include <memory>
#include <new>

#include "av2/common/entenc.h"
#include "av2/common/entdec.h"

TEST(AV2_EC_TEST, random_ec_test) {
  od_ec_enc enc;
  od_ec_dec dec;
  int sz;
  int i;
  unsigned int seed = 0xdaa1a;
  unsigned char *ptr;
  uint32_t ptr_sz;

  srand(seed);
  od_ec_enc_init(&enc, 1);
  for (i = 0; i < 10000; i++) {
    int j;
    sz = (rand() % 100) + 1;
    std::unique_ptr<unsigned[]> fz(new (std::nothrow) unsigned[sz]);
    ASSERT_NE(fz, nullptr);
    std::unique_ptr<unsigned[]> data(new (std::nothrow) unsigned[sz]);
    ASSERT_NE(data, nullptr);

    od_ec_enc_reset(&enc);
    for (j = 0; j < sz; j++) {
      data[j] = rand() & 1;
      fz[j] = (rand() % 32766) + 1;
      if ((j & 1) == 0) {
        av2_od_ec_encode_bool_q15(&enc, data[j], fz[j]);
      } else {
        uint16_t cdf[2];
        cdf[0] = AOM_ICDF(fz[j]);
        cdf[1] = AOM_ICDF(32768U);
        av2_od_ec_encode_cdf_q15(&enc, data[j], cdf, 2);
      }
    }
    ptr = od_ec_enc_done(&enc, &ptr_sz);
    ASSERT_NE(ptr, nullptr);
    od_ec_dec_init(&dec, ptr, ptr_sz);
    for (j = 0; j < sz; j++) {
      unsigned int sym = data[j] + 1;
      if ((j & 1) == 0) {
        sym = av2_od_ec_decode_bool_q15(&dec, fz[j]);
      } else {
        uint16_t cdf[2];
        cdf[0] = AOM_ICDF(fz[j]);
        cdf[1] = AOM_ICDF(32768U);
        sym = av2_od_ec_decode_cdf_q15(&dec, cdf, 2);
      }
      EXPECT_EQ(sym, data[j]);
    }
  }
  od_ec_enc_clear(&enc);
}
