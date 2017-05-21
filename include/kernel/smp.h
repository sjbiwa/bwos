/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

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

#ifdef __cplusplus
extern "C" {
#endif

extern void spin_init(SpinLockObj* lock);
extern void spin_lock(SpinLockObj* lock);
extern void spin_unlock(SpinLockObj* lock);
extern int spin_trylock(SpinLockObj* lock);

#ifdef __cplusplus
}
#endif

#else
typedef	int		SpinLockObj;

#define	spin_init(a)
#define	spin_lock(a)
#define	spin_unlock(a)
#define	spin_trylock(a)		(1)

#endif

#endif
