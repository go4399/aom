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

#ifndef AOM_AOM_PORTS_AVM_TIMER_H_
#define AOM_AOM_PORTS_AVM_TIMER_H_

#include "aom_ports/aom_timer.h"

#define av2_usec_timer aom_usec_timer
#define av2_usec_timer_start aom_usec_timer_start
#define av2_usec_timer_mark aom_usec_timer_mark
#define av2_usec_timer_elapsed aom_usec_timer_elapsed

#endif  // AOM_AOM_PORTS_AVM_TIMER_H_
