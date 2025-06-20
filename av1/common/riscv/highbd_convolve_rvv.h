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

#ifndef AOM_AV1_COMMON_RISCV_HIGHBD_CONVOLVE_RVV_H_
#define AOM_AV1_COMMON_RISCV_HIGHBD_CONVOLVE_RVV_H_

#include "aom_dsp/riscv/mem_rvv.h"

#include "av1/common/convolve.h"
#include "av1/common/riscv/convolve_rvv.h"

// store_strided_u16_4xN
static inline void store_strided_u16_4xN(uint16_t *addr, vuint16m1_t vdst,
                                         ptrdiff_t stride, size_t vl) {
  __riscv_vse16_v_u16m1(addr, vdst, vl >> 1);
  vdst = __riscv_vslidedown_vx_u16m1(vdst, vl >> 1, vl);
  __riscv_vse16_v_u16m1(addr + stride, vdst, vl >> 1);
}

static inline vuint16m1_t highbd_convolve12_2d_v_rvv(
    const vint16m1_t s0, const vint16m1_t s1, const vint16m1_t s2,
    const vint16m1_t s3, const vint16m1_t s4, const vint16m1_t s5,
    const vint16m1_t s6, const vint16m1_t s7, const vint16m1_t s8,
    const vint16m1_t s9, const vint16m1_t s10, const vint16m1_t s11,
    const int16_t *y_filter, const int32_t offset, const int32_t shift,
    const uint16_t max, size_t vl) {
  vint32m2_t sum = __riscv_vwmul_vx_i32m2(s0, y_filter[0], vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[1], s1, vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[2], s2, vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[3], s3, vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[4], s4, vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[5], s5, vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[6], s6, vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[7], s7, vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[8], s8, vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[9], s9, vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[10], s10, vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[11], s11, vl);
  sum = __riscv_vadd_vx_i32m2(sum, offset, vl);

  vint16m1_t i16_sum = __riscv_vnsra_wx_i16m1(sum, shift, vl);
  vint16m1_t iclip_sum =
      __riscv_vmin_vx_i16m1(__riscv_vmax_vx_i16m1(i16_sum, 0, vl), max, vl);
  return __riscv_vreinterpret_v_i16m1_u16m1(iclip_sum);
}

static inline void highbd_convolve_2d_sr_vert_12tap_rvv(
    const uint16_t *src_ptr, int src_stride, uint16_t *dst_ptr, int dst_stride,
    int w, int h, const int16_t *y_filter_ptr, ConvolveParams *conv_params,
    const int bd, const int offset, size_t vl) {
  const int32_t shift_s32 = conv_params->round_1;
  const int32_t offset_s32 = offset;
  const uint16_t max_u16 = (1 << bd) - 1;

  if (w == 4) {
    int16_t *s = (int16_t *)src_ptr;
    vl = vl << 1;

    vint16m1_t s0 = load_strided_i16_4xN(s, src_stride, vl);
    s += src_stride;
    vint16m1_t s1 = load_strided_i16_4xN(s, src_stride, vl);
    s += src_stride;
    vint16m1_t s2 = load_strided_i16_4xN(s, src_stride, vl);
    s += src_stride;
    vint16m1_t s3 = load_strided_i16_4xN(s, src_stride, vl);
    s += src_stride;
    vint16m1_t s4 = load_strided_i16_4xN(s, src_stride, vl);
    s += src_stride;
    vint16m1_t s5 = load_strided_i16_4xN(s, src_stride, vl);
    s += src_stride;
    vint16m1_t s6 = load_strided_i16_4xN(s, src_stride, vl);
    s += src_stride;
    vint16m1_t s7 = load_strided_i16_4xN(s, src_stride, vl);
    s += src_stride;
    vint16m1_t s8 = load_strided_i16_4xN(s, src_stride, vl);
    s += src_stride;
    vint16m1_t s9 = load_strided_i16_4xN(s, src_stride, vl);
    s += src_stride;

    do {
      vint16m1_t s10 = load_strided_i16_4xN(s, src_stride, vl);
      s += src_stride;
      vint16m1_t s11 = load_strided_i16_4xN(s, src_stride, vl);
      s += src_stride;
      vint16m1_t s12 = load_strided_i16_4xN(s, src_stride, vl);
      s += src_stride;
      vint16m1_t s13 = load_strided_i16_4xN(s, src_stride, vl);
      s += src_stride;

      vuint16m1_t d0 = highbd_convolve12_2d_v_rvv(
          s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, y_filter_ptr,
          offset_s32, shift_s32, max_u16, vl);
      vuint16m1_t d1 = highbd_convolve12_2d_v_rvv(
          s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, y_filter_ptr,
          offset_s32, shift_s32, max_u16, vl);

      store_strided_u16_4xN(dst_ptr, d0, dst_stride, vl);
      dst_ptr += dst_stride << 1;
      store_strided_u16_4xN(dst_ptr, d1, dst_stride, vl);
      dst_ptr += dst_stride << 1;

      s0 = s4;
      s1 = s5;
      s2 = s6;
      s3 = s7;
      s4 = s8;
      s5 = s9;
      s6 = s10;
      s7 = s11;
      s8 = s12;
      s9 = s13;

      h -= 4;
    } while (h != 0);
  } else {
    do {
      int height = h;
      int16_t *s = (int16_t *)src_ptr;
      uint16_t *d = dst_ptr;

      vint16m1_t s0 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;
      vint16m1_t s1 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;
      vint16m1_t s2 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;
      vint16m1_t s3 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;
      vint16m1_t s4 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;
      vint16m1_t s5 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;
      vint16m1_t s6 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;
      vint16m1_t s7 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;
      vint16m1_t s8 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;
      vint16m1_t s9 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;
      vint16m1_t s10 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;

      do {
        vint16m1_t s11 = __riscv_vle16_v_i16m1(s, vl);
        s += src_stride;
        vint16m1_t s12 = __riscv_vle16_v_i16m1(s, vl);
        s += src_stride;
        vint16m1_t s13 = __riscv_vle16_v_i16m1(s, vl);
        s += src_stride;
        vint16m1_t s14 = __riscv_vle16_v_i16m1(s, vl);
        s += src_stride;

        vuint16m1_t d0 = highbd_convolve12_2d_v_rvv(
            s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, y_filter_ptr,
            offset_s32, shift_s32, max_u16, vl);
        vuint16m1_t d1 = highbd_convolve12_2d_v_rvv(
            s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, y_filter_ptr,
            offset_s32, shift_s32, max_u16, vl);
        vuint16m1_t d2 = highbd_convolve12_2d_v_rvv(
            s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, y_filter_ptr,
            offset_s32, shift_s32, max_u16, vl);
        vuint16m1_t d3 = highbd_convolve12_2d_v_rvv(
            s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, y_filter_ptr,
            offset_s32, shift_s32, max_u16, vl);

        __riscv_vse16_v_u16m1(d, d0, vl);
        d += dst_stride;
        __riscv_vse16_v_u16m1(d, d1, vl);
        d += dst_stride;
        __riscv_vse16_v_u16m1(d, d2, vl);
        d += dst_stride;
        __riscv_vse16_v_u16m1(d, d3, vl);
        d += dst_stride;

        s0 = s4;
        s1 = s5;
        s2 = s6;
        s3 = s7;
        s4 = s8;
        s5 = s9;
        s6 = s10;
        s7 = s11;
        s8 = s12;
        s9 = s13;
        s10 = s14;

        height -= 4;
      } while (height != 0);

      src_ptr += vl;
      dst_ptr += vl;
      w -= vl;
    } while (w != 0);
  }
}

static inline vuint16m1_t highbd_convolve8_2d_v_rvv(
    const vint16m1_t s0, const vint16m1_t s1, const vint16m1_t s2,
    const vint16m1_t s3, const vint16m1_t s4, const vint16m1_t s5,
    const vint16m1_t s6, const vint16m1_t s7, const int16_t *y_filter,
    const int32_t offset, const int32_t shift, const uint16_t max, size_t vl) {
  vint32m2_t sum = __riscv_vwmul_vx_i32m2(s0, y_filter[0], vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[1], s1, vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[2], s2, vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[3], s3, vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[4], s4, vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[5], s5, vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[6], s6, vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[7], s7, vl);
  sum = __riscv_vadd_vx_i32m2(sum, offset, vl);

  vint16m1_t i16_sum = __riscv_vnsra_wx_i16m1(sum, shift, vl);
  vint16m1_t iclip_sum =
      __riscv_vmin_vx_i16m1(__riscv_vmax_vx_i16m1(i16_sum, 0, vl), max, vl);
  return __riscv_vreinterpret_v_i16m1_u16m1(iclip_sum);
}

static inline void highbd_convolve_2d_sr_vert_8tap_rvv(
    const uint16_t *src_ptr, int src_stride, uint16_t *dst_ptr, int dst_stride,
    int w, int h, const int16_t *y_filter_ptr, ConvolveParams *conv_params,
    int bd, const int offset, size_t vl) {
  const int32_t shift_s32 = conv_params->round_1;
  const int32_t offset_s32 = offset;
  const uint16_t max_u16 = (1 << bd) - 1;

  if (w <= 4) {
    int16_t *s = (int16_t *)src_ptr;
    vl = vl << 1;

    vint16m1_t s0 = load_strided_i16_4xN(s, src_stride, vl);
    s += src_stride;
    vint16m1_t s1 = load_strided_i16_4xN(s, src_stride, vl);
    s += src_stride;
    vint16m1_t s2 = load_strided_i16_4xN(s, src_stride, vl);
    s += src_stride;
    vint16m1_t s3 = load_strided_i16_4xN(s, src_stride, vl);
    s += src_stride;
    vint16m1_t s4 = load_strided_i16_4xN(s, src_stride, vl);
    s += src_stride;
    vint16m1_t s5 = load_strided_i16_4xN(s, src_stride, vl);
    s += src_stride;

    do {
      vint16m1_t s6 = load_strided_i16_4xN(s, src_stride, vl);
      s += src_stride;
      vint16m1_t s7 = load_strided_i16_4xN(s, src_stride, vl);
      s += src_stride;

      vuint16m1_t d0 = highbd_convolve8_2d_v_rvv(s0, s1, s2, s3, s4, s5, s6, s7,
                                                 y_filter_ptr, offset_s32,
                                                 shift_s32, max_u16, vl);

      store_strided_u16_4xN(dst_ptr, d0, dst_stride, vl);
      dst_ptr += dst_stride << 1;

      s0 = s2;
      s1 = s3;
      s2 = s4;
      s3 = s5;
      s4 = s6;
      s5 = s7;

      h -= 2;
    } while (h != 0);
  } else {
    do {
      int height = h;
      int16_t *s = (int16_t *)src_ptr;
      uint16_t *d = dst_ptr;

      vint16m1_t s0 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;
      vint16m1_t s1 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;
      vint16m1_t s2 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;
      vint16m1_t s3 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;
      vint16m1_t s4 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;
      vint16m1_t s5 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;
      vint16m1_t s6 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;

      do {
        vint16m1_t s7 = __riscv_vle16_v_i16m1(s, vl);
        vuint16m1_t d0 = highbd_convolve8_2d_v_rvv(s0, s1, s2, s3, s4, s5, s6,
                                                   s7, y_filter_ptr, offset_s32,
                                                   shift_s32, max_u16, vl);
        __riscv_vse16_v_u16m1(d, d0, vl);

        s0 = s1;
        s1 = s2;
        s2 = s3;
        s3 = s4;
        s4 = s5;
        s5 = s6;
        s6 = s7;
        s += src_stride;
        d += dst_stride;
        height--;
      } while (height != 0);

      src_ptr += vl;
      dst_ptr += vl;
      w -= vl;
    } while (w != 0);
  }
}

static inline vuint16m1_t highbd_convolve6_2d_v_rvv(
    const vint16m1_t s0, const vint16m1_t s1, const vint16m1_t s2,
    const vint16m1_t s3, const vint16m1_t s4, const vint16m1_t s5,
    const int16_t *y_filter, const int32_t offset, const int32_t shift,
    const uint16_t max, size_t vl) {
  vint32m2_t sum = __riscv_vwmul_vx_i32m2(s0, y_filter[0], vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[1], s1, vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[2], s2, vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[3], s3, vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[4], s4, vl);
  sum = __riscv_vwmacc_vx_i32m2(sum, y_filter[5], s5, vl);
  sum = __riscv_vadd_vx_i32m2(sum, offset, vl);

  vint16m1_t i16_sum = __riscv_vnsra_wx_i16m1(sum, shift, vl);
  vint16m1_t iclip_sum =
      __riscv_vmin_vx_i16m1(__riscv_vmax_vx_i16m1(i16_sum, 0, vl), max, vl);
  return __riscv_vreinterpret_v_i16m1_u16m1(iclip_sum);
}

static inline void highbd_convolve_2d_sr_vert_6tap_rvv(
    const uint16_t *src_ptr, int src_stride, uint16_t *dst_ptr, int dst_stride,
    int w, int h, const int16_t *y_filter_ptr, ConvolveParams *conv_params,
    int bd, const int offset, size_t vl) {
  const int32_t shift_s32 = conv_params->round_1;
  const int32_t offset_s32 = offset;
  const uint16_t max_u16 = (1 << bd) - 1;
  const int16_t *yfilter_6tap = y_filter_ptr + 1;

  if (w == 4) {
    int16_t *s = (int16_t *)src_ptr;
    vl = vl << 1;

    vint16m1_t s0 = load_strided_i16_4xN(s, src_stride, vl);
    s += src_stride;
    vint16m1_t s1 = load_strided_i16_4xN(s, src_stride, vl);
    s += src_stride;
    vint16m1_t s2 = load_strided_i16_4xN(s, src_stride, vl);
    s += src_stride;
    vint16m1_t s3 = load_strided_i16_4xN(s, src_stride, vl);
    s += src_stride;

    do {
      vint16m1_t s4 = load_strided_i16_4xN(s, src_stride, vl);
      s += src_stride;
      vint16m1_t s5 = load_strided_i16_4xN(s, src_stride, vl);
      s += src_stride;
      vint16m1_t s6 = load_strided_i16_4xN(s, src_stride, vl);
      s += src_stride;
      vint16m1_t s7 = load_strided_i16_4xN(s, src_stride, vl);
      s += src_stride;

      vuint16m1_t d0 =
          highbd_convolve6_2d_v_rvv(s0, s1, s2, s3, s4, s5, yfilter_6tap,
                                    offset_s32, shift_s32, max_u16, vl);
      vuint16m1_t d1 =
          highbd_convolve6_2d_v_rvv(s2, s3, s4, s5, s6, s7, yfilter_6tap,
                                    offset_s32, shift_s32, max_u16, vl);

      store_strided_u16_4xN(dst_ptr, d0, dst_stride, vl);
      dst_ptr += dst_stride << 1;
      store_strided_u16_4xN(dst_ptr, d1, dst_stride, vl);
      dst_ptr += dst_stride << 1;

      s0 = s4;
      s1 = s5;
      s2 = s6;
      s3 = s7;

      h -= 4;
    } while (h != 0);
  } else {
    do {
      int height = h;
      int16_t *s = (int16_t *)src_ptr;
      uint16_t *d = dst_ptr;

      vint16m1_t s0 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;
      vint16m1_t s1 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;
      vint16m1_t s2 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;
      vint16m1_t s3 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;
      vint16m1_t s4 = __riscv_vle16_v_i16m1(s, vl);
      s += src_stride;

      do {
        vint16m1_t s5 = __riscv_vle16_v_i16m1(s, vl);
        s += src_stride;
        vint16m1_t s6 = __riscv_vle16_v_i16m1(s, vl);
        s += src_stride;
        vint16m1_t s7 = __riscv_vle16_v_i16m1(s, vl);
        s += src_stride;
        vint16m1_t s8 = __riscv_vle16_v_i16m1(s, vl);
        s += src_stride;

        vuint16m1_t d0 =
            highbd_convolve6_2d_v_rvv(s0, s1, s2, s3, s4, s5, yfilter_6tap,
                                      offset_s32, shift_s32, max_u16, vl);
        vuint16m1_t d1 =
            highbd_convolve6_2d_v_rvv(s1, s2, s3, s4, s5, s6, yfilter_6tap,
                                      offset_s32, shift_s32, max_u16, vl);
        vuint16m1_t d2 =
            highbd_convolve6_2d_v_rvv(s2, s3, s4, s5, s6, s7, yfilter_6tap,
                                      offset_s32, shift_s32, max_u16, vl);
        vuint16m1_t d3 =
            highbd_convolve6_2d_v_rvv(s3, s4, s5, s6, s7, s8, yfilter_6tap,
                                      offset_s32, shift_s32, max_u16, vl);

        __riscv_vse16_v_u16m1(d, d0, vl);
        d += dst_stride;
        __riscv_vse16_v_u16m1(d, d1, vl);
        d += dst_stride;
        __riscv_vse16_v_u16m1(d, d2, vl);
        d += dst_stride;
        __riscv_vse16_v_u16m1(d, d3, vl);
        d += dst_stride;

        s0 = s4;
        s1 = s5;
        s2 = s6;
        s3 = s7;
        s4 = s8;

        height -= 4;
      } while (height != 0);

      src_ptr += vl;
      dst_ptr += vl;
      w -= vl;
    } while (w != 0);
  }
}

#endif  // AOM_AV1_COMMON_RISCV_HIGHBD_CONVOLVE_RVV_H_
