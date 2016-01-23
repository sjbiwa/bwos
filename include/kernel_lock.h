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

#include "kernel.h"

#if USE_SMP==1
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

/* オブジェクト用spinlock関数 */
#if USE_SMP==1

#define	OBJECT_SPINLOCK_FUNC(OBJNAME,OBJSTRUCT) \
static inline void OBJNAME##_spininit(OBJSTRUCT* obj) \
{ \
	spin_init(&(obj->spin_lock)); \
} \
static inline void OBJNAME##_spinlock(OBJSTRUCT* obj) \
{ \
	spin_lock(&(obj->spin_lock)); \
} \
static inline void OBJNAME##_spinunlock(OBJSTRUCT* obj) \
{ \
	spin_unlock(&(obj->spin_lock)); \
} \
static inline int OBJNAME##_spintrylock(OBJSTRUCT* obj) \
{ \
	return spin_trylock(&(obj->spin_lock)); \
} \
static inline uint32_t OBJNAME##_spinlock_irq_save(OBJSTRUCT* obj) \
{ \
	uint32_t irq_state = irq_save(); \
	spin_lock(&(obj->spin_lock)); \
	return irq_state; \
} \
static inline void OBJNAME##_spinunlock_irq_restore(OBJSTRUCT* obj, uint32_t irq_state) \
{ \
	spin_unlock(&(obj->spin_lock)); \
	irq_restore(irq_state); \
}

#else

#define	OBJECT_SPINLOCK_FUNC(OBJNAME,OBJSTRUCT) \
static inline void OBJNAME##_spininit(OBJSTRUCT* obj) \
{ \
} \
static inline void OBJNAME##_spinlock(OBJSTRUCT* obj) \
{ \
} \
static inline void OBJNAME##_spinunlock(OBJSTRUCT* obj) \
{ \
} \
static inline int OBJNAME##_spintrylock(OBJSTRUCT* obj) \
{ \
	return 1; \
} \
static inline uint32_t OBJNAME##_spinlock_irq_save(OBJSTRUCT* obj) \
{ \
	return irq_save(); \
} \
static inline void OBJNAME##_spinunlock_irq_restore(OBJSTRUCT* obj, uint32_t irq_state) \
{ \
	irq_restore(irq_state); \
}

#endif

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
