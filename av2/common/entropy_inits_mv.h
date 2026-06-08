/*
 * Copyright (c) 2024, Alliance for Open Media. All rights reserved
 *
 * This source code is subject to the terms of the BSD 3-Clause Clear License
 * and the Alliance for Open Media Patent License 1.0. If the BSD 3-Clause Clear
 * License was not distributed with this source code in the LICENSE file, you
 * can obtain it at aomedia.org/license/software-license/bsd-3-c-c/.  If the
 * Alliance for Open Media Patent License 1.0 was not distributed with this
 * source code in the PATENTS file, you can obtain it at
 * aomedia.org/license/patent-license/.
 */

#ifndef AV2_AV2_COMMON_ENTROPY_INITS_MV_H_
#define AV2_AV2_COMMON_ENTROPY_INITS_MV_H_

#include "config/aom_config.h"
#include "av2/common/entropymv.h"

#ifdef __cplusplus
extern "C" {
#endif

static const nmv_context default_nmv_context = {
  { AV2_CDF2(31579), AV2_PARA2(-1, 0, 0) },
  {
      { AV2_CDF5(4460, 12999, 22505, 30840), AV2_PARA5(-1, -2, 1) },
      { AV2_CDF6(7519, 18907, 25563, 29875, 31983), AV2_PARA6(-1, 0, -2) },
      { AV2_CDF6(5461, 10923, 16384, 21845, 27307), AV2_PARA6(0, 0, 0) },
      { AV2_CDF7(8680, 13723, 18208, 22686, 26722, 30020),
        AV2_PARA7(0, -1, 0) },
      { AV2_CDF7(4324, 15300, 23690, 28697, 31282, 32359),
        AV2_PARA7(0, 0, -1) },
      { AV2_CDF8(7497, 17301, 23848, 27438, 29395, 30879, 32003),
        AV2_PARA8(-1, -1, -1) },
      { AV2_CDF8(10667, 20239, 25883, 29670, 31400, 32153, 32579),
        AV2_PARA8(0, 0, 0) },
  },
  {
      { AV2_CDF6(21329, 30564, 32589, 32649, 32708), AV2_PARA6(-2, 0, 0) },
      { AV2_CDF6(24250, 31806, 32676, 32722, 32732), AV2_PARA6(-2, 0, 0) },
      { AV2_CDF7(4681, 9362, 14043, 18725, 23406, 28087), AV2_PARA7(0, 0, 0) },
      { AV2_CDF7(19978, 30160, 32564, 32732, 32736, 32740),
        AV2_PARA7(0, 0, -1) },
      { AV2_CDF8(19707, 28414, 31240, 31648, 32692, 32717, 32721),
        AV2_PARA8(-1, 0, 0) },
      { AV2_CDF8(18469, 27427, 31562, 32652, 32724, 32728, 32732),
        AV2_PARA8(-1, -1, -1) },
      { AV2_CDF8(17810, 25196, 29372, 31953, 32564, 32720, 32724),
        AV2_PARA8(-2, -1, 0) },
  },
  { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
  {
      { AV2_CDF2(14587), AV2_PARA2(-1, -2, -1) },
      { AV2_CDF2(20966), AV2_PARA2(1, 0, 0) },
  },
  { AV2_CDF2(13189), AV2_PARA2(0, 0, 0) },
  {
      {
          { AV2_CDF2(17943), AV2_PARA2(1, 1, 1) },
          { AV2_CDF2(18934), AV2_PARA2(1, 1, 1) },
          { AV2_CDF2(18928), AV2_PARA2(1, 1, 1) },
          { AV2_CDF2(18696), AV2_PARA2(1, 1, 1) },
          { AV2_CDF2(19044), AV2_PARA2(1, 1, 1) },
          { AV2_CDF2(20362), AV2_PARA2(1, 1, 1) },
          { AV2_CDF2(20426), AV2_PARA2(1, 1, 1) },
          { AV2_CDF2(22563), AV2_PARA2(1, 1, 1) },
          { AV2_CDF2(22190), AV2_PARA2(1, 1, 1) },
          { AV2_CDF2(23458), AV2_PARA2(1, 1, 0) },
          { AV2_CDF2(26227), AV2_PARA2(0, 0, -2) },
          { AV2_CDF2(30765), AV2_PARA2(-2, 0, 0) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
          { AV2_CDF2(16384), AV2_PARA2(0, 0, 0) },
      },
  },
  {
      { AV2_CDF2(5663), AV2_PARA2(-1, 0, 0) },
      { AV2_CDF2(4856), AV2_PARA2(1, 1, 0) },
  },
  {
      { AV2_CDF2(13445), AV2_PARA2(0, 0, -1) },
      { AV2_CDF2(13541), AV2_PARA2(0, 0, -1) },
      { AV2_CDF2(14045), AV2_PARA2(0, 0, -1) },
      { AV2_CDF2(12888), AV2_PARA2(-1, -1, -1) },
  },
  { AV2_CDF4(4, 17705, 32748), AV2_PARA4(0, 0, -1) },
  {
      {
          { AV2_CDF8(10549, 15298, 16241, 22533, 27449, 30520, 32080),
            AV2_PARA8(-1, 0, -1) },
      },
      {
          { AV2_CDF8(9414, 14965, 15966, 22465, 27468, 30628, 32144),
            AV2_PARA8(-1, 0, -1) },
      },
  },
};

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  // AV2_AV2_COMMON_ENTROPY_INITS_MV_H_
