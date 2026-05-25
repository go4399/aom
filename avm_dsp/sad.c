#include <stdlib.h>
#include "config/avm_config.h"
#include "config/avm_dsp_rtcd.h"
#include "avm/avm_integer.h"
#include "avm_ports/mem.h"
#include "avm_dsp/blend.h"

static INLINE unsigned int highbd_sad(const uint16_t *a, int a_stride,
                                      const uint16_t *b, int b_stride,
                                      int width, int height) {
  int y, x;
  unsigned int sad = 0;
  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      sad += abs(a[x] - b[x]);
    }
    a += a_stride;
    b += b_stride;
  }
  return sad;
}

static INLINE unsigned int highbd_sad_ds(const uint16_t *a, int a_stride,
                                          const uint16_t *b, int b_stride,
                                          int width, int height) {
  int y, x;
  unsigned int sad = 0;
  for (y = 0; y < height; y += 2) {
    for (x = 0; x < width; x++) {
      sad += abs(a[x] - b[x]);
    }
    a += a_stride << 1;
    b += b_stride << 1;
  }
  return sad;
}

#define highbd_sadMxN(m, n)                                     \
  unsigned int avm_highbd_sad##m##x##n##_c(                     \
      const uint16_t *src, int src_stride, const uint16_t *ref, \
      int ref_stride) {                                         \
    return highbd_sad(src, src_stride, ref, ref_stride, m, n);  \
  }

#define highbd_sadMxN_ds(m, n)                                    \
  unsigned int avm_highbd_sad##m##x##n##_ds_c(                    \
      const uint16_t *src, int src_stride, const uint16_t *ref,   \
      int ref_stride) {                                           \
    return highbd_sad_ds(src, src_stride, ref, ref_stride, m, n); \
  }

highbd_sadMxN_ds(16, 16);
highbd_sadMxN_ds(16, 8);
highbd_sadMxN_ds(8, 8);
highbd_sadMxN_ds(8, 16);
highbd_sadMxN_ds(12, 20);
highbd_sadMxN_ds(20, 12);
highbd_sadMxN_ds(12, 12);
highbd_sadMxN_ds(20, 20);

highbd_sadMxN(256, 256);
highbd_sadMxN(256, 128);
highbd_sadMxN(128, 256);
highbd_sadMxN(128, 128);
highbd_sadMxN(128, 64);
highbd_sadMxN(64, 128);
highbd_sadMxN(64, 64);
highbd_sadMxN(64, 32);
highbd_sadMxN(32, 64);
highbd_sadMxN(32, 32);
highbd_sadMxN(32, 16);
highbd_sadMxN(16, 32);
highbd_sadMxN(16, 16);
highbd_sadMxN(16, 8);
highbd_sadMxN(8, 16);
highbd_sadMxN(8, 8);
highbd_sadMxN(8, 4);
highbd_sadMxN(4, 8);
highbd_sadMxN(4, 4);

highbd_sadMxN(12, 12);
highbd_sadMxN(12, 20);
highbd_sadMxN(20, 12);
highbd_sadMxN(20, 20);

highbd_sadMxN(4, 16);
highbd_sadMxN(16, 4);
highbd_sadMxN(8, 32);
highbd_sadMxN(32, 8);
highbd_sadMxN(16, 64);
highbd_sadMxN(64, 16);

highbd_sadMxN(4, 32);
highbd_sadMxN(32, 4);
highbd_sadMxN(8, 64);
highbd_sadMxN(64, 8);
highbd_sadMxN(4, 64);
highbd_sadMxN(64, 4);
