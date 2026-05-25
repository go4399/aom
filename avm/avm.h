#ifndef AVM_AVM_AVM_H_
#define AVM_AVM_AVM_H_

#include "aom/aom.h"
#include "avm/avm_codec.h"
#include "avm/avm_image.h"

#ifdef __cplusplus
extern "C" {
#endif

enum avm_com_control_id {
  AV2_GET_REFERENCE = 128,
  AV2_SET_REFERENCE = 129,
  AV2_COPY_REFERENCE = 130,
  AVM_COMMON_CTRL_ID_MAX,
  AV2_GET_NEW_FRAME_IMAGE = 192,
  AV2_COPY_NEW_FRAME_IMAGE = 193,
  AVM_DECODER_CTRL_ID_START = 256
};

typedef struct av2_ref_frame {
  int idx;
  int use_external_ref;
  avm_image_t img;
} av2_ref_frame_t;

#define AVM_CTRL_USE_TYPE AOM_CTRL_USE_TYPE

AVM_CTRL_USE_TYPE(AV2_GET_REFERENCE, av2_ref_frame_t *)
#define AVM_CTRL_AV2_GET_REFERENCE

AVM_CTRL_USE_TYPE(AV2_SET_REFERENCE, av2_ref_frame_t *)
#define AVM_CTRL_AV2_SET_REFERENCE

AVM_CTRL_USE_TYPE(AV2_COPY_REFERENCE, av2_ref_frame_t *)
#define AVM_CTRL_AV2_COPY_REFERENCE

AVM_CTRL_USE_TYPE(AV2_GET_NEW_FRAME_IMAGE, avm_image_t *)
#define AVM_CTRL_AV2_GET_NEW_FRAME_IMAGE

AVM_CTRL_USE_TYPE(AV2_COPY_NEW_FRAME_IMAGE, avm_image_t *)
#define AVM_CTRL_AV2_COPY_NEW_FRAME_IMAGE

#ifdef __cplusplus
}
#endif

#endif  // AVM_AVM_AVM_H_
