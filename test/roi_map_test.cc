/*
 * Copyright (c) 2016, Alliance for Open Media. All rights reserved.
 *
 * This source code is subject to the terms of the BSD 2 Clause License and
 * the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
 * was not distributed with this source code in the LICENSE file, you can
 * obtain it at www.aomedia.org/license/software. If the Alliance for Open
 * Media Patent License 1.0 was not distributed with this source code in the
 * PATENTS file, you can obtain it at www.aomedia.org/license/patent.
 */

#include <climits>
#include <vector>
#include "gtest/gtest.h"
#include "test/codec_factory.h"
#include "test/encode_test_driver.h"
#include "test/i420_video_source.h"
#include "test/util.h"

namespace {

// Params: test mode, speed, aq_mode and screen_content mode.
class ROIMapTest
    : public ::libaom_test::CodecTestWith4Params<libaom_test::TestMode, int,
                                                 int, int>,
      public ::libaom_test::EncoderTest {
 protected:
  static const int kWidth = 640;
  static const int kHeight = 360;

  ROIMapTest() : EncoderTest(GET_PARAM(0)) {}
  ~ROIMapTest() override = default;

  void SetUp() override {
    InitializeConfig(GET_PARAM(1));
    cpu_used_ = GET_PARAM(2);
    aq_mode_ = GET_PARAM(3);
    screen_mode_ = GET_PARAM(4);
  }

  void PreEncodeFrameHook(::libaom_test::VideoSource *video,
                          ::libaom_test::Encoder *encoder) override {
    if (video->frame() == 0) {
      encoder->Control(AOME_SET_CPUUSED, cpu_used_);
      encoder->Control(AV1E_SET_ALLOW_WARPED_MOTION, 0);
      encoder->Control(AV1E_SET_ENABLE_GLOBAL_MOTION, 0);
      encoder->Control(AV1E_SET_ENABLE_OBMC, 0);
      encoder->Control(AV1E_SET_AQ_MODE, aq_mode_ ? 3 : 0);
      encoder->Control(AV1E_SET_TUNE_CONTENT, screen_mode_);
      if (screen_mode_) encoder->Control(AV1E_SET_ENABLE_PALETTE, 1);
      const int block_size = 4;
      roi_.rows = (cfg_.g_h + block_size - 1) / block_size;
      roi_.cols = (cfg_.g_w + block_size - 1) / block_size;
      memset(&roi_.skip, 0, sizeof(roi_.skip));
      memset(&roi_.delta_q, 0, sizeof(roi_.delta_q));
      memset(&roi_.delta_lf, 0, sizeof(roi_.delta_lf));
      memset(roi_.ref_frame, -1, sizeof(roi_.ref_frame));
      roi_.ref_frame[1] = 1;
      // Use segment 1 for skip.
      roi_.skip[1] = 1;
      roi_.roi_map =
          (uint8_t *)calloc(roi_.rows * roi_.cols, sizeof(*roi_.roi_map));
      for (unsigned int i = 0; i < roi_.rows; ++i) {
        for (unsigned int j = 0; j < roi_.cols; ++j) {
          const int idx = i * roi_.cols + j;
          if (i > roi_.rows / 2 && i < (3 * roi_.rows) / 4 &&
              j > roi_.cols / 2 && j < (3 * roi_.cols) / 4)
            roi_.roi_map[idx] = 1;
        }
      }
    }
  }

  void DoTest() {
    cfg_.g_lag_in_frames = 0;
    cfg_.rc_target_bitrate = 400;
    cfg_.rc_resize_mode = 0;
    cfg_.g_pass = AOM_RC_ONE_PASS;
    cfg_.rc_end_usage = AOM_CBR;
    cfg_.kf_max_dist = 90000;
    ::libaom_test::I420VideoSource video("niklas_640_480_30.yuv", 640, 480, 30,
                                         1, 0, 400);

    ASSERT_NO_FATAL_FAILURE(RunLoop(&video));
  }

  int cpu_used_;
  int aq_mode_;
  int screen_mode_;

 private:
  aom_roi_map_t roi_;
};

TEST_P(ROIMapTest, Test) { DoTest(); }

AV1_INSTANTIATE_TEST_SUITE(ROIMapTest,
                           ::testing::Values(::libaom_test::kRealTime),
                           ::testing::Range(6, 12), ::testing::Values(0, 1),
                           ::testing::Values(0, 1));

}  // namespace
