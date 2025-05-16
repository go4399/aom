/*
 * Copyright (c) 2025, Alliance for Open Media. All rights reserved.
 *
 * This source code is subject to the terms of the BSD 2 Clause License and
 * the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
 * was not distributed with this source code in the LICENSE file, you can
 * obtain it at www.aomedia.org/license/software. If the Alliance for Open
 * Media Patent License 1.0 was not distributed with this source code in the
 * PATENTS file, you can obtain it at www.aomedia.org/license/patent.
 */
#include "gtest/gtest.h"

#include "av1/encoder/encoder.h"

namespace {
/* clang-format off */
  // Test the example taken from dilate_block()
  uint8_t source[] =   {   0,   0,   1,   2, 255,   3,   4,   0,   0,
                           0,   5,   6, 255, 255, 255,   7,   8,   0,
                           0, 255, 255, 255, 255, 255, 255, 255,   0,
                           0, 255, 255, 255, 255, 255, 255, 255,   0,
                           0,   9,  10, 255, 255, 255,  11,  12,   0,
                           0,   0,  13,  14, 255,  15,  16,   0,   0};

  uint8_t expected[] = {   0,   0, 255, 255, 255, 255, 255,   0,   0,
                         255, 255, 255, 255, 255, 255, 255, 255, 255,
                         255, 255, 255, 255, 255, 255, 255, 255, 255,
                         255, 255, 255, 255, 255, 255, 255, 255, 255,
                         255, 255, 255, 255, 255, 255, 255, 255, 255,
                           0,   0, 255, 255, 255, 255, 255,   0,   0};
/* clang-format on */

int32_t width = 9;
int32_t height = 6;

TEST(ScreenContentDetectionMode2, FindDominantValue) {
  // Find the dominant value of source[], which should be 255,
  // as it appears 22 times. This is in contrast to 0 (16 times).
  EXPECT_EQ(find_dominant_value(source, width, height, width), 255);
}

TEST(ScreenContentDetectionMode2, DilateBlock) {
  uint8_t dilated[9 * 6] = { 0 };

  dilate_block(source, width, dilated, width, height, width);

  // Compare values coming from dilate_block against the expected values
  for (int32_t r = 0; r < height; ++r) {
    for (int32_t c = 0; c < width; ++c) {
      EXPECT_EQ(expected[r * height + c], dilated[r * height + c]);
    }
  }
}

}  // namespace
