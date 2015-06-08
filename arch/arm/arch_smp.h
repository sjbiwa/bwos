/*
 * arch_smp.h
 *
 *  Created on: 2015/05/28
 *      Author: biwa
 */

#ifndef _ARCH_SMP_H_
#define _ARCH_SMP_H_

#include <stdint.h>
#include "arm.h"
#include "cp15reg.h"
#include "board_smp.h"

typedef	uint32_t	SpinLockObj;

void spin_init(SpinLockObj* ptr);
void spin_lock(SpinLockObj* ptr);
void spin_unlock(SpinLockObj* ptr);
int spin_trylock(SpinLockObj* ptr);

#endif /* _ARCH_SMP_H_ */
