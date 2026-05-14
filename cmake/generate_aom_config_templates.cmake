#
# Copyright (c) 2017, Alliance for Open Media. All rights reserved.
#
# This source code is subject to the terms of the BSD 2 Clause License and the
# Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License was
# not distributed with this source code in the LICENSE file, you can obtain it
# at www.aomedia.org/license/software. If the Alliance for Open Media Patent
# License 1.0 was not distributed with this source code in the PATENTS file, you
# can obtain it at www.aomedia.org/license/patent.
#
cmake_minimum_required(VERSION 3.16)

set(year "2017")
set(asm_file_header_block "\;
\; Copyright (c) ${year}, Alliance for Open Media. All rights reserved.
\;
\; This source code is subject to the terms of the BSD 2 Clause License and
\; the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
\; was not distributed with this source code in the LICENSE file, you can
\; obtain it at www.aomedia.org/license/software. If the Alliance for Open
\; Media Patent License 1.0 was not distributed with this source code in the
\; PATENTS file, you can obtain it at www.aomedia.org/license/patent.
\;
")
set(h_file_header_block "/*
 * Copyright (c) ${year}, Alliance for Open Media. All rights reserved.
 *
 * This source code is subject to the terms of the BSD 2 Clause License and
 * the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
 * was not distributed with this source code in the LICENSE file, you can
 * obtain it at www.aomedia.org/license/software. If the Alliance for Open
 * Media Patent License 1.0 was not distributed with this source code in the
 * PATENTS file, you can obtain it at www.aomedia.org/license/patent.
 */
\#ifndef AOM_CONFIG_H_
\#define AOM_CONFIG_H_
")
set(cmake_file_header_block "##
## Copyright (c) ${year}, Alliance for Open Media. All rights reserved.
##
## This source code is subject to the terms of the BSD 2 Clause License and
## the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
## was not distributed with this source code in the LICENSE file, you can
## obtain it at www.aomedia.org/license/software. If the Alliance for Open
## Media Patent License 1.0 was not distributed with this source code in the
## PATENTS file, you can obtain it at www.aomedia.org/license/patent.
##
")

# Terminates cmake execution when $var_name is an empty string, or the variable
# name it contains does not expand to an existing directory.
function(check_directory_var var_name)
  if("${var_name}" STREQUAL "")
    message(FATAL_ERROR "The CMake variable ${var_name} must be defined.")
  endif()

  if(NOT EXISTS "${${var_name}}")
    message(FATAL_ERROR "${${var_name}} (${var_name}) missing.")
  endif()
endfunction()

check_directory_var(AOM_CONFIG_DIR)
check_directory_var(AOM_ROOT)

set(AOM_DEFAULTS "${AOM_ROOT}/cmake/aom_config_defaults.cmake")
if(NOT EXISTS "${AOM_DEFAULTS}")
  message(
    FATAL_ERROR "Configuration default values file (${AOM_DEFAULTS}) missing.")
endif()

include("${AOM_ROOT}/cmake/aom_config_defaults.cmake")
list(APPEND aom_build_vars ${AOM_DETECT_VARS} ${AOM_CONFIG_VARS})
list(SORT aom_build_vars)

set(aom_config_h_template "${AOM_CONFIG_DIR}/config/aom_config.h.cmake")
file(WRITE "${aom_config_h_template}" ${h_file_header_block})
foreach(aom_var ${aom_build_vars})
  if(NOT "${aom_var}" STREQUAL "AOM_RTCD_FLAGS")
    file(APPEND "${aom_config_h_template}"
         "\#define ${aom_var} \${${aom_var}}\n")
  endif()
endforeach()
file(APPEND "${aom_config_h_template}"
     "
#if defined(__has_feature)
#define AOM_HAS_FEATURE(x) __has_feature(x)
#else
#define AOM_HAS_FEATURE(x) 0
#endif

#if AOM_HAS_FEATURE(bounds_safety)
#include <stdint.h>
#include <string.h>
#include <ptrcheck.h>

#define AOM_ASSUME_UNSAFE_INDEXABLE_ABI \\
  __ptrcheck_abi_assume_unsafe_indexable()

#define AOM_COUNTED_BY(x) __counted_by(x)
#define AOM_COUNTED_BY_OR_NULL(x) __counted_by_or_null(x)
#define AOM_SIZED_BY(x) __sized_by(x)
#define AOM_SIZED_BY_OR_NULL(x) __sized_by_or_null(x)
#define AOM_ENDED_BY(x) __ended_by(x)

#define AOM_UNSAFE_INDEXABLE __unsafe_indexable
#define AOM_SINGLE __single
#define AOM_INDEXABLE __indexable
#define AOM_BIDI_INDEXABLE __bidi_indexable

#define AOM_UNSAFE_FORGE_SINGLE(typ, ptr) __unsafe_forge_single(typ, ptr)

#define AOM_UNSAFE_FORGE_BIDI_INDEXABLE(typ, ptr, size) \\
  __unsafe_forge_bidi_indexable(typ, ptr, size)

// Provide memcpy/memset/memmove wrappers to make migration easier.
#define AOM_UNSAFE_MEMCPY(dst, src, size)                               \\
  do {                                                                   \\
    memcpy(AOM_UNSAFE_FORGE_BIDI_INDEXABLE(uint8_t*, dst, size),        \\
           AOM_UNSAFE_FORGE_BIDI_INDEXABLE(uint8_t*, src, size), size); \\
  } while (0)

#define AOM_UNSAFE_MEMSET(dst, c, size)                                    \\
  do {                                                                      \\
    memset(AOM_UNSAFE_FORGE_BIDI_INDEXABLE(uint8_t*, dst, size), c, size); \\
  } while (0)

#define AOM_UNSAFE_MEMMOVE(dst, src, size)                               \\
  do {                                                                    \\
    memmove(AOM_UNSAFE_FORGE_BIDI_INDEXABLE(uint8_t*, dst, size),        \\
            AOM_UNSAFE_FORGE_BIDI_INDEXABLE(uint8_t*, src, size), size); \\
  } while (0)

#define AOM_UNSAFE_MEMCMP(s1, s2, size)                       \\
  memcmp(AOM_UNSAFE_FORGE_BIDI_INDEXABLE(uint8_t*, s1, size), \\
         AOM_UNSAFE_FORGE_BIDI_INDEXABLE(uint8_t*, s2, size), size)

#else  // !AOM_HAS_FEATURE(bounds_safety)

#define AOM_ASSUME_UNSAFE_INDEXABLE_ABI

#define AOM_COUNTED_BY(x)
#define AOM_COUNTED_BY_OR_NULL(x)
#define AOM_SIZED_BY(x)
#define AOM_SIZED_BY_OR_NULL(x)
#define AOM_ENDED_BY(x)

#define AOM_UNSAFE_INDEXABLE
#define AOM_SINGLE
#define AOM_INDEXABLE
#define AOM_BIDI_INDEXABLE

#define AOM_UNSAFE_MEMCPY(dst, src, size) memcpy(dst, src, size)
#define AOM_UNSAFE_MEMSET(dst, c, size) memset(dst, c, size)
#define AOM_UNSAFE_MEMMOVE(dst, src, size) memmove(dst, src, size)
#define AOM_UNSAFE_MEMCMP(s1, s2, size) memcmp(s1, s2, size)

#define AOM_UNSAFE_FORGE_SINGLE(typ, ptr) ((typ)(ptr))
#define AOM_UNSAFE_FORGE_BIDI_INDEXABLE(typ, ptr, size) ((typ)(ptr))

#endif  // AOM_HAS_FEATURE(bounds_safety)
"
)
file(APPEND "${aom_config_h_template}" "\#endif  // AOM_CONFIG_H_")

set(aom_asm_config_template "${AOM_CONFIG_DIR}/config/aom_config.asm.cmake")
file(WRITE "${aom_asm_config_template}" ${asm_file_header_block})
foreach(aom_var ${aom_build_vars})
  if(NOT "${aom_var}" STREQUAL "AOM_RTCD_FLAGS")
    file(APPEND "${aom_asm_config_template}" "${aom_var} equ \${${aom_var}}\n")
  endif()
endforeach()
