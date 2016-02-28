/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * arch_kernel_api.h
 *
 *  Created on: 2014/12/07
 *      Author: biwa
 */

#ifndef ARCH_KERNEL_API_H_
#define ARCH_KERNEL_API_H_

#include "common.h"

extern bool arch_can_dispatch(void);
extern TimeSpec get_tick_count(void);
extern void update_first_timeout(TimeSpec tmout);

#endif /* ARCH_COMMON_H_ */
