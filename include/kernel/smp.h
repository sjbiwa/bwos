/*
 * smp.h
 *
 *  Created on: 2015/05/26
 *      Author: biwa
 */
#ifndef _SMP_H_
#define _SMP_H_

#if USE_SMP==1
#include "arch_smp.h"

void spin_init(SpinLockObj* ptr);
void spin_lock(SpinLockObj* ptr);
void spin_unlock(SpinLockObj* ptr);
int spin_trylock(SpinLockObj* ptr);
#else
typedef	int		SpinLockObj;

#define	spin_init(a)
#define	spin_lock(a)
#define	spin_unlock(a)
#define	spin_trylock(a)		(1)
#endif

#endif
