/*
 * Copyright (C) 2016 sjbiwa All rights reserved.
 */

/*
 * lock.h
 *
 *  Created on: 2016/01/23
 *      Author: biwa
 */

#ifndef _LOCK_H_
#define _LOCK_H_

#include "bwos.h"
#include "lockobject.h"

/* CpuSturct用spinlock関数(全域で使用するのでここで定義する) */
OBJECT_SPINLOCK_FUNC(cpu,CpuStruct);

#if USE_SMP == 1
static inline void cpu_spinlock_by_task(TaskStruct* task)
{
	for (;;) {
		CpuStruct* cpu = task->cpu_struct;
		cpu_spinlock(cpu);
		if ( cpu == task->cpu_struct ) {
			break;
		}
		cpu_spinunlock(cpu);
	}
}

static inline int cpu_spintrylock_by_task(TaskStruct* task)
{
	int ret = 0;
	CpuStruct* cpu = task->cpu_struct;
	if ( cpu_spintrylock(cpu) ) {
		if ( cpu == task->cpu_struct ) {
			ret = 1;
		}
		else {
			cpu_spinunlock(cpu);
		}
	}
	return ret;
}
#else
static inline void cpu_spinlock_by_task(TaskStruct* task)
{
}
static inline int cpu_spintrylock_by_task(TaskStruct* task)
{
	return 1;
}
#endif

#endif /* INCLUDE_LOCK_H_ */
