#include <assert.h>
#include <string.h>

#include "config/aom_config.h"
#include "config/aom_scale_rtcd.h"
#include "config/aom_dsp_rtcd.h"

#include "avm/avm_integer.h"
#include "aom_dsp/aom_dsp_common.h"
#include "aom_dsp/intrapred_common.h"
#include "aom_mem/aom_mem.h"

#define IBP_WEIGHT_SHIFT 14
#define IBP_WEIGHT_REF 16384

static const uint8_t ibp_weights[5][16] = {
  { 96, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 86, 107, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 77, 90, 102, 115, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 71, 78, 86, 92, 100, 107, 114, 121, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 68, 72, 76, 79, 83, 87, 90, 94, 98, 102, 106, 109, 113, 117, 121, 124 }
};

static const uint8_t size_to_weights_index[9] = { 0, 1, 2, 0, 3, 0, 0, 0, 4 };

static inline int abs_diff(int a, int b) { return (a > b) ? a - b : b - a; }

static inline uint16_t paeth_predictor_single(uint16_t left, uint16_t top,
                                              uint16_t top_left) {
  const int base = top + left - top_left;
  const int p_left = abs_diff(base, left);
  const int p_top = abs_diff(base, top);
  const int p_top_left = abs_diff(base, top_left);

  return (p_left <= p_top && p_left <= p_top_left) ? left
         : (p_top <= p_top_left)                   ? top
                                                   : top_left;
}

#define BLEND_WEIGHT_MAX 32
static const uint8_t blk_size_log2[65] = {
  0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
  5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6
};

#define divide_round(value, bits) (((value) + (1 << ((bits) - 1))) >> (bits))

// Standard Generic Helpers implemented inside avm_dsp/intrapred.c to support any aspect ratios:

static INLINE void highbd_v_predictor(uint16_t *dst, ptrdiff_t stride, int bw,
                                      int bh, const uint16_t *above,
                                      const uint16_t *left, int bd) {
  int r;
  (void)left;
  (void)bd;
  for (r = 0; r < bh; r++) {
    memcpy(dst, above, bw * sizeof(uint16_t));
    dst += stride;
  }
}

static INLINE void highbd_h_predictor(uint16_t *dst, ptrdiff_t stride, int bw,
                                      int bh, const uint16_t *above,
                                      const uint16_t *left, int bd) {
  int r;
  (void)above;
  (void)bd;
  for (r = 0; r < bh; r++) {
    aom_memset16(dst, left[r], bw);
    dst += stride;
  }
}

static INLINE void highbd_paeth_predictor(uint16_t *dst, ptrdiff_t stride,
                                          int bw, int bh, const uint16_t *above,
                                          const uint16_t *left, int bd) {
  int r, c;
  const uint16_t ytop_left = above[-1];
  (void)bd;

  for (r = 0; r < bh; r++) {
    for (c = 0; c < bw; c++)
      dst[c] = paeth_predictor_single(left[r], above[c], ytop_left);
    dst += stride;
  }
}

static INLINE void highbd_smooth_predictor(uint16_t *dst, ptrdiff_t stride,
                                           int bw, int bh,
                                           const uint16_t *above,
                                           const uint16_t *left, int bd) {
  (void)bd;
  const uint16_t bl = left[bh];   // estimated by bottom-left pixel
  const uint16_t tr = above[bw];  // estimated by top-right pixel

  uint16_t *pred = dst;
  const int scale =
      ROUND_POWER_OF_TWO((blk_size_log2[bh] - 2 + blk_size_log2[bw] - 2), 2);
  assert(scale >= 0 && scale <= BLEND_WEIGHT_MAX - 1);
  for (int r = 0; r < bh; r++) {
    const int s_top =
        BLEND_WEIGHT_MAX >>
        AOMMIN(blk_size_log2[BLEND_WEIGHT_MAX << 1], ((r << 1) >> scale));
    const uint16_t l = left[r];
    for (int c = 0; c < bw; c++) {
      const int s_left =
          BLEND_WEIGHT_MAX >>
          AOMMIN(blk_size_log2[BLEND_WEIGHT_MAX << 1], ((c << 1) >> scale));
      const uint16_t top = above[c];
      const int blend_max_log2 = blk_size_log2[BLEND_WEIGHT_MAX];
      uint16_t predv =
          bl + divide_round(((int16_t)above[c] - (int16_t)bl) * (bh - 1 - r),
                            blk_size_log2[bh]);
      uint16_t predh =
          tr + divide_round(((int16_t)left[r] - (int16_t)tr) * (bw - 1 - c),
                            blk_size_log2[bw]);
      predv = predv + divide_round(((int16_t)top - (int16_t)predv) * s_top,
                                   (blend_max_log2 + 1));
      predh = predh + divide_round(((int16_t)l - (int16_t)predh) * s_left,
                                   (blend_max_log2 + 1));
      pred[c] = divide_round((predv + predh), 1);
    }
    pred += stride;
  }
}

static INLINE void highbd_smooth_v_predictor(uint16_t *dst, ptrdiff_t stride,
                                             int bw, int bh,
                                             const uint16_t *above,
                                             const uint16_t *left, int bd) {
  (void)bd;
  const uint16_t bl = left[bh];  // estimated by bottom-left pixel

  uint16_t *pred = dst;
  const int scale =
      ROUND_POWER_OF_TWO((blk_size_log2[bh] - 2 + blk_size_log2[bw] - 2), 2);
  assert(scale >= 0 && scale <= BLEND_WEIGHT_MAX - 1);
  for (int r = 0; r < bh; ++r) {
    const int s_top =
        BLEND_WEIGHT_MAX >>
        AOMMIN(blk_size_log2[BLEND_WEIGHT_MAX << 1], ((r << 1) >> scale));
    for (int c = 0; c < bw; ++c) {
      const uint16_t top = above[c];
      const int blend_max_log2 = blk_size_log2[BLEND_WEIGHT_MAX];
      uint16_t predv =
          bl + divide_round(((int16_t)above[c] - (int16_t)bl) * (bh - 1 - r),
                            blk_size_log2[bh]);
      pred[c] = predv + divide_round(((int16_t)top - (int16_t)predv) * s_top,
                                     (blend_max_log2 + 1));
    }
    pred += stride;
  }
}

static INLINE void highbd_smooth_h_predictor(uint16_t *dst, ptrdiff_t stride,
                                             int bw, int bh,
                                             const uint16_t *above,
                                             const uint16_t *left, int bd) {
  (void)bd;
  const uint16_t tr = above[bw];  // estimated by top-right pixel

  uint16_t *pred = dst;
  const int scale =
      ROUND_POWER_OF_TWO((blk_size_log2[bh] - 2 + blk_size_log2[bw] - 2), 2);
  assert(scale >= 0 && scale <= BLEND_WEIGHT_MAX - 1);
  for (int r = 0; r < bh; r++) {
    const uint16_t l = left[r];
    for (int c = 0; c < bw; c++) {
      const int s_left =
          BLEND_WEIGHT_MAX >>
          AOMMIN(blk_size_log2[BLEND_WEIGHT_MAX << 1], ((c << 1) >> scale));
      const int blend_max_log2 = blk_size_log2[BLEND_WEIGHT_MAX];
      uint16_t predh =
          tr + divide_round(((int16_t)left[r] - (int16_t)tr) * (bw - 1 - c),
                            blk_size_log2[bw]);
      pred[c] = predh + divide_round(((int16_t)l - (int16_t)predh) * s_left,
                                     (blend_max_log2 + 1));
    }
    pred += stride;
  }
}

static INLINE void highbd_dc_128_predictor(uint16_t *dst, ptrdiff_t stride,
                                           int bw, int bh,
                                           const uint16_t *above,
                                           const uint16_t *left, int bd) {
  int r;
  (void)above;
  (void)left;

  for (r = 0; r < bh; r++) {
    aom_memset16(dst, 128 << (bd - 8), bw);
    dst += stride;
  }
}

static INLINE void highbd_dc_left_predictor(uint16_t *dst, ptrdiff_t stride,
                                            int bw, int bh,
                                            const uint16_t *above,
                                            const uint16_t *left, int bd) {
  int i, r, expected_dc, sum = 0;
  (void)above;
  (void)bd;

  for (i = 0; i < bh; i++) sum += left[i];
  expected_dc = (sum + (bh >> 1)) / bh;

  for (r = 0; r < bh; r++) {
    aom_memset16(dst, expected_dc, bw);
    dst += stride;
  }
}

static INLINE void highbd_dc_top_predictor(uint16_t *dst, ptrdiff_t stride,
                                           int bw, int bh,
                                           const uint16_t *above,
                                           const uint16_t *left, int bd) {
  int i, r, expected_dc, sum = 0;
  (void)left;
  (void)bd;

  for (i = 0; i < bw; i++) sum += above[i];
  expected_dc = (sum + (bw >> 1)) / bw;

  for (r = 0; r < bh; r++) {
    aom_memset16(dst, expected_dc, bw);
    dst += stride;
  }
}

static INLINE void highbd_dc_predictor(uint16_t *dst, ptrdiff_t stride, int bw,
                                       int bh, const uint16_t *above,
                                       const uint16_t *left, int bd) {
  int i, r, expected_dc, sum = 0;
  const int count = bw + bh;
  (void)bd;

  for (i = 0; i < bw; i++) {
    sum += above[i];
  }
  for (i = 0; i < bh; i++) {
    sum += left[i];
  }

  expected_dc = (sum + (count >> 1)) / count;

  for (r = 0; r < bh; r++) {
    aom_memset16(dst, expected_dc, bw);
    dst += stride;
  }
}

static INLINE void highbd_ibp_dc_left_predictor(uint16_t *dst, ptrdiff_t stride,
                                                int bw, int bh,
                                                const uint16_t *above,
                                                const uint16_t *left, int bd) {
  int r, c;
  (void)above;
  (void)bd;

  int len = bw >> 2;
  const uint8_t weights_index = size_to_weights_index[bw >> 3];
  const uint8_t *weights = ibp_weights[weights_index];
  for (r = 0; r < bh; r++) {
    for (c = 0; c < len; c++) {
      int val = ROUND_POWER_OF_TWO(
          left[r] * (IBP_WEIGHT_REF - weights[c]) + dst[c] * weights[c],
          IBP_WEIGHT_SHIFT);
      dst[c] = val;
    }
    dst += stride;
  }
}

static INLINE void highbd_ibp_dc_top_predictor(uint16_t *dst, ptrdiff_t stride,
                                               int bw, int bh,
                                               const uint16_t *above,
                                               const uint16_t *left, int bd) {
  int r, c;
  (void)left;
  (void)bd;

  int len = bh >> 2;
  const uint8_t weights_index = size_to_weights_index[bh >> 3];
  const uint8_t *weights = ibp_weights[weights_index];
  for (r = 0; r < len; r++) {
    for (c = 0; c < bw; c++) {
      int val = ROUND_POWER_OF_TWO(
          above[c] * (IBP_WEIGHT_REF - weights[r]) + dst[c] * weights[r],
          IBP_WEIGHT_SHIFT);
      dst[c] = val;
    }
    dst += stride;
  }
}

static INLINE void highbd_ibp_dc_predictor(uint16_t *dst, ptrdiff_t stride,
                                           int bw, int bh,
                                           const uint16_t *above,
                                           const uint16_t *left, int bd) {
  int r, c;
  (void)bd;

  uint16_t *orig_dst = dst;
  int len_h = bh >> 2;
  int len_w = bw >> 2;
  int row_start = 0;
  int col_start = 0;
  if (bw >= bh)
    row_start = len_h;
  else
    col_start = len_w;
  uint8_t weights_index = size_to_weights_index[bh >> 3];
  const uint8_t *weights = ibp_weights[weights_index];
  for (r = 0; r < len_h; r++) {
    for (c = col_start; c < bw; c++) {
      int val = ROUND_POWER_OF_TWO(
          above[c] * (IBP_WEIGHT_REF - weights[r]) + dst[c] * weights[r],
          IBP_WEIGHT_SHIFT);
      dst[c] = val;
    }
    dst += stride;
  }
  dst = orig_dst + row_start * stride;
  weights_index = size_to_weights_index[bw >> 3];
  weights = ibp_weights[weights_index];
  for (r = row_start; r < bh; r++) {
    for (c = 0; c < len_w; c++) {
      int val = ROUND_POWER_OF_TWO(
          left[r] * (IBP_WEIGHT_REF - weights[c]) + dst[c] * weights[c],
          IBP_WEIGHT_SHIFT);
      dst[c] = val;
    }
    dst += stride;
  }
}

static INLINE void ibp_dc_left_predictor(uint8_t *dst, ptrdiff_t stride, int bw,
                                         int bh, const uint8_t *above,
                                         const uint8_t *left) {
  int r, c;
  (void)above;

  const uint8_t weights_index = size_to_weights_index[bw >> 3];
  const uint8_t *weights = ibp_weights[weights_index];
  int len = bw >> 2;
  for (r = 0; r < bh; r++) {
    for (c = 0; c < len; c++) {
      int val = ROUND_POWER_OF_TWO(
          left[r] * (IBP_WEIGHT_REF - weights[c]) + dst[c] * weights[c],
          IBP_WEIGHT_SHIFT);
      dst[c] = val;
    }
    dst += stride;
  }
}

static INLINE void ibp_dc_top_predictor(uint8_t *dst, ptrdiff_t stride, int bw,
                                        int bh, const uint8_t *above,
                                        const uint8_t *left) {
  int r, c;
  (void)left;

  const uint8_t weights_index = size_to_weights_index[bh >> 3];
  const uint8_t *weights = ibp_weights[weights_index];
  int len = bh >> 2;
  for (r = 0; r < len; r++) {
    for (c = 0; c < bw; c++) {
      int val = ROUND_POWER_OF_TWO(
          above[c] * (IBP_WEIGHT_REF - weights[r]) + dst[c] * weights[r],
          IBP_WEIGHT_SHIFT);
      dst[c] = val;
    }
    dst += stride;
  }
}

static INLINE void ibp_dc_predictor(uint8_t *dst, ptrdiff_t stride, int bw,
                                    int bh, const uint8_t *above,
                                    const uint8_t *left) {
  int r, c;
  uint8_t *orig_dst = dst;
  uint8_t weights_index = size_to_weights_index[bh >> 3];
  const uint8_t *weights = ibp_weights[weights_index];
  int len_w = bw >> 2;
  int len_h = bh >> 2;
  int row_start = 0;
  int col_start = 0;
  if (bw >= bh)
    row_start = len_h;
  else
    col_start = len_w;
  for (r = 0; r < len_h; r++) {
    for (c = col_start; c < bw; c++) {
      int val = ROUND_POWER_OF_TWO(
          above[c] * (IBP_WEIGHT_REF - weights[r]) + dst[c] * weights[r],
          IBP_WEIGHT_SHIFT);
      dst[c] = val;
    }
    dst += stride;
  }
  dst = orig_dst + row_start * stride;
  weights_index = size_to_weights_index[bw >> 3];
  weights = ibp_weights[weights_index];
  for (r = row_start; r < bh; r++) {
    for (c = 0; c < len_w; c++) {
      int val = ROUND_POWER_OF_TWO(
          left[r] * (IBP_WEIGHT_REF - weights[c]) + dst[c] * weights[c],
          IBP_WEIGHT_SHIFT);
      dst[c] = val;
    }
    dst += stride;
  }
}

// Macro expansions to dynamically generate function declarations:

#define intra_pred_sized(type, width, height)                  \
  void aom_##type##_predictor_##width##x##height##_c(          \
      uint8_t *dst, ptrdiff_t stride, const uint8_t *above,    \
      const uint8_t *left) {                                   \
    type##_predictor(dst, stride, width, height, above, left); \
  }

#define intra_pred_highbd_sized(type, width, height)                        \
  void aom_highbd_##type##_predictor_##width##x##height##_c(                \
      uint16_t *dst, ptrdiff_t stride, const uint16_t *above,               \
      const uint16_t *left, int bd) {                                       \
    highbd_##type##_predictor(dst, stride, width, height, above, left, bd); \
  }

// Extra Sizes to generate standard predictors:
#define intra_pred_extra_sizes(type) \
  intra_pred_highbd_sized(type, 4, 32) \
  intra_pred_highbd_sized(type, 32, 4) \
  intra_pred_highbd_sized(type, 8, 64) \
  intra_pred_highbd_sized(type, 64, 8) \
  intra_pred_highbd_sized(type, 4, 64) \
  intra_pred_highbd_sized(type, 64, 4)

intra_pred_extra_sizes(v)
intra_pred_extra_sizes(h)
intra_pred_extra_sizes(paeth)
intra_pred_extra_sizes(smooth)
intra_pred_extra_sizes(smooth_v)
intra_pred_extra_sizes(smooth_h)
intra_pred_extra_sizes(dc_128)
intra_pred_extra_sizes(dc_top)
intra_pred_extra_sizes(dc_left)
intra_pred_extra_sizes(dc)

// Expansion list for custom AV2 predictors:
#define intra_pred_rectangular(type) \
  intra_pred_sized(type, 4, 8) \
  intra_pred_sized(type, 8, 4) \
  intra_pred_sized(type, 8, 16) \
  intra_pred_sized(type, 16, 8) \
  intra_pred_sized(type, 16, 32) \
  intra_pred_sized(type, 32, 16) \
  intra_pred_sized(type, 32, 64) \
  intra_pred_sized(type, 64, 32) \
  intra_pred_sized(type, 4, 16) \
  intra_pred_sized(type, 16, 4) \
  intra_pred_sized(type, 8, 32) \
  intra_pred_sized(type, 32, 8) \
  intra_pred_sized(type, 16, 64) \
  intra_pred_sized(type, 64, 16) \
  intra_pred_sized(type, 4, 32) \
  intra_pred_sized(type, 32, 4) \
  intra_pred_sized(type, 8, 64) \
  intra_pred_sized(type, 64, 8) \
  intra_pred_sized(type, 4, 64) \
  intra_pred_sized(type, 64, 4) \
  intra_pred_highbd_sized(type, 4, 8) \
  intra_pred_highbd_sized(type, 8, 4) \
  intra_pred_highbd_sized(type, 8, 16) \
  intra_pred_highbd_sized(type, 16, 8) \
  intra_pred_highbd_sized(type, 16, 32) \
  intra_pred_highbd_sized(type, 32, 16) \
  intra_pred_highbd_sized(type, 32, 64) \
  intra_pred_highbd_sized(type, 64, 32) \
  intra_pred_highbd_sized(type, 4, 16) \
  intra_pred_highbd_sized(type, 16, 4) \
  intra_pred_highbd_sized(type, 8, 32) \
  intra_pred_highbd_sized(type, 32, 8) \
  intra_pred_highbd_sized(type, 16, 64) \
  intra_pred_highbd_sized(type, 64, 16) \
  intra_pred_highbd_sized(type, 4, 32) \
  intra_pred_highbd_sized(type, 32, 4) \
  intra_pred_highbd_sized(type, 8, 64) \
  intra_pred_highbd_sized(type, 64, 8) \
  intra_pred_highbd_sized(type, 4, 64) \
  intra_pred_highbd_sized(type, 64, 4)

#define intra_pred_above_4x4(type) \
  intra_pred_sized(type, 8, 8) \
  intra_pred_sized(type, 16, 16) \
  intra_pred_sized(type, 32, 32) \
  intra_pred_sized(type, 64, 64) \
  intra_pred_highbd_sized(type, 4, 4) \
  intra_pred_highbd_sized(type, 8, 8) \
  intra_pred_highbd_sized(type, 16, 16) \
  intra_pred_highbd_sized(type, 32, 32) \
  intra_pred_highbd_sized(type, 64, 64) \
  intra_pred_rectangular(type)

#define intra_pred_allsizes(type) \
  intra_pred_sized(type, 4, 4) \
  intra_pred_above_4x4(type)

intra_pred_allsizes(ibp_dc_left)
intra_pred_allsizes(ibp_dc_top)
intra_pred_allsizes(ibp_dc)

static void highbd_img_upshift(avm_image_t *dst, const avm_image_t *src,
                               int input_shift) {
  const int offset = 0;
  int plane;
  assert(dst->d_w == src->d_w && dst->d_h == src->d_h &&
      dst->x_chroma_shift == src->x_chroma_shift &&
      dst->y_chroma_shift == src->y_chroma_shift && dst->fmt == src->fmt &&
      input_shift >= 0);
  for (plane = 0; plane < (dst->monochrome ? 1 : 3); plane++) {
    int w = src->d_w;
    int h = src->d_h;
    int x, y;
    if (plane) {
      w = (w + src->x_chroma_shift) >> src->x_chroma_shift;
      h = (h + src->y_chroma_shift) >> src->y_chroma_shift;
      if (src->monochrome) {
        for (y = 0; y < h; y++) {
          uint16_t *p_dst =
              (uint16_t *)(dst->planes[plane] + y * dst->stride[plane]);
          for (x = 0; x < w; x++) *p_dst++ = offset;
        }
        continue;
      }
    }
    for (y = 0; y < h; y++) {
      const uint16_t *p_src =
          (const uint16_t *)(src->planes[plane] + y * src->stride[plane]);
      uint16_t *p_dst =
          (uint16_t *)(dst->planes[plane] + y * dst->stride[plane]);
      for (x = 0; x < w; x++) *p_dst++ = (*p_src++ << input_shift) + offset;
    }
  }
}

static void lowbd_img_upshift(avm_image_t *dst, const avm_image_t *src,
                              int input_shift) {
  const int offset = 0;
  int plane;
  assert(dst->d_w == src->d_w && dst->d_h == src->d_h &&
      dst->x_chroma_shift == src->x_chroma_shift &&
      dst->y_chroma_shift == src->y_chroma_shift &&
      dst->fmt == src->fmt + AVM_IMG_FMT_HIGHBITDEPTH && input_shift >= 0);
  for (plane = 0; plane < (dst->monochrome ? 1 : 3); plane++) {
    int w = src->d_w;
    int h = src->d_h;
    int x, y;
    if (plane) {
      w = (w + src->x_chroma_shift) >> src->x_chroma_shift;
      h = (h + src->y_chroma_shift) >> src->y_chroma_shift;
      if (src->monochrome) {
        for (y = 0; y < h; y++) {
          uint16_t *p_dst =
              (uint16_t *)(dst->planes[plane] + y * dst->stride[plane]);
          for (x = 0; x < w; x++) *p_dst++ = offset;
        }
        continue;
      }
    }
    for (y = 0; y < h; y++) {
      const uint8_t *p_src = src->planes[plane] + y * src->stride[plane];
      uint16_t *p_dst =
          (uint16_t *)(dst->planes[plane] + y * dst->stride[plane]);
      for (x = 0; x < w; x++) {
        *p_dst++ = (*p_src++ << input_shift) + offset;
      }
    }
  }
}

void avm_img_upshift(avm_image_t *dst, const avm_image_t *src,
                     int input_shift) {
  if (src->fmt & AVM_IMG_FMT_HIGHBITDEPTH) {
    highbd_img_upshift(dst, src, input_shift);
  } else {
    lowbd_img_upshift(dst, src, input_shift);
  }
}
