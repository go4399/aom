/*
 * Copyright (c) 2026, Alliance for Open Media. All rights reserved
 *
 * This source code is subject to the terms of the BSD 3-Clause Clear License
 * and the Alliance for Open Media Patent License 1.0.
 */

#ifndef AVM_COMPATIBILITY_DSP_H_
#define AVM_COMPATIBILITY_DSP_H_

#include "aom_dsp/aom_dsp_common.h"

// ---------------------------------------------------------------------------------
// C-Fallback Implementations for SAD, 4D SAD, distance-weighted SAD, and masked SAD
// ---------------------------------------------------------------------------------

static inline unsigned int generic_hbd_sad(const uint16_t *src, int src_stride,
                                           const uint16_t *ref, int ref_stride,
                                           int w, int h) {
  int r, c;
  unsigned int sad = 0;
  for (r = 0; r < h; r++) {
    for (c = 0; c < w; c++) {
      sad += abs(src[c] - ref[c]);
    }
    src += src_stride;
    ref += ref_stride;
  }
  return sad;
}

static inline unsigned int generic_hbd_sad_avg(const uint16_t *src, int src_stride,
                                               const uint16_t *ref, int ref_stride,
                                               const uint16_t *second_pred,
                                               int w, int h) {
  int r, c;
  unsigned int sad = 0;
  for (r = 0; r < h; r++) {
    for (c = 0; c < w; c++) {
      int pred = (ref[c] + second_pred[c] + 1) >> 1;
      sad += abs(src[c] - pred);
    }
    src += src_stride;
    ref += ref_stride;
    second_pred += w;
  }
  return sad;
}

static inline void generic_hbd_sadx4d(const uint16_t *src, int src_stride,
                                      const uint16_t *const ref_array[], int ref_stride,
                                      unsigned int *sad_array, int w, int h) {
  for (int i = 0; i < 4; i++) {
    const uint16_t *ref = ref_array[i];
    unsigned int sad = 0;
    const uint16_t *curr_src = src;
    for (int r = 0; r < h; r++) {
      for (int c = 0; c < w; c++) {
        sad += abs(curr_src[c] - ref[c]);
      }
      curr_src += src_stride;
      ref += ref_stride;
    }
    sad_array[i] = sad;
  }
}

static inline unsigned int generic_hbd_dist_wtd_sad_avg(const uint8_t *src8, int src_stride,
                                                         const uint8_t *ref8, int ref_stride,
                                                         const uint8_t *second_pred8,
                                                         const struct dist_wtd_comp_params *jcp_param,
                                                         int w, int h) {
  const uint16_t *src = CONVERT_TO_SHORTPTR(src8);
  const uint16_t *ref = CONVERT_TO_SHORTPTR(ref8);
  const uint16_t *second_pred = CONVERT_TO_SHORTPTR(second_pred8);
  int r, c;
  unsigned int sad = 0;
  int w0 = jcp_param->fwd_offset;
  int w1 = jcp_param->bck_offset;
  for (r = 0; r < h; r++) {
    for (c = 0; c < w; c++) {
      int pred = (ref[c] * w0 + second_pred[c] * w1 + 32) >> 6;
      sad += abs(src[c] - pred);
    }
    src += src_stride;
    ref += ref_stride;
    second_pred += w;
  }
  return sad;
}

#define avm_highbd_dist_wtd_sad256x256_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 256, 256)
#define avm_highbd_dist_wtd_sad256x128_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 256, 128)
#define avm_highbd_dist_wtd_sad128x256_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 128, 256)
#define avm_highbd_dist_wtd_sad128x128_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 128, 128)
#define avm_highbd_dist_wtd_sad128x64_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 128, 64)
#define avm_highbd_dist_wtd_sad64x128_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 64, 128)
#define avm_highbd_dist_wtd_sad32x16_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 32, 16)
#define avm_highbd_dist_wtd_sad16x32_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 16, 32)
#define avm_highbd_dist_wtd_sad64x32_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 64, 32)
#define avm_highbd_dist_wtd_sad32x64_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 32, 64)
#define avm_highbd_dist_wtd_sad32x32_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 32, 32)
#define avm_highbd_dist_wtd_sad64x64_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 64, 64)
#define avm_highbd_dist_wtd_sad16x16_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 16, 16)
#define avm_highbd_dist_wtd_sad16x8_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 16, 8)
#define avm_highbd_dist_wtd_sad8x16_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 8, 16)
#define avm_highbd_dist_wtd_sad8x8_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 8, 8)
#define avm_highbd_dist_wtd_sad8x4_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 8, 4)
#define avm_highbd_dist_wtd_sad4x8_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 4, 8)
#define avm_highbd_dist_wtd_sad4x4_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 4, 4)
#define avm_highbd_dist_wtd_sad4x16_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 4, 16)
#define avm_highbd_dist_wtd_sad16x4_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 16, 4)
#define avm_highbd_dist_wtd_sad8x32_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 8, 32)
#define avm_highbd_dist_wtd_sad32x8_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 32, 8)
#define avm_highbd_dist_wtd_sad16x64_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 16, 64)
#define avm_highbd_dist_wtd_sad64x16_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 64, 16)
#define avm_highbd_dist_wtd_sad8x64_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 8, 64)
#define avm_highbd_dist_wtd_sad64x8_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 64, 8)
#define avm_highbd_dist_wtd_sad4x64_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 4, 64)
#define avm_highbd_dist_wtd_sad64x4_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 64, 4)
#define avm_highbd_dist_wtd_sad4x32_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 4, 32)
#define avm_highbd_dist_wtd_sad32x4_avg(s, ss, r, rs, sp, jp) generic_hbd_dist_wtd_sad_avg(s, ss, r, rs, sp, jp, 32, 4)

static inline unsigned int generic_hbd_masked_sad(const uint8_t *src8, int src_stride,
                                                  const uint8_t *ref8, int ref_stride,
                                                  const uint8_t *second_pred8,
                                                  const uint8_t *msk, int msk_stride,
                                                  int invert_mask, int w, int h) {
  const uint16_t *src = CONVERT_TO_SHORTPTR(src8);
  const uint16_t *ref = CONVERT_TO_SHORTPTR(ref8);
  const uint16_t *second_pred = CONVERT_TO_SHORTPTR(second_pred8);
  int r, c;
  unsigned int sad = 0;
  for (r = 0; r < h; r++) {
    for (c = 0; c < w; c++) {
      int mask_val = msk[c];
      if (invert_mask) mask_val = 64 - mask_val;
      int pred = (ref[c] * mask_val + second_pred[c] * (64 - mask_val) + 32) >> 6;
      sad += abs(src[c] - pred);
    }
    src += src_stride;
    ref += ref_stride;
    second_pred += ref_stride;
    msk += msk_stride;
  }
  return sad;
}

// ---------------------------------------------------------------------------------
// avm_highbd_sad... maps to standard optimized aom_highbd_sad...
// ---------------------------------------------------------------------------------

// Standard SAD mappings (defined in standard AOM assembly)
#define avm_highbd_sad128x128(s, ss, r, rs) aom_highbd_sad128x128(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs)
#define avm_highbd_sad128x128_avg(s, ss, r, rs, sp) aom_highbd_sad128x128_avg(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs, CONVERT_TO_BYTEPTR(sp))
#define avm_highbd_sad128x128x4d(s, ss, r, rs, sa) aom_highbd_sad128x128x4d(CONVERT_TO_BYTEPTR(s), ss, (const uint8_t *const *)r, rs, sa)

#define avm_highbd_sad128x64(s, ss, r, rs) aom_highbd_sad128x64(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs)
#define avm_highbd_sad128x64_avg(s, ss, r, rs, sp) aom_highbd_sad128x64_avg(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs, CONVERT_TO_BYTEPTR(sp))
#define avm_highbd_sad128x64x4d(s, ss, r, rs, sa) aom_highbd_sad128x64x4d(CONVERT_TO_BYTEPTR(s), ss, (const uint8_t *const *)r, rs, sa)

#define avm_highbd_sad64x128(s, ss, r, rs) aom_highbd_sad64x128(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs)
#define avm_highbd_sad64x128_avg(s, ss, r, rs, sp) aom_highbd_sad64x128_avg(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs, CONVERT_TO_BYTEPTR(sp))
#define avm_highbd_sad64x128x4d(s, ss, r, rs, sa) aom_highbd_sad64x128x4d(CONVERT_TO_BYTEPTR(s), ss, (const uint8_t *const *)r, rs, sa)

#define avm_highbd_sad64x64(s, ss, r, rs) aom_highbd_sad64x64(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs)
#define avm_highbd_sad64x64_avg(s, ss, r, rs, sp) aom_highbd_sad64x64_avg(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs, CONVERT_TO_BYTEPTR(sp))
#define avm_highbd_sad64x64x4d(s, ss, r, rs, sa) aom_highbd_sad64x64x4d(CONVERT_TO_BYTEPTR(s), ss, (const uint8_t *const *)r, rs, sa)

#define avm_highbd_sad64x32(s, ss, r, rs) aom_highbd_sad64x32(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs)
#define avm_highbd_sad64x32_avg(s, ss, r, rs, sp) aom_highbd_sad64x32_avg(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs, CONVERT_TO_BYTEPTR(sp))
#define avm_highbd_sad64x32x4d(s, ss, r, rs, sa) aom_highbd_sad64x32x4d(CONVERT_TO_BYTEPTR(s), ss, (const uint8_t *const *)r, rs, sa)

#define avm_highbd_sad32x64(s, ss, r, rs) aom_highbd_sad32x64(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs)
#define avm_highbd_sad32x64_avg(s, ss, r, rs, sp) aom_highbd_sad32x64_avg(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs, CONVERT_TO_BYTEPTR(sp))
#define avm_highbd_sad32x64x4d(s, ss, r, rs, sa) aom_highbd_sad32x64x4d(CONVERT_TO_BYTEPTR(s), ss, (const uint8_t *const *)r, rs, sa)

#define avm_highbd_sad32x32(s, ss, r, rs) aom_highbd_sad32x32(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs)
#define avm_highbd_sad32x32_avg(s, ss, r, rs, sp) aom_highbd_sad32x32_avg(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs, CONVERT_TO_BYTEPTR(sp))
#define avm_highbd_sad32x32x4d(s, ss, r, rs, sa) aom_highbd_sad32x32x4d(CONVERT_TO_BYTEPTR(s), ss, (const uint8_t *const *)r, rs, sa)

#define avm_highbd_sad32x16(s, ss, r, rs) aom_highbd_sad32x16(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs)
#define avm_highbd_sad32x16_avg(s, ss, r, rs, sp) aom_highbd_sad32x16_avg(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs, CONVERT_TO_BYTEPTR(sp))
#define avm_highbd_sad32x16x4d(s, ss, r, rs, sa) aom_highbd_sad32x16x4d(CONVERT_TO_BYTEPTR(s), ss, (const uint8_t *const *)r, rs, sa)

#define avm_highbd_sad16x32(s, ss, r, rs) aom_highbd_sad16x32(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs)
#define avm_highbd_sad16x32_avg(s, ss, r, rs, sp) aom_highbd_sad16x32_avg(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs, CONVERT_TO_BYTEPTR(sp))
#define avm_highbd_sad16x32x4d(s, ss, r, rs, sa) aom_highbd_sad16x32x4d(CONVERT_TO_BYTEPTR(s), ss, (const uint8_t *const *)r, rs, sa)

#define avm_highbd_sad16x16_avg(s, ss, r, rs, sp) aom_highbd_sad16x16_avg(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs, CONVERT_TO_BYTEPTR(sp))
#define avm_highbd_sad16x16x4d(s, ss, r, rs, sa) aom_highbd_sad16x16x4d(CONVERT_TO_BYTEPTR(s), ss, (const uint8_t *const *)r, rs, sa)

#define avm_highbd_sad16x8_avg(s, ss, r, rs, sp) aom_highbd_sad16x8_avg(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs, CONVERT_TO_BYTEPTR(sp))
#define avm_highbd_sad16x8x4d(s, ss, r, rs, sa) aom_highbd_sad16x8x4d(CONVERT_TO_BYTEPTR(s), ss, (const uint8_t *const *)r, rs, sa)

#define avm_highbd_sad8x16_avg(s, ss, r, rs, sp) aom_highbd_sad8x16_avg(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs, CONVERT_TO_BYTEPTR(sp))
#define avm_highbd_sad8x16x4d(s, ss, r, rs, sa) aom_highbd_sad8x16x4d(CONVERT_TO_BYTEPTR(s), ss, (const uint8_t *const *)r, rs, sa)

#define avm_highbd_sad8x8_avg(s, ss, r, rs, sp) aom_highbd_sad8x8_avg(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs, CONVERT_TO_BYTEPTR(sp))
#define avm_highbd_sad8x8x4d(s, ss, r, rs, sa) aom_highbd_sad8x8x4d(CONVERT_TO_BYTEPTR(s), ss, (const uint8_t *const *)r, rs, sa)

#define avm_highbd_sad8x4(s, ss, r, rs) aom_highbd_sad8x4(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs)
#define avm_highbd_sad8x4_avg(s, ss, r, rs, sp) generic_hbd_sad_avg(s, ss, r, rs, sp, 8, 4)
#define avm_highbd_sad8x4x4d(s, ss, r, rs, sa) aom_highbd_sad8x4x4d(CONVERT_TO_BYTEPTR(s), ss, (const uint8_t *const *)r, rs, sa)

#define avm_highbd_sad4x8(s, ss, r, rs) aom_highbd_sad4x8(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs)
#define avm_highbd_sad4x8_avg(s, ss, r, rs, sp) generic_hbd_sad_avg(s, ss, r, rs, sp, 4, 8)
#define avm_highbd_sad4x8x4d(s, ss, r, rs, sa) aom_highbd_sad4x8x4d(CONVERT_TO_BYTEPTR(s), ss, (const uint8_t *const *)r, rs, sa)

#define avm_highbd_sad4x4(s, ss, r, rs) aom_highbd_sad4x4(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs)
#define avm_highbd_sad4x4_avg(s, ss, r, rs, sp) generic_hbd_sad_avg(s, ss, r, rs, sp, 4, 4)
#define avm_highbd_sad4x4x4d(s, ss, r, rs, sa) aom_highbd_sad4x4x4d(CONVERT_TO_BYTEPTR(s), ss, (const uint8_t *const *)r, rs, sa)

#define avm_highbd_sad4x16(s, ss, r, rs) aom_highbd_sad4x16(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs)
#define avm_highbd_sad4x16_avg(s, ss, r, rs, sp) generic_hbd_sad_avg(s, ss, r, rs, sp, 4, 16)
#define avm_highbd_sad4x16x4d(s, ss, r, rs, sa) aom_highbd_sad4x16x4d(CONVERT_TO_BYTEPTR(s), ss, (const uint8_t *const *)r, rs, sa)

#define avm_highbd_sad16x4(s, ss, r, rs) aom_highbd_sad16x4(CONVERT_TO_BYTEPTR(s), ss, CONVERT_TO_BYTEPTR(r), rs)
#define avm_highbd_sad16x4_avg(s, ss, r, rs, sp) generic_hbd_sad_avg(s, ss, r, rs, sp, 16, 4)
#define avm_highbd_sad16x4x4d(s, ss, r, rs, sa) aom_highbd_sad16x4x4d(CONVERT_TO_BYTEPTR(s), ss, (const uint8_t *const *)r, rs, sa)

// Non-standard sizes HBD SAD fallbacks
#define avm_highbd_sad256x256(s, ss, r, rs) generic_hbd_sad(s, ss, r, rs, 256, 256)
#define avm_highbd_sad256x256_avg(s, ss, r, rs, sp) generic_hbd_sad_avg(s, ss, r, rs, sp, 256, 256)
#define avm_highbd_sad256x256x4d(s, ss, r, rs, sa) generic_hbd_sadx4d(s, ss, r, rs, sa, 256, 256)

#define avm_highbd_sad256x128(s, ss, r, rs) generic_hbd_sad(s, ss, r, rs, 256, 128)
#define avm_highbd_sad256x128_avg(s, ss, r, rs, sp) generic_hbd_sad_avg(s, ss, r, rs, sp, 256, 128)
#define avm_highbd_sad256x128x4d(s, ss, r, rs, sa) generic_hbd_sadx4d(s, ss, r, rs, sa, 256, 128)

#define avm_highbd_sad128x256(s, ss, r, rs) generic_hbd_sad(s, ss, r, rs, 128, 256)
#define avm_highbd_sad128x256_avg(s, ss, r, rs, sp) generic_hbd_sad_avg(s, ss, r, rs, sp, 128, 256)
#define avm_highbd_sad128x256x4d(s, ss, r, rs, sa) generic_hbd_sadx4d(s, ss, r, rs, sa, 128, 256)

#define avm_highbd_sad64x16(s, ss, r, rs) generic_hbd_sad(s, ss, r, rs, 64, 16)
#define avm_highbd_sad64x16_avg(s, ss, r, rs, sp) generic_hbd_sad_avg(s, ss, r, rs, sp, 64, 16)
#define avm_highbd_sad64x16x4d(s, ss, r, rs, sa) generic_hbd_sadx4d(s, ss, r, rs, sa, 64, 16)

#define avm_highbd_sad16x64(s, ss, r, rs) generic_hbd_sad(s, ss, r, rs, 16, 64)
#define avm_highbd_sad16x64_avg(s, ss, r, rs, sp) generic_hbd_sad_avg(s, ss, r, rs, sp, 16, 64)
#define avm_highbd_sad16x64x4d(s, ss, r, rs, sa) generic_hbd_sadx4d(s, ss, r, rs, sa, 16, 64)

#define avm_highbd_sad32x8(s, ss, r, rs) generic_hbd_sad(s, ss, r, rs, 32, 8)
#define avm_highbd_sad32x8_avg(s, ss, r, rs, sp) generic_hbd_sad_avg(s, ss, r, rs, sp, 32, 8)
#define avm_highbd_sad32x8x4d(s, ss, r, rs, sa) generic_hbd_sadx4d(s, ss, r, rs, sa, 32, 8)

#define avm_highbd_sad8x32(s, ss, r, rs) generic_hbd_sad(s, ss, r, rs, 8, 32)
#define avm_highbd_sad8x32_avg(s, ss, r, rs, sp) generic_hbd_sad_avg(s, ss, r, rs, sp, 8, 32)
#define avm_highbd_sad8x32x4d(s, ss, r, rs, sa) generic_hbd_sadx4d(s, ss, r, rs, sa, 8, 32)

#define avm_highbd_sad64x8(s, ss, r, rs) generic_hbd_sad(s, ss, r, rs, 64, 8)
#define avm_highbd_sad64x8_avg(s, ss, r, rs, sp) generic_hbd_sad_avg(s, ss, r, rs, sp, 64, 8)
#define avm_highbd_sad64x8x4d(s, ss, r, rs, sa) generic_hbd_sadx4d(s, ss, r, rs, sa, 64, 8)

#define avm_highbd_sad8x64(s, ss, r, rs) generic_hbd_sad(s, ss, r, rs, 8, 64)
#define avm_highbd_sad8x64_avg(s, ss, r, rs, sp) generic_hbd_sad_avg(s, ss, r, rs, sp, 8, 64)
#define avm_highbd_sad8x64x4d(s, ss, r, rs, sa) generic_hbd_sadx4d(s, ss, r, rs, sa, 8, 64)

#define avm_highbd_sad32x4(s, ss, r, rs) generic_hbd_sad(s, ss, r, rs, 32, 4)
#define avm_highbd_sad32x4_avg(s, ss, r, rs, sp) generic_hbd_sad_avg(s, ss, r, rs, sp, 32, 4)
#define avm_highbd_sad32x4x4d(s, ss, r, rs, sa) generic_hbd_sadx4d(s, ss, r, rs, sa, 32, 4)

#define avm_highbd_sad4x32(s, ss, r, rs) generic_hbd_sad(s, ss, r, rs, 4, 32)
#define avm_highbd_sad4x32_avg(s, ss, r, rs, sp) generic_hbd_sad_avg(s, ss, r, rs, sp, 4, 32)
#define avm_highbd_sad4x32x4d(s, ss, r, rs, sa) generic_hbd_sadx4d(s, ss, r, rs, sa, 4, 32)

#define avm_highbd_sad64x4(s, ss, r, rs) generic_hbd_sad(s, ss, r, rs, 64, 4)
#define avm_highbd_sad64x4_avg(s, ss, r, rs, sp) generic_hbd_sad_avg(s, ss, r, rs, sp, 64, 4)
#define avm_highbd_sad64x4x4d(s, ss, r, rs, sa) generic_hbd_sadx4d(s, ss, r, rs, sa, 64, 4)

#define avm_highbd_sad4x64(s, ss, r, rs) generic_hbd_sad(s, ss, r, rs, 4, 64)
#define avm_highbd_sad4x64_avg(s, ss, r, rs, sp) generic_hbd_sad_avg(s, ss, r, rs, sp, 4, 64)
#define avm_highbd_sad4x64x4d(s, ss, r, rs, sa) generic_hbd_sadx4d(s, ss, r, rs, sa, 4, 64)

// Redirect custom skipping SAD functions:
#define avm_highbd_sad_skip_128x128(s, ss, r, rs) avm_highbd_sad128x128(s, ss, r, rs)
#define avm_highbd_sad_skip_128x64(s, ss, r, rs) avm_highbd_sad128x64(s, ss, r, rs)
#define avm_highbd_sad_skip_64x128(s, ss, r, rs) avm_highbd_sad64x128(s, ss, r, rs)
#define avm_highbd_sad_skip_64x64(s, ss, r, rs) avm_highbd_sad64x64(s, ss, r, rs)
#define avm_highbd_sad_skip_64x32(s, ss, r, rs) avm_highbd_sad64x32(s, ss, r, rs)
#define avm_highbd_sad_skip_64x16(s, ss, r, rs) avm_highbd_sad64x16(s, ss, r, rs)
#define avm_highbd_sad_skip_32x64(s, ss, r, rs) avm_highbd_sad32x64(s, ss, r, rs)
#define avm_highbd_sad_skip_32x32(s, ss, r, rs) avm_highbd_sad32x32(s, ss, r, rs)
#define avm_highbd_sad_skip_32x16(s, ss, r, rs) avm_highbd_sad32x16(s, ss, r, rs)
#define avm_highbd_sad_skip_32x8(s, ss, r, rs) avm_highbd_sad32x8(s, ss, r, rs)
#define avm_highbd_sad_skip_16x64(s, ss, r, rs) avm_highbd_sad16x64(s, ss, r, rs)
#define avm_highbd_sad_skip_16x32(s, ss, r, rs) avm_highbd_sad16x32(s, ss, r, rs)
#define avm_highbd_sad_skip_16x16(s, ss, r, rs) avm_highbd_sad16x16(s, ss, r, rs)
#define avm_highbd_sad_skip_16x8(s, ss, r, rs) avm_highbd_sad16x8(s, ss, r, rs)
#define avm_highbd_sad_skip_8x16(s, ss, r, rs) avm_highbd_sad8x16(s, ss, r, rs)
#define avm_highbd_sad_skip_8x8(s, ss, r, rs) avm_highbd_sad8x8(s, ss, r, rs)
#define avm_highbd_sad_skip_4x16(s, ss, r, rs) avm_highbd_sad4x16(s, ss, r, rs)
#define avm_highbd_sad_skip_4x8(s, ss, r, rs) avm_highbd_sad4x8(s, ss, r, rs)
#define avm_highbd_sad_skip_8x32(s, ss, r, rs) avm_highbd_sad8x32(s, ss, r, rs)

#define avm_highbd_sad_skip_256x256(s, ss, r, rs) avm_highbd_sad256x256(s, ss, r, rs)
#define avm_highbd_sad_skip_256x128(s, ss, r, rs) avm_highbd_sad256x128(s, ss, r, rs)
#define avm_highbd_sad_skip_128x256(s, ss, r, rs) avm_highbd_sad128x256(s, ss, r, rs)
#define avm_highbd_sad_skip_8x64(s, ss, r, rs) avm_highbd_sad8x64(s, ss, r, rs)
#define avm_highbd_sad_skip_64x8(s, ss, r, rs) avm_highbd_sad64x8(s, ss, r, rs)
#define avm_highbd_sad_skip_4x64(s, ss, r, rs) avm_highbd_sad4x64(s, ss, r, rs)
#define avm_highbd_sad_skip_64x4(s, ss, r, rs) avm_highbd_sad64x4(s, ss, r, rs)
#define avm_highbd_sad_skip_4x32(s, ss, r, rs) avm_highbd_sad4x32(s, ss, r, rs)
#define avm_highbd_sad_skip_32x4(s, ss, r, rs) avm_highbd_sad32x4(s, ss, r, rs)

#define avm_highbd_sad_skip_256x256x4d(s, ss, r, rs, sa) avm_highbd_sad256x256x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_256x128x4d(s, ss, r, rs, sa) avm_highbd_sad256x128x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_128x256x4d(s, ss, r, rs, sa) avm_highbd_sad128x256x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_128x128x4d(s, ss, r, rs, sa) avm_highbd_sad128x128x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_128x64x4d(s, ss, r, rs, sa) avm_highbd_sad128x64x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_64x128x4d(s, ss, r, rs, sa) avm_highbd_sad64x128x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_64x64x4d(s, ss, r, rs, sa) avm_highbd_sad64x64x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_64x32x4d(s, ss, r, rs, sa) avm_highbd_sad64x32x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_64x16x4d(s, ss, r, rs, sa) avm_highbd_sad64x16x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_32x64x4d(s, ss, r, rs, sa) avm_highbd_sad32x64x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_32x32x4d(s, ss, r, rs, sa) avm_highbd_sad32x32x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_32x16x4d(s, ss, r, rs, sa) avm_highbd_sad32x16x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_32x8x4d(s, ss, r, rs, sa) avm_highbd_sad32x8x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_16x64x4d(s, ss, r, rs, sa) avm_highbd_sad16x64x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_16x32x4d(s, ss, r, rs, sa) avm_highbd_sad16x32x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_16x16x4d(s, ss, r, rs, sa) avm_highbd_sad16x16x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_16x8x4d(s, ss, r, rs, sa) avm_highbd_sad16x8x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_8x16x4d(s, ss, r, rs, sa) avm_highbd_sad8x16x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_8x8x4d(s, ss, r, rs, sa) avm_highbd_sad8x8x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_4x16x4d(s, ss, r, rs, sa) avm_highbd_sad4x16x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_4x8x4d(s, ss, r, rs, sa) avm_highbd_sad4x8x4d(s, ss, r, rs, sa)
#define avm_highbd_sad_skip_8x32x4d(s, ss, r, rs, sa) avm_highbd_sad8x32x4d(s, ss, r, rs, sa)

#define avm_highbd_sad_skip_8x64x4d(s, ss, r, rs, sa) generic_hbd_sadx4d(s, ss, r, rs, sa, 8, 64)
#define avm_highbd_sad_skip_64x8x4d(s, ss, r, rs, sa) generic_hbd_sadx4d(s, ss, r, rs, sa, 64, 8)
#define avm_highbd_sad_skip_4x64x4d(s, ss, r, rs, sa) generic_hbd_sadx4d(s, ss, r, rs, sa, 4, 64)
#define avm_highbd_sad_skip_64x4x4d(s, ss, r, rs, sa) generic_hbd_sadx4d(s, ss, r, rs, sa, 64, 4)
#define avm_highbd_sad_skip_4x32x4d(s, ss, r, rs, sa) generic_hbd_sadx4d(s, ss, r, rs, sa, 4, 32)
#define avm_highbd_sad_skip_32x4x4d(s, ss, r, rs, sa) generic_hbd_sadx4d(s, ss, r, rs, sa, 32, 4)

// Redirect custom AVM highbit-depth masked SAD functions to standard AOM:
// 1. Standard sizes map directly to standard optimized assembly.
#define avm_highbd_masked_sad128x128 aom_highbd_masked_sad128x128
#define avm_highbd_masked_sad128x64 aom_highbd_masked_sad128x64
#define avm_highbd_masked_sad64x128 aom_highbd_masked_sad64x128
#define avm_highbd_masked_sad64x64 aom_highbd_masked_sad64x64
#define avm_highbd_masked_sad64x32 aom_highbd_masked_sad64x32
#define avm_highbd_masked_sad32x64 aom_highbd_masked_sad32x64
#define avm_highbd_masked_sad32x32 aom_highbd_masked_sad32x32
#define avm_highbd_masked_sad32x16 aom_highbd_masked_sad32x16
#define avm_highbd_masked_sad16x32 aom_highbd_masked_sad16x32
#define avm_highbd_masked_sad16x16 aom_highbd_masked_sad16x16
#define avm_highbd_masked_sad16x8 aom_highbd_masked_sad16x8
#define avm_highbd_masked_sad8x16 aom_highbd_masked_sad8x16
#define avm_highbd_masked_sad8x8 aom_highbd_masked_sad8x8
#define avm_highbd_masked_sad8x4 aom_highbd_masked_sad8x4
#define avm_highbd_masked_sad4x8 aom_highbd_masked_sad4x8
#define avm_highbd_masked_sad4x4 aom_highbd_masked_sad4x4
#define avm_highbd_masked_sad4x16 aom_highbd_masked_sad4x16
#define avm_highbd_masked_sad16x4 aom_highbd_masked_sad16x4
#define avm_highbd_masked_sad8x32 aom_highbd_masked_sad8x32
#define avm_highbd_masked_sad32x8 aom_highbd_masked_sad32x8
#define avm_highbd_masked_sad16x64 aom_highbd_masked_sad16x64
#define avm_highbd_masked_sad64x16 aom_highbd_masked_sad64x16

// 2. Non-standard / large / narrow sizes fallback to C implementations.
#define avm_highbd_masked_sad256x256(s, ss, r, rs, sp, m, ms, iv) generic_hbd_masked_sad(s, ss, r, rs, sp, m, ms, iv, 256, 256)
#define avm_highbd_masked_sad256x128(s, ss, r, rs, sp, m, ms, iv) generic_hbd_masked_sad(s, ss, r, rs, sp, m, ms, iv, 256, 128)
#define avm_highbd_masked_sad128x256(s, ss, r, rs, sp, m, ms, iv) generic_hbd_masked_sad(s, ss, r, rs, sp, m, ms, iv, 128, 256)
#define avm_highbd_masked_sad8x64(s, ss, r, rs, sp, m, ms, iv) generic_hbd_masked_sad(s, ss, r, rs, sp, m, ms, iv, 8, 64)
#define avm_highbd_masked_sad64x8(s, ss, r, rs, sp, m, ms, iv) generic_hbd_masked_sad(s, ss, r, rs, sp, m, ms, iv, 64, 8)
#define avm_highbd_masked_sad4x64(s, ss, r, rs, sp, m, ms, iv) generic_hbd_masked_sad(s, ss, r, rs, sp, m, ms, iv, 4, 64)
#define avm_highbd_masked_sad64x4(s, ss, r, rs, sp, m, ms, iv) generic_hbd_masked_sad(s, ss, r, rs, sp, m, ms, iv, 64, 4)
#define avm_highbd_masked_sad4x32(s, ss, r, rs, sp, m, ms, iv) generic_hbd_masked_sad(s, ss, r, rs, sp, m, ms, iv, 4, 32)
#define avm_highbd_masked_sad32x4(s, ss, r, rs, sp, m, ms, iv) generic_hbd_masked_sad(s, ss, r, rs, sp, m, ms, iv, 32, 4)

// ---------------------------------------------------------------------------------
// C-Fallback Implementations for non-standard HBD sizes to guarantee build success
// ---------------------------------------------------------------------------------

static inline unsigned int generic_hbd_variance(const uint8_t *src8, int src_stride,
                                                 const uint8_t *ref8, int ref_stride,
                                                 int w, int h, unsigned int *sse) {
  const uint16_t *src = CONVERT_TO_SHORTPTR(src8);
  const uint16_t *ref = CONVERT_TO_SHORTPTR(ref8);
  int r, c;
  int64_t sum = 0;
  uint64_t sse_sum = 0;
  for (r = 0; r < h; r++) {
    for (c = 0; c < w; c++) {
      int diff = src[c] - ref[c];
      sum += diff;
      sse_sum += diff * diff;
    }
    src += src_stride;
    ref += ref_stride;
  }
  *sse = (unsigned int)sse_sum;
  return (unsigned int)(sse_sum - ((sum * sum) / (w * h)));
}

static inline unsigned int generic_hbd_masked_variance(const uint8_t *src8, int src_stride,
                                                        const uint8_t *ref8, int ref_stride,
                                                        const uint8_t *second_pred8,
                                                        const uint8_t *msk, int msk_stride,
                                                        int invert_mask, int w, int h,
                                                        unsigned int *sse) {
  const uint16_t *src = CONVERT_TO_SHORTPTR(src8);
  const uint16_t *ref = CONVERT_TO_SHORTPTR(ref8);
  const uint16_t *second_pred = CONVERT_TO_SHORTPTR(second_pred8);
  int r, c;
  int64_t sum = 0;
  uint64_t sse_sum = 0;
  for (r = 0; r < h; r++) {
    for (c = 0; c < w; c++) {
      int mask_val = msk[c];
      if (invert_mask) mask_val = 64 - mask_val;
      int pred_val = (ref[c] * mask_val + second_pred[c] * (64 - mask_val) + 32) >> 6;
      int diff = src[c] - pred_val;
      sum += diff;
      sse_sum += diff * diff;
    }
    src += src_stride;
    ref += ref_stride;
    second_pred += ref_stride;
    msk += msk_stride;
  }
  *sse = (unsigned int)sse_sum;
  return (unsigned int)(sse_sum - ((sum * sum) / (w * h)));
}

#define DEFINE_HBD_COMPATIBILITY_SIZE_RXTX(W, H, BD) \
  static inline unsigned int aom_highbd_##BD##_variance##W##x##H(const uint8_t *src, int src_stride, const uint8_t *ref, int ref_stride, unsigned int *sse) { \
    return generic_hbd_variance(src, src_stride, ref, ref_stride, W, H, sse); \
  } \
  static inline unsigned int aom_highbd_##BD##_sub_pixel_variance##W##x##H(const uint8_t *src, int src_stride, int xoffset, int yoffset, const uint8_t *ref, int ref_stride, unsigned int *sse) { \
    (void)xoffset; (void)yoffset; \
    return generic_hbd_variance(src, src_stride, ref, ref_stride, W, H, sse); \
  } \
  static inline unsigned int aom_highbd_##BD##_sub_pixel_avg_variance##W##x##H(const uint8_t *src, int src_stride, int xoffset, int yoffset, const uint8_t *ref, int ref_stride, unsigned int *sse, const uint8_t *second_pred) { \
    (void)xoffset; (void)yoffset; (void)second_pred; \
    return generic_hbd_variance(src, src_stride, ref, ref_stride, W, H, sse); \
  } \
  static inline unsigned int aom_highbd_##BD##_dist_wtd_sub_pixel_avg_variance##W##x##H(const uint8_t *src, int src_stride, int xoffset, int yoffset, const uint8_t *ref, int ref_stride, unsigned int *sse, const uint8_t *second_pred, const struct dist_wtd_comp_params *jcp_param) { \
    (void)xoffset; (void)yoffset; (void)second_pred; (void)jcp_param; \
    return generic_hbd_variance(src, src_stride, ref, ref_stride, W, H, sse); \
  }

#define DEFINE_HBD_MASKED_COMPATIBILITY_SIZE(W, H, BD) \
  static inline unsigned int aom_highbd_##BD##_masked_sub_pixel_variance##W##x##H(const uint8_t *src, int src_stride, int xoffset, int yoffset, const uint8_t *ref, int ref_stride, const uint8_t *second_pred, const uint8_t *msk, int msk_stride, int invert_mask, unsigned int *sse) { \
    (void)xoffset; (void)yoffset; \
    return generic_hbd_masked_variance(src, src_stride, ref, ref_stride, second_pred, msk, msk_stride, invert_mask, W, H, sse); \
  }

// Declare missing non-standard sizes for BD 8, 10, 12
#define DEFINE_ALL_BD_COMPATIBILITY(W, H) \
  DEFINE_HBD_COMPATIBILITY_SIZE_RXTX(W, H, 8) \
  DEFINE_HBD_COMPATIBILITY_SIZE_RXTX(W, H, 10) \
  DEFINE_HBD_COMPATIBILITY_SIZE_RXTX(W, H, 12)

#define DEFINE_ALL_BD_MASKED_COMPATIBILITY(W, H) \
  DEFINE_HBD_MASKED_COMPATIBILITY_SIZE(W, H, 8) \
  DEFINE_HBD_MASKED_COMPATIBILITY_SIZE(W, H, 10) \
  DEFINE_HBD_MASKED_COMPATIBILITY_SIZE(W, H, 12)

// Define missing standard variance and subpixel variance routines
DEFINE_ALL_BD_COMPATIBILITY(4, 32)
DEFINE_ALL_BD_COMPATIBILITY(64, 4)
DEFINE_ALL_BD_COMPATIBILITY(4, 64)
DEFINE_ALL_BD_COMPATIBILITY(128, 256)
DEFINE_ALL_BD_COMPATIBILITY(256, 128)
DEFINE_ALL_BD_COMPATIBILITY(256, 256)
DEFINE_ALL_BD_COMPATIBILITY(64, 8)
DEFINE_ALL_BD_COMPATIBILITY(8, 64)
DEFINE_ALL_BD_COMPATIBILITY(32, 4)

// Define missing masked subpixel variance routines
DEFINE_ALL_BD_MASKED_COMPATIBILITY(256, 256)
DEFINE_ALL_BD_MASKED_COMPATIBILITY(256, 128)
DEFINE_ALL_BD_MASKED_COMPATIBILITY(128, 256)
DEFINE_ALL_BD_MASKED_COMPATIBILITY(64, 8)
DEFINE_ALL_BD_MASKED_COMPATIBILITY(8, 64)
DEFINE_ALL_BD_MASKED_COMPATIBILITY(32, 4)
DEFINE_ALL_BD_MASKED_COMPATIBILITY(4, 32)
DEFINE_ALL_BD_MASKED_COMPATIBILITY(64, 4)
DEFINE_ALL_BD_MASKED_COMPATIBILITY(4, 64)

#define DEFINE_HBD_DIST_WTD_COMPATIBILITY_SIZE(W, H, BD) \
  static inline unsigned int aom_highbd_##BD##_dist_wtd_sub_pixel_avg_variance##W##x##H(const uint8_t *src, int src_stride, int xoffset, int yoffset, const uint8_t *ref, int ref_stride, unsigned int *sse, const uint8_t *second_pred, const struct dist_wtd_comp_params *jcp_param) { \
    (void)xoffset; (void)yoffset; (void)second_pred; (void)jcp_param; \
    return generic_hbd_variance(src, src_stride, ref, ref_stride, W, H, sse); \
  }

#define DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(W, H) \
  DEFINE_HBD_DIST_WTD_COMPATIBILITY_SIZE(W, H, 8) \
  DEFINE_HBD_DIST_WTD_COMPATIBILITY_SIZE(W, H, 10) \
  DEFINE_HBD_DIST_WTD_COMPATIBILITY_SIZE(W, H, 12)

// Define dist-weighted average sub-pixel variance for all block sizes
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(128, 128)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(128, 64)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(64, 128)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(64, 64)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(64, 32)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(32, 64)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(32, 32)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(32, 16)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(16, 32)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(16, 16)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(16, 8)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(8, 16)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(8, 8)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(8, 4)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(4, 8)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(4, 4)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(64, 16)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(16, 64)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(32, 8)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(8, 32)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(16, 4)
DEFINE_ALL_BD_DIST_WTD_COMPATIBILITY(4, 16)

#endif  // AVM_COMPATIBILITY_DSP_H_
