/*
 * Copyright (c) 2018, Alliance for Open Media. All rights reserved.
 *
 * This source code is subject to the terms of the BSD 2 Clause License and
 * the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
 * was not distributed with this source code in the LICENSE file, you can
 * obtain it at www.aomedia.org/license/software. If the Alliance for Open
 * Media Patent License 1.0 was not distributed with this source code in the
 * PATENTS file, you can obtain it at www.aomedia.org/license/patent.
 */

#include "gtest/gtest.h"
#include "test/codec_factory.h"
#include "test/encode_test_driver.h"
#include "test/i420_video_source.h"
#include "test/util.h"

namespace {

const int kCpuUsed = 8;
const int kBaseLayerQp = 55;
const int kEnhancementLayerQp = 20;

class ScalabilityTest
    : public ::libaom_test::CodecTestWithParam<libaom_test::TestMode>,
      public ::libaom_test::EncoderTest {
 protected:
  ScalabilityTest() : EncoderTest(GET_PARAM(0)) {}
  ~ScalabilityTest() override = default;

  void SetUp() override {
    InitializeConfig(GET_PARAM(1));
    num_spatial_layers_ = 2;
  }

  void PreEncodeFrameHook(::libaom_test::VideoSource *video,
                          ::libaom_test::Encoder *encoder) override {
    if (video->frame() == 0) {
      encoder->Control(AOME_SET_CPUUSED, kCpuUsed);
      aom_svc_params_t svc_params = {};
      svc_params.number_spatial_layers = num_spatial_layers_;
      svc_params.number_temporal_layers = 1;
      for (int i = 0; i < num_spatial_layers_; i++) {
        svc_params.scaling_factor_num[i] = 1;
        svc_params.scaling_factor_den[i] = 1;
      }
      encoder->Control(AV1E_SET_SVC_PARAMS, &svc_params);
    }
    if (video->frame() % num_spatial_layers_) {
      frame_flags_ = AOM_EFLAG_NO_REF_LAST2 | AOM_EFLAG_NO_REF_LAST3 |
                     AOM_EFLAG_NO_REF_GF | AOM_EFLAG_NO_REF_ARF |
                     AOM_EFLAG_NO_REF_BWD | AOM_EFLAG_NO_REF_ARF2 |
                     AOM_EFLAG_NO_UPD_LAST | AOM_EFLAG_NO_UPD_GF |
                     AOM_EFLAG_NO_UPD_ARF | AOM_EFLAG_NO_UPD_ENTROPY;
      encoder->Control(AOME_SET_SPATIAL_LAYER_ID, 1);
      encoder->Control(AOME_SET_CQ_LEVEL, kEnhancementLayerQp);
    } else {
      frame_flags_ = AOM_EFLAG_NO_REF_LAST2 | AOM_EFLAG_NO_REF_LAST3 |
                     AOM_EFLAG_NO_REF_GF | AOM_EFLAG_NO_REF_ARF |
                     AOM_EFLAG_NO_REF_BWD | AOM_EFLAG_NO_REF_ARF2 |
                     AOM_EFLAG_NO_UPD_GF | AOM_EFLAG_NO_UPD_ARF |
                     AOM_EFLAG_NO_UPD_ENTROPY;
      encoder->Control(AOME_SET_SPATIAL_LAYER_ID, 0);
      encoder->Control(AOME_SET_CQ_LEVEL, kBaseLayerQp);
    }
  }

  void DoTest(int num_spatial_layers) {
    num_spatial_layers_ = num_spatial_layers;
    cfg_.rc_end_usage = AOM_Q;
    cfg_.g_lag_in_frames = 0;

    ::libaom_test::I420VideoSource video("hantro_collage_w352h288.yuv", 352,
                                         288, 30, 1, 0, 18);
    ASSERT_NO_FATAL_FAILURE(RunLoop(&video));
  }

  int num_spatial_layers_;
};

TEST_P(ScalabilityTest, TestNoMismatch2SpatialLayers) { DoTest(2); }

TEST_P(ScalabilityTest, TestNoMismatch3SpatialLayers) { DoTest(3); }

AV1_INSTANTIATE_TEST_SUITE(ScalabilityTest,
                           ::testing::Values(::libaom_test::kRealTime));

}  // namespace
