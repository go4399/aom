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

#ifndef AOM_AV1_ENCODER_PARTITIONS_PRUNE_MODEL_H_
#define AOM_AV1_ENCODER_PARTITIONS_PRUNE_MODEL_H_

#include "av1/encoder/ml.h"
#include "partitions_prune_model_definition.h"
#include "partitions_prune_model_16x16_definition.h"
#include "partitions_prune_model_32x32_definition.h"
#include "partitions_prune_model_64x64_definition.h"


#ifdef __cplusplus
extern "C" {
#endif

#define NUM_CLASSES 4  // Number of partition types supported to prune

static inline int av1_partitions_prune_inference(const float *features, int max_modes, BLOCK_SIZE bsize) {
  float logits[NUM_CLASSES];
  if (bsize == BLOCK_16X16) {
    av1_nn_predict_c(features, &partitions_prune_16x16_nn_config, 1, logits);
  } else if (bsize == BLOCK_32X32) {
    av1_nn_predict_c(features, &partitions_prune_32x32_nn_config, 1, logits);
  } else if (bsize == BLOCK_64X64) {
    av1_nn_predict_c(features, &partitions_prune_64x64_nn_config, 1, logits);
  } else {
    fprintf(stderr, "Unsupported block size %d. It should be square 16/32/64 \n", bsize);
    assert(0);
    // Return safe default: allow all partitions for unsupported block sizes
    // return (1 << NUM_CLASSES) - 1;
  }
  // av1_nn_predict_c(features, &partitions_prune_nn_config, 1, logits);
  

  int indices[NUM_CLASSES];
  for (int i = 0; i < NUM_CLASSES; i++) indices[i] = i;

  // Simple insertion sort for NUM_CLASSES elements to get indices in descending order of
  // probability
  for (int i = 1; i < NUM_CLASSES; i++) {
    int key_idx = indices[i];
    float key_prob = logits[key_idx];
    int j = i - 1;
    while (j >= 0 && logits[indices[j]] < key_prob) {
      indices[j + 1] = indices[j];
      j = j - 1;
    }
    indices[j + 1] = key_idx;
  }

  int bitmask = 0;
  int count = (max_modes <= NUM_CLASSES) ? max_modes : NUM_CLASSES;
  for (int i = 0; i < count; i++) {
    bitmask |= (1 << indices[i]);
  }

  // fprintf(stderr, "features: ");
  // for (int i = 1; i < NUM_CLASSES; i++) {
  //   fprintf(stderr, "%f, ", features[i]);
  // }
  // fprintf(stderr, "\n");
  // fprintf(stderr, "logits: %f, %f, %f, %f\n", logits[0], logits[1],
  //         logits[2], logits[3]);
  // fprintf(stderr, "indices: %d, %d, %d, %d\n", indices[0], indices[1],
  //         indices[2], indices[3]);
  // fprintf(stderr, "bitmask: 0x%x\n", bitmask);

  return bitmask;
}

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // AOM_AV1_ENCODER_PARTITIONS_PRUNE_MODEL_H_
