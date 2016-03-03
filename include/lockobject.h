/*
 * Copyright (C) 2016 sjbiwa All rights reserved.
 */

/*
 * lockobject.h
 *
 *  Created on: 2016/03/04
 *      Author: biwa
 */

#ifndef _LOCKOBJECT_H_
#define _LOCKOBJECT_H_

#include "bwos.h"
#include "kernel/smp.h"

/* オブジェクト用spinlock関数 */
#if USE_SMP==1

#define	SPINLOCK_OBJECT()		SpinLockObj spin_lock

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

#define	SPINLOCK_OBJECT()

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

#endif /* INCLUDE_LOCK_H_ */
