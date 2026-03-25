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

// Simple Encoder
// ==============
//
// This is an example of a simple encoder loop. It takes an input file in
// YV12 format, passes it through the encoder, and writes the compressed
// frames to disk in IVF format. Other decoder examples build upon this
// one.
//
// The details of the IVF format have been elided from this example for
// simplicity of presentation, as IVF files will not generally be used by
// your application. In general, an IVF file consists of a file header,
// followed by a variable number of frames. Each frame consists of a frame
// header followed by a variable length payload. The length of the payload
// is specified in the first four bytes of the frame header. The payload is
// the raw compressed data.
//
// Standard Includes
// -----------------
// For encoders, you only have to include `aom_encoder.h` and then any
// header files for the specific codecs you use. In this case, we're using
// aom.
//
// Getting The Default Configuration
// ---------------------------------
// Encoders have the notion of "usage profiles." For example, an encoder
// may want to publish default configurations for both a video
// conferencing application and a best quality offline encoder. These
// obviously have very different default settings. Consult the
// documentation for your codec to see if it provides any default
// configurations. All codecs provide a default configuration, number 0,
// which is valid for material in the vacinity of QCIF/QVGA.
//
// Updating The Configuration
// ---------------------------------
// Almost all applications will want to update the default configuration
// with settings specific to their usage. Here we set the width and height
// of the video file to that specified on the command line. We also scale
// the default bitrate based on the ratio between the default resolution
// and the resolution specified on the command line.
//
// Initializing The Codec
// ----------------------
// The encoder is initialized by the following code.
//
// Encoding A Frame
// ----------------
// The frame is read as a continuous block (size width * height * 3 / 2)
// from the input file. If a frame was read (the input file has not hit
// EOF) then the frame is passed to the encoder. Otherwise, a NULL
// is passed, indicating the End-Of-Stream condition to the encoder. The
// `frame_cnt` is reused as the presentation time stamp (PTS) and each
// frame is shown for one frame-time in duration. The flags parameter is
// unused in this example.

// Forced Keyframes
// ----------------
// Keyframes can be forced by setting the AOM_EFLAG_FORCE_KF bit of the
// flags passed to `aom_codec_control()`. In this example, we force a
// keyframe every <keyframe-interval> frames. Note, the output stream can
// contain additional keyframes beyond those that have been forced using the
// AOM_EFLAG_FORCE_KF flag because of automatic keyframe placement by the
// encoder.
//
// Processing The Encoded Data
// ---------------------------
// Each packet of type `AOM_CODEC_CX_FRAME_PKT` contains the encoded data
// for this frame. We write a IVF frame header, followed by the raw data.
//
// Cleanup
// -------
// The `aom_codec_destroy` call frees any memory allocated by the codec.
//
// Error Handling
// --------------
// This example does not special case any error return codes. If there was
// an error, a descriptive message is printed and the program exits. With
// few exeptions, aom_codec functions return an enumerated error status,
// with the value `0` indicating success.
//
// Error Resiliency Features
// -------------------------
// Error resiliency is controlled by the g_error_resilient member of the
// configuration structure. Use the `decode_with_drops` example to decode with
// frames 5-10 dropped. Compare the output for a file encoded with this example
// versus one encoded with the `simple_encoder` example.

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aom/aom_encoder.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "av1/common/blockd.h"
#include "common/tools_common.h"
#include "common/video_writer.h"

static const char *exec_name;

void usage_exit(void) {
  fprintf(stderr,
          "Usage: %s <codec> <width> <height> <infile> <outfile> "
          "<partitionfile> <keyframe-interval> <frames to encode>\n"
          "See comments in simple_encoder.c for more information.\n",
          exec_name);
  exit(EXIT_FAILURE);
}

const int kVersion = 1;

typedef struct TestData {
  int version;
} TestData;

typedef struct ToyModel {
  TestData *data;
  aom_ext_part_config_t config;
  aom_ext_part_funcs_t funcs;
  int mi_row;
  int mi_col;
  int frame_width;
  int frame_height;
  BLOCK_SIZE block_size;
  int frame_num;
} ToyModel;

// 64x64: 1 node
// 32x32: 4 nodes (only the first one will further split)
// 16x16: 4 nodes
// 8x8:   4 * 4 nodes
// 4x4:   4 * 4 * 4 nodes
// const int num_blocks = 1 + 4 + 4 + 4 * 4 + 4 * 4 * 4;
typedef struct SuperBlockPartitions {
  int sbx;
  int sby;
  int num_partitions;
  int partition_tree[1 + 4 + 4 * 4 + 4 * 4 * 4 + 4 * 4 * 4 * 4];
} SuperBlockPartitions;

typedef struct FramePartitions {
  int num_sbs;
  int sbw;
  int sbh;
  SuperBlockPartitions *partition_tree;
} FramePartitions;

// Global array for storing superblock parttions.
// static SuperBlockPartitions * ext_partition_tree;
static FramePartitions *ext_partition_tree;

static int ext_framecount = 0;

static int partition_data_free() {
  // int num_frames = 1;
  for (int i = 0; i < ext_framecount; ++i) {
    free(ext_partition_tree[i].partition_tree);
  }
  free(ext_partition_tree);
}

static int encode_frame(aom_codec_ctx_t *codec, aom_image_t *img,
                        int frame_index, int flags, AvxVideoWriter *writer) {
  int got_pkts = 0;
  aom_codec_iter_t iter = NULL;
  const aom_codec_cx_pkt_t *pkt = NULL;
  const aom_codec_err_t res =
      aom_codec_encode(codec, img, frame_index, 1, flags);
  if (res != AOM_CODEC_OK) die_codec(codec, "Failed to encode frame");

  while ((pkt = aom_codec_get_cx_data(codec, &iter)) != NULL) {
    got_pkts = 1;

    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
      const int keyframe = (pkt->data.frame.flags & AOM_FRAME_IS_KEY) != 0;
      if (!aom_video_writer_write_frame(writer, pkt->data.frame.buf,
                                        pkt->data.frame.sz,
                                        pkt->data.frame.pts)) {
        die_codec(codec, "Failed to write compressed frame");
      }
      printf(keyframe ? "K" : ".");
      fflush(stdout);
    }
  }

  return got_pkts;
}

aom_ext_part_status_t ext_part_create_model(
    void *priv, const aom_ext_part_config_t *part_config,
    aom_ext_part_model_t *ext_part_model) {
  // printf("create model\n");

  TestData *received_data = (TestData *)(priv);
  // assert(received_data->version == kVersion);
  // EXPECT_EQ(received_data->version, kVersion);
  ToyModel *toy_model = malloc(sizeof(ToyModel));
  // ToyModel *toy_model = new (std::nothrow) ToyModel;
  // if (toy_model == nullptr) {
  //   EXPECT_NE(toy_model, nullptr);
  //   return AOM_EXT_PART_ERROR;
  // }
  toy_model->data = received_data;
  *ext_part_model = toy_model;
  assert(part_config->superblock_size == BLOCK_64X64);
  // printf("part_config->superblock size %d == %d?\n",
  //        part_config->superblock_size, BLOCK_64X64);
  // EXPECT_EQ(part_config->superblock_size, BLOCK_64X64);
  return AOM_EXT_PART_OK;
}

aom_ext_part_status_t ext_part_send_features(
    aom_ext_part_model_t ext_part_model,
    const aom_partition_features_t *part_features) {
  ToyModel *toy_model = (ToyModel *)(ext_part_model);
  toy_model->mi_row = part_features->mi_row;
  toy_model->mi_col = part_features->mi_col;
  toy_model->frame_width = part_features->frame_width;
  toy_model->frame_height = part_features->frame_height;
  toy_model->block_size = (BLOCK_SIZE)(part_features->block_size);
  toy_model->frame_num = part_features->frame_num;
  // printf("send features %d %d %d\n", toy_model->frame_num, toy_model->mi_row, toy_model->mi_col);
  (void)ext_part_model;
  (void)part_features;
  return AOM_EXT_PART_OK;
}

// The model provide the whole decision tree to the encoder.
aom_ext_part_status_t ext_part_get_partition_decision_whole_tree(
    aom_ext_part_model_t ext_part_model,
    aom_partition_decision_t *ext_part_decision) {
  // printf("decision whole tree %d\n", ext_framecount);
  ToyModel *toy_model = (ToyModel *)(ext_part_model);
  // A toy model that always asks the encoder to encode with
  // 4x4 blocks (the smallest).
  ext_part_decision->is_final_decision = 1;
  // Note: super block size is fixed to BLOCK_64X64 for the
  // input video. It is determined inside the encoder, see the
  // check in "ext_part_create_model".
  const int is_last_sb_col =
      toy_model->mi_col * 4 + 64 > toy_model->frame_width;
  const int is_last_sb_row =
      toy_model->mi_row * 4 + 64 > toy_model->frame_height;

  const int sbrow = toy_model->mi_row / 16;
  const int sbcol = toy_model->mi_col / 16;
  const int sb_idx = sbrow * ((toy_model->frame_width + 63) / 64) + sbcol;
  // printf("sbrow: %d, sbcol: %d, sb index: %d\n", sbrow, sbcol, sb_idx);

#if 1
  const int num_partitions = ext_partition_tree[toy_model->frame_num].partition_tree[sb_idx].num_partitions;
    ext_part_decision->num_nodes = num_partitions;

  // printf("decision: num partitionds %d\n", num_partitions);
  for(int i = 0; i < num_partitions; ++i){
      ext_part_decision->partition_decision[i] = ext_partition_tree[toy_model->frame_num].partition_tree[sb_idx].partition_tree[i];
      // printf("partition %d: %d\n", i, ext_part_decision->partition_decision[i]);
  }
#endif

#if 0
    // 64x64: 1 node
    // 32x32: 4 nodes
    // 16x16: 4 * 4 nodes
    // 8x8:   4 * 4 * 4 nodes
    // 4x4:   4 * 4 * 4 * 4 nodes
    const int num_blocks = 1 + 4 + 4 * 4 + 4 * 4 * 4 + 4 * 4 * 4 * 4;
    const int num_4x4_blocks = 4 * 4 * 4 * 4;
    ext_part_decision->num_nodes = num_blocks;

    // 64x64: 1 node
    // 32x32: 4 nodes
    // 16x16: 4 * 4 nodes

    // 8x8:   4 * 4 * 4 nodes
    // 4x4:   4 * 4 * 4 * 4 nodes

    if (is_last_sb_row) {
      // 64x64
      ext_part_decision->num_nodes = 21;
      ext_part_decision->partition_decision[0] = PARTITION_SPLIT;
      // // 32x32, only the top-left and bottom-left will split, the other two
      // are
      // // out of frame boundary.
      ext_part_decision->partition_decision[1] = PARTITION_SPLIT;
      ext_part_decision->partition_decision[2] = PARTITION_SPLIT;
      ext_part_decision->partition_decision[3] = PARTITION_SPLIT;
      ext_part_decision->partition_decision[4] = PARTITION_SPLIT;

      ext_part_decision->partition_decision[5] = PARTITION_NONE;
      ext_part_decision->partition_decision[6] = PARTITION_NONE;
      ext_part_decision->partition_decision[7] = PARTITION_NONE;
      ext_part_decision->partition_decision[8] = PARTITION_NONE;
      ext_part_decision->partition_decision[9] = PARTITION_NONE;
      ext_part_decision->partition_decision[10] = PARTITION_NONE;
      ext_part_decision->partition_decision[11] = PARTITION_NONE;
      ext_part_decision->partition_decision[12] = PARTITION_NONE;

      ext_part_decision->partition_decision[13] = PARTITION_NONE;
      ext_part_decision->partition_decision[14] = PARTITION_NONE;
      ext_part_decision->partition_decision[15] = PARTITION_NONE;
      ext_part_decision->partition_decision[16] = PARTITION_NONE;
      ext_part_decision->partition_decision[17] = PARTITION_NONE;
      ext_part_decision->partition_decision[18] = PARTITION_NONE;
      ext_part_decision->partition_decision[19] = PARTITION_NONE;
      ext_part_decision->partition_decision[20] = PARTITION_NONE;
    } else if(sbcol % 2 == 1){
            // 64x64
      ext_part_decision->num_nodes = 1;
      ext_part_decision->partition_decision[0] = PARTITION_NONE;
      // // 32x32, only the top-left and bottom-left will split, the other two
      // are
      // // out of frame boundary.
      ext_part_decision->partition_decision[1] = PARTITION_HORZ_A;
      ext_part_decision->partition_decision[2] = PARTITION_SPLIT;
      ext_part_decision->partition_decision[3] = PARTITION_SPLIT;
      ext_part_decision->partition_decision[4] = PARTITION_SPLIT;

      ext_part_decision->partition_decision[5] = PARTITION_NONE;
      ext_part_decision->partition_decision[6] = PARTITION_NONE;
      ext_part_decision->partition_decision[7] = PARTITION_NONE;
      ext_part_decision->partition_decision[8] = PARTITION_NONE;
      ext_part_decision->partition_decision[9] = PARTITION_NONE;
      ext_part_decision->partition_decision[10] = PARTITION_NONE;
      ext_part_decision->partition_decision[11] = PARTITION_NONE;
      ext_part_decision->partition_decision[12] = PARTITION_NONE;

      ext_part_decision->partition_decision[13] = PARTITION_NONE;
      ext_part_decision->partition_decision[14] = PARTITION_NONE;
      ext_part_decision->partition_decision[15] = PARTITION_NONE;
      ext_part_decision->partition_decision[16] = PARTITION_NONE;
      ext_part_decision->partition_decision[17] = PARTITION_NONE;
      ext_part_decision->partition_decision[18] = PARTITION_NONE;
      ext_part_decision->partition_decision[19] = PARTITION_NONE;
    } else{
      ext_part_decision->num_nodes = 5;
      ext_part_decision->partition_decision[0] = PARTITION_SPLIT;
      // // 32x32, only the top-left and bottom-left will split, the other two
      // are
      // // out of frame boundary.
      ext_part_decision->partition_decision[1] = PARTITION_NONE;
      ext_part_decision->partition_decision[2] = PARTITION_NONE;
      ext_part_decision->partition_decision[3] = PARTITION_NONE;
      ext_part_decision->partition_decision[4] = PARTITION_NONE;
    }

#endif


#if 0
  if (is_last_sb_row && is_last_sb_col) {
    // printf("callack is_last_sb_row && is_last_sb_col\n");

    // 64x64: 1 node
    // 32x32: 4 nodes (only the first one will further split)
    // 16x16: 4 nodes
    // 8x8:   4 * 4 nodes
    // 4x4:   4 * 4 * 4 nodes
    const int num_blocks = 1 + 4 + 4 + 4 * 4 + 4 * 4 * 4;
    const int num_4x4_blocks = 4 * 4 * 4;
    ext_part_decision->num_nodes = num_blocks;
    // 64x64
    ext_part_decision->partition_decision[0] = PARTITION_SPLIT;
    // 32x32, only the first one will split, the other three are
    // out of frame boundary.
    ext_part_decision->partition_decision[1] = PARTITION_SPLIT;
    ext_part_decision->partition_decision[2] = PARTITION_NONE;
    ext_part_decision->partition_decision[3] = PARTITION_NONE;
    ext_part_decision->partition_decision[4] = PARTITION_NONE;
    // The rest blocks inside the top-left 32x32 block.
    for (int i = 5; i < num_blocks - num_4x4_blocks; ++i) {
      ext_part_decision->partition_decision[i] = PARTITION_SPLIT;
    }
    for (int i = num_blocks - num_4x4_blocks; i < num_blocks; ++i) {
      ext_part_decision->partition_decision[i] = PARTITION_NONE;
    }
  } else if (is_last_sb_row) {
    // printf("callack is_last_sb_row\n");

    // 64x64: 1 node
    // 32x32: 4 nodes (only the first two will further split)
    // 16x16: 2 * 4 nodes
    // 8x8:   2 * 4 * 4 nodes
    // 4x4:   2 * 4 * 4 * 4 nodes
    const int num_blocks = 1 + 4 + 2 * 4 + 2 * 4 * 4 + 2 * 4 * 4 * 4;
    const int num_4x4_blocks = 2 * 4 * 4 * 4;
    ext_part_decision->num_nodes = num_blocks;
    // 64x64
    ext_part_decision->partition_decision[0] = PARTITION_SPLIT;
    // 32x32, only the first two will split, the other two are out
    // of frame boundary.
    ext_part_decision->partition_decision[1] = PARTITION_SPLIT;
    ext_part_decision->partition_decision[2] = PARTITION_SPLIT;
    ext_part_decision->partition_decision[3] = PARTITION_NONE;
    ext_part_decision->partition_decision[4] = PARTITION_NONE;
    // The rest blocks.
    for (int i = 5; i < num_blocks - num_4x4_blocks; ++i) {
      ext_part_decision->partition_decision[i] = PARTITION_SPLIT;
    }
    for (int i = num_blocks - num_4x4_blocks; i < num_blocks; ++i) {
      ext_part_decision->partition_decision[i] = PARTITION_NONE;
    }
  } else if (is_last_sb_col) {
    // printf("callack is_last_sb_col\n");

    // 64x64: 1 node
    // 32x32: 4 nodes (only the top-left and bottom-left will further split)
    // 16x16: 2 * 4 nodes
    // 8x8:   2 * 4 * 4 nodes
    // 4x4:   2 * 4 * 4 * 4 nodes
    const int num_blocks = 1 + 4 + 2 * 4 + 2 * 4 * 4 + 2 * 4 * 4 * 4;
    const int num_4x4_blocks = 2 * 4 * 4 * 4;
    ext_part_decision->num_nodes = num_blocks;
    // 64x64
    ext_part_decision->partition_decision[0] = PARTITION_SPLIT;
    // 32x32, only the top-left and bottom-left will split, the other two are
    // out of frame boundary.
    ext_part_decision->partition_decision[1] = PARTITION_SPLIT;
    ext_part_decision->partition_decision[2] = PARTITION_NONE;
    ext_part_decision->partition_decision[3] = PARTITION_SPLIT;
    ext_part_decision->partition_decision[4] = PARTITION_NONE;
    // The rest blocks.
    for (int i = 5; i < num_blocks - num_4x4_blocks; ++i) {
      ext_part_decision->partition_decision[i] = PARTITION_SPLIT;
    }
    for (int i = num_blocks - num_4x4_blocks; i < num_blocks; ++i) {
      ext_part_decision->partition_decision[i] = PARTITION_NONE;
    }
  } else {
    // printf("callack else\n");

    // 64x64: 1 node
    // 32x32: 4 nodes
    // 16x16: 4 * 4 nodes
    // 8x8:   4 * 4 * 4 nodes
    // 4x4:   4 * 4 * 4 * 4 nodes
    const int num_blocks = 1 + 4 + 4 * 4 + 4 * 4 * 4 + 4 * 4 * 4 * 4;
    const int num_4x4_blocks = 4 * 4 * 4 * 4;
    ext_part_decision->num_nodes = num_blocks;
    for (int i = 0; i < num_blocks - num_4x4_blocks; ++i) {
      ext_part_decision->partition_decision[i] = PARTITION_SPLIT;
    }
    for (int i = num_blocks - num_4x4_blocks; i < num_blocks; ++i) {
      ext_part_decision->partition_decision[i] = PARTITION_NONE;
    }

    // 64x64: 1 node
    // 32x32: 4 nodes
    // 16x16: 4 * 4 nodes

    // 8x8:   4 * 4 * 4 nodes
    // 4x4:   4 * 4 * 4 * 4 nodes

    // 64x64
    ext_part_decision->num_nodes = 18;
    ext_part_decision->partition_decision[0] = PARTITION_SPLIT;
    // // 32x32, only the top-left and bottom-left will split, the other two are
    // // out of frame boundary.
    ext_part_decision->partition_decision[1] = PARTITION_HORZ_4;
    ext_part_decision->partition_decision[2] = PARTITION_VERT_B;
    ext_part_decision->partition_decision[3] = PARTITION_HORZ_B;
    ext_part_decision->partition_decision[4] = PARTITION_VERT_A;

    ext_part_decision->partition_decision[5] = PARTITION_NONE;
    ext_part_decision->partition_decision[6] = PARTITION_NONE;
    ext_part_decision->partition_decision[7] = PARTITION_NONE;
    ext_part_decision->partition_decision[8] = PARTITION_NONE;
    ext_part_decision->partition_decision[9] = PARTITION_NONE;
    ext_part_decision->partition_decision[10] = PARTITION_NONE;
    ext_part_decision->partition_decision[11] = PARTITION_NONE;
    ext_part_decision->partition_decision[12] = PARTITION_NONE;

    ext_part_decision->partition_decision[13] = PARTITION_NONE;
    ext_part_decision->partition_decision[14] = PARTITION_NONE;
    ext_part_decision->partition_decision[15] = PARTITION_NONE;
    ext_part_decision->partition_decision[16] = PARTITION_NONE;
    ext_part_decision->partition_decision[17] = PARTITION_NONE;
    // ext_part_decision->partition_decision[18] = PARTITION_NONE;
    // ext_part_decision->partition_decision[19] = PARTITION_NONE;
    // ext_part_decision->partition_decision[20] = PARTITION_NONE;
  }
  // printf("encoder : num_nodes = %d\n", ext_part_decision->num_nodes);
  // printf("callback partition:\n");
#endif

  return AOM_EXT_PART_OK;
}

aom_ext_part_status_t ext_part_send_partition_stats(
    aom_ext_part_model_t ext_part_model,
    const aom_partition_stats_t *ext_part_stats) {
  // printf("send partition stats\n");
  (void)ext_part_model;
  (void)ext_part_stats;
  return AOM_EXT_PART_OK;
}

aom_ext_part_status_t ext_part_delete_model(
    aom_ext_part_model_t ext_part_model) {
  // printf("delete model\n");
  ToyModel *toy_model = (ToyModel *)(ext_part_model);
  // EXPECT_EQ(toy_model->data->version, kVersion);
  free(toy_model);
  return AOM_EXT_PART_OK;
}

// TODO(tomfinegan): Improve command line parsing and add args for bitrate/fps.
int main(int argc, char **argv) {
  FILE *infile = NULL;
  aom_codec_ctx_t codec;
  aom_codec_enc_cfg_t cfg;
  int frame_count = 0;
  aom_image_t raw;
  aom_codec_err_t res;
  AvxVideoInfo info;
  AvxVideoWriter *writer = NULL;
  const int fps = 30;
  int bitrate = 200;
  int keyframe_interval = 30;
  int num_frames = 0;
  int frames_encoded = 0;
  const char *codec_arg = "av1";
  // const char *width_arg = NULL;
  // const char *height_arg = NULL;
  char *input_file = NULL;
  char *partition_file = NULL;
  char *output_file = NULL;
  int usage = 0;
  int speed = 5;
  int bitdepth = 8;
  int rc = 1;
  aom_codec_flags_t flags = 0;

  exec_name = argv[0];

  // Clear explicitly, as simply assigning "{ 0 }" generates
  // "missing-field-initializers" warning in some compilers.
  memset(&info, 0, sizeof(info));

  int width = 0;
  int height = 0;
  int use_ext_partition = 0;
  int quantizer = -1;

  int c;

  // Define the long options
  static struct option long_options[] = {
      {"width", required_argument, 0, 'w'},
      {"height", required_argument, 0, 'h'},
      {"num_frames", required_argument, 0, 'n'},
      {"input", required_argument, 0, 'i'},
      {"partitionfile", required_argument, 0, 'p'},
      {"output", required_argument, 0, 'o'},
      {"cpu_level", required_argument, 0, 'c'},
      {"kf_interval", required_argument, 0, 'k'},
      {"use_ext_partition", optional_argument, 0, 'e'},
      {"bitrate", optional_argument, 0, 'b'},
      {"quantizer", optional_argument, 0, 'q'},
      {"bitdepth", optional_argument, 0, 'd'},
      {"rc", optional_argument, 0, 'r'},
      {0, 0, 0, 0}  // Sentinel to indicate the end of options
  };

  int option_index = 0;

  // Loop through all command-line arguments
  while ((c = getopt_long(argc, argv, "w:h:n:i:p:o:c:k:e:b:q:d:r:", long_options,
                          &option_index)) != -1) {
    switch (c) {
      case 'w':
        width = atoi(optarg);
        break;
      case 'h':
        height = atoi(optarg);
        break;
      case 'n':
        num_frames = atoi(optarg);
        break;
      case 'i':
        input_file = optarg;  // Duplicate the string to own it
        break;
      case 'p':
        partition_file = optarg;
        break;
      case 'o':
        output_file = optarg;
        break;
      case 'c':
        speed = atoi(optarg);
        break;
      case 'k':
        keyframe_interval = atoi(optarg);
        break;
      case 'e':
        use_ext_partition = atoi(optarg);
        break;
       case 'b':
        bitrate = atoi(optarg);
        break;
      case 'q':
        quantizer = atoi(optarg);
        break;
      case 'd':
        bitdepth = atoi(optarg);
        break;
      case 'r':
        rc = atoi(optarg);
        break;
      case '?':
        // `getopt_long` already printed an error message
        break;
      default:
        fprintf(stderr, "?? getopt_long returned character code 0%o ??\n", c);
    }
  }

  // Print the parsed arguments
#if 1
  printf("Parsed Arguments:\n");
  printf("  Width: %d\n", width);
  printf("  Height: %d\n", height);
  printf("  Num Frames: %d\n", num_frames);
  printf("  Input File: %s\n", input_file ? input_file : "Not specified");
  printf("  Partition File: %s\n",
         partition_file ? partition_file : "Not specified");
  printf("  Output File: %s\n", output_file ? output_file : "Not specified");
  printf("  CPU Level: %d\n", speed);
  printf("  Keyframe interval: %d\n", keyframe_interval);
  printf("  Bitdepth: %d\n", bitdepth);
#endif

#if 1
  // Low Delay config
/**
--cpu-used=0 --passes=1 --lag-in-frames=0 --min-gf-interval=16
--max-gf-interval=16
--gf-min-pyr-height=4 --gf-max-pyr-height=4 --limit=130 --kf-min-dist=9999
--kf-max-dist=9999
--use-fixed-qp-offsets=1 --deltaq-mode=0 --enable-tpl-model=0 --end-usage=q
--qp=x
--subgop-config-str=ld --enable-keyframe-filtering=0 --obu
**/

  cfg.g_lag_in_frames = 0;
  cfg.use_fixed_qp_offsets = 1;
  cfg.rc_min_quantizer = 0;
  cfg.rc_max_quantizer = 255;
  cfg.rc_end_usage = AOM_Q;
    // Set keyframe distances
  cfg.kf_min_dist = 9999;
  cfg.kf_max_dist = 9999;
  cfg.g_pass = AOM_RC_ONE_PASS;
#endif

  aom_codec_iface_t *encoder = get_aom_encoder_by_short_name(codec_arg);
  if (!encoder) die("Unsupported codec.");

  info.codec_fourcc = get_fourcc_by_aom_encoder(encoder);
  // info.frame_width = (int)strtol(width_arg, NULL, 0);
  // info.frame_height = (int)strtol(height_arg, NULL, 0);
  info.frame_width = width;
  info.frame_height = height;
  info.time_base.numerator = 1;
  info.time_base.denominator = fps;

  if (info.frame_width <= 0 || info.frame_height <= 0 ||
      (info.frame_width % 2) != 0 || (info.frame_height % 2) != 0) {
    die("Invalid frame size: %dx%d", info.frame_width, info.frame_height);
  }


  // keyframe_interval = (int)strtol(keyframe_interval_arg, NULL, 0);
  if (keyframe_interval < 0) die("Invalid keyframe interval value.");

  printf("Using %s\n", aom_codec_iface_name(encoder));

  res = aom_codec_enc_config_default(encoder, &cfg, usage);
  if (res) die_codec(&codec, "Failed to get default codec config.");

  cfg.g_w = info.frame_width;
  cfg.g_h = info.frame_height;
  cfg.g_timebase.num = info.time_base.numerator;
  cfg.g_timebase.den = info.time_base.denominator;
  // cfg.g_usage = AOM_USAGE_REALTIME;
  if(rc == 1){
    cfg.rc_target_bitrate = bitrate;
  } else {
    cfg.rc_min_quantizer = quantizer;
    cfg.rc_max_quantizer = quantizer;
    cfg.rc_end_usage = AOM_Q;
  }
  cfg.g_error_resilient = 0;
  cfg.g_input_bit_depth = bitdepth;
  cfg.g_bit_depth = bitdepth;
  if(bitdepth > 8){
    flags |= AOM_CODEC_USE_HIGHBITDEPTH;
  }
  cfg.g_profile = 0;
  // if(use_ext_partition){
    cfg.encoder_cfg.super_block_size = 64;
  // }

  int fmt = (bitdepth > 8) ? AOM_IMG_FMT_I42016 : AOM_IMG_FMT_I420;

  // cfg.g_error_resilient = (aom_codec_er_flags_t)strtoul(argv[7], NULL, 0);

  writer = aom_video_writer_open(output_file, kContainerIVF, &info);
  if (!writer) die("Failed to open %s for writing.", output_file);

  if (!(infile = fopen(input_file, "rb")))
    die("Failed to open %s for reading.", input_file);

  if (aom_codec_enc_init(&codec, encoder, &cfg, flags))
    die("Failed to initialize encoder");

  if (aom_codec_control(&codec, AOME_SET_CPUUSED, speed))
    die_codec(&codec, "Failed to set cpu-used");
  if (aom_codec_control(&codec, AV1E_SET_MIN_GF_INTERVAL, 16))
    die_codec(&codec, "Failed to set min_gf_interval");
  if (aom_codec_control(&codec, AV1E_SET_MAX_GF_INTERVAL, 16))
    die_codec(&codec, "Failed to set max_gf_interval");
  if (aom_codec_control(&codec, AV1E_SET_DELTAQ_MODE, 0))
    die_codec(&codec, "Failed to set deltaq_mode");
  if (aom_codec_control(&codec, AV1E_SET_ENABLE_TPL_MODEL, 0))
    die_codec(&codec, "Failed to set enable-tpl-model");
  if (aom_codec_control(&codec, AV1E_SET_MAX_REFERENCE_FRAMES, 4))
    die_codec(&codec, "Failed to set max-reference-frames");

  if (aom_codec_control(&codec, AV1E_SET_GF_MAX_PYRAMID_HEIGHT, 4))
    die_codec(&codec, "Failed to set max-pyr-height");
  if (aom_codec_control(&codec, AV1E_SET_GF_MIN_PYRAMID_HEIGHT, 4))
    die_codec(&codec, "Failed to set min-pyr-height");
  if (aom_codec_control(&codec, AV1E_SET_ENABLE_KEYFRAME_FILTERING, 0))
    die_codec(&codec, "Failed to set enable-keyframe-filtering");

  
  if (aom_codec_enc_config_set(&codec, &cfg))
    die_codec(&codec, "Failed to set enc cfg for layer 0");

  // if (use_ext_partition) {
    if (aom_codec_control(&codec, AV1E_SET_SUPERBLOCK_SIZE,
                          AOM_SUPERBLOCK_SIZE_64X64)) {
      die_codec(&codec, "Failed to set cpu-used");
    }
  // }

  if (!aom_img_alloc(&raw, fmt, info.frame_width,
                     info.frame_height, 1)) {
    die("Failed to allocate image.");
  }
  


  // Force Partition size w/ min/max
  // aom_codec_control(&codec, AV1E_SET_MAX_PARTITION_SIZE, 64);
  // aom_codec_control(&codec, AV1E_SET_MIN_PARTITION_SIZE, 64);

  /*
  // SetExternalPartition(false);
  // SetPartitionControlMode(2);
  // SetDecisionMode(AOM_EXT_PART_WHOLE_TREE);
  */

  if (use_ext_partition) {
    // Set Up External Partition.
    aom_ext_part_funcs_t ext_part_funcs;
    ext_part_funcs.decision_mode = AOM_EXT_PART_WHOLE_TREE;

    // TODO: Store motion tree data here
    //  ext_part_funcs.priv = reinterpret_cast<void *>(&motion_tree_data);

    // External partition Callbacks
    ext_part_funcs.create_model = ext_part_create_model;
    ext_part_funcs.send_features = ext_part_send_features;
    ext_part_funcs.get_partition_decision =
        ext_part_get_partition_decision_whole_tree;
    ext_part_funcs.send_partition_stats = ext_part_send_partition_stats;
    ext_part_funcs.delete_model = ext_part_delete_model;

    aom_codec_control(&codec, AV1E_SET_EXTERNAL_PARTITION, &ext_part_funcs);

    // Read partition data files.
    char line[100] = {};
    FILE *partition_data_ptr = fopen(partition_file, "r");
    if (partition_data_ptr == NULL) {
      printf("Can't read partition data file!\n");
      return 1;
    }

    fgets(line, 100, partition_data_ptr);
    int partition_data_num_frames = atoi(line);
    ext_framecount = partition_data_num_frames;

    ext_partition_tree =
        malloc(partition_data_num_frames * sizeof(FramePartitions));

    for (int frame_num = 0; frame_num < partition_data_num_frames;
         ++frame_num) {
      fgets(line, 100, partition_data_ptr);
      int num_sbs = atoi(line);
      // printf("number of SBs: %d\n", num_sbs);
      ext_partition_tree[frame_num].partition_tree =
          malloc(num_sbs * sizeof(SuperBlockPartitions));
      for (int i = 0; i < num_sbs; ++i) {
        fgets(line, 100, partition_data_ptr);
        ext_partition_tree[frame_num].partition_tree[i].sbx = atoi(line);
        fgets(line, 100, partition_data_ptr);
        ext_partition_tree[frame_num].partition_tree[i].sby = atoi(line);

        int partition_val = 0;
        int partition_idx = 0;
        while (partition_val != -1) {
          fgets(line, 100, partition_data_ptr);
          partition_val = atoi(line);
          if (partition_val != -1) {
            // printf("idx %d,  %d\n", partition_idx, partition_val);

            ext_partition_tree[frame_num]
                .partition_tree[i]
                .partition_tree[partition_idx] = partition_val;
            partition_idx++;
          } else {
            // printf("end of sb %d\n", partition_val);
          }
        }
        ext_partition_tree[frame_num].partition_tree[i].num_partitions =
            partition_idx;
        // printf("partition idx %d\n", partition_idx);
      }
      fgets(line, 100, partition_data_ptr);
      int eof = atoi(line);
      (void)eof;
      assert(eof == -2);
    }

    // printf("done with reading sb's\n");
    // while (fgets(line, 100, partition_data_ptr)) {
    //   printf("%s", line);
    // }
    fclose(partition_data_ptr);  // release memory
  }

  // Encode frames.
  while (aom_img_read(&raw, infile)) {
    // Encode a frame
    int flags = 0;
    if (keyframe_interval > 0 && frame_count % keyframe_interval == 0)
      flags |= AOM_EFLAG_FORCE_KF;
    if(keyframe_interval == 0){
      flags |= AOM_EFLAG_FORCE_KF;
    }
    encode_frame(&codec, &raw, frame_count++, flags, writer);
    frames_encoded++;
    if (num_frames > 0 && frames_encoded >= num_frames) break;
  }

  // Flush encoder.
  while (encode_frame(&codec, NULL, -1, 0, writer)) continue;

  partition_data_free();

  fclose(infile);

  aom_img_free(&raw);
  if (aom_codec_destroy(&codec)) die_codec(&codec, "Failed to destroy codec.");

  aom_video_writer_close(writer);

  return EXIT_SUCCESS;
}
