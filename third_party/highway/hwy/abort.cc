// Copyright 2019 Google LLC
// Copyright 2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
// SPDX-License-Identifier: Apache-2.0
// SPDX-License-Identifier: BSD-3-Clause


#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <atomic>
#include <string>

#include "third_party/highway/hwy/base.h"

#if HWY_IS_ASAN || HWY_IS_MSAN || HWY_IS_TSAN
#include "sanitizer/common_interface_defs.h"  // __sanitizer_print_stack_trace
#endif

namespace hwy {



}  // namespace hwy
