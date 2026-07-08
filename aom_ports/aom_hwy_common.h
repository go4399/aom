/*
 * Copyright (c) 2026, Alliance for Open Media. All rights reserved.
 *
 * This source code is subject to the terms of the BSD 2 Clause License and
 * the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
 * was not distributed with this source code in the LICENSE file, you can
 * obtain it at www.aomedia.org/license/software. If the Alliance for Open
 * Media Patent License 1.0 was not distributed with this source code in the
 * PATENTS file, you can obtain it at www.aomedia.org/license/patent.
 */

#ifndef AOM_AOM_PORTS_AOM_HWY_COMMON_H_
#define AOM_AOM_PORTS_AOM_HWY_COMMON_H_

#if defined(__GNUC__) && !defined(__clang__)
#define AOM_SUPPRESS_HWY_WARNINGS_PUSH \
  _Pragma("GCC diagnostic push")       \
      _Pragma("GCC diagnostic ignored \"-Wextra-semi\"")
#define AOM_SUPPRESS_HWY_WARNINGS_POP _Pragma("GCC diagnostic pop")
#else
#define AOM_SUPPRESS_HWY_WARNINGS_PUSH
#define AOM_SUPPRESS_HWY_WARNINGS_POP
#endif

#endif  // AOM_AOM_PORTS_AOM_HWY_COMMON_H_
