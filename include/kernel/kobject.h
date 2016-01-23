/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * kobject.h
 *
 *  Created on: 2016/01/23
 *      Author: biwa
 */

#ifndef _KOBJECT_H_
#define _KOBJECT_H_

#include "kernel.h"

/***********************************************/
/* オブジェクト<->インデックス変換用マクロ一式 */
/***********************************************/
#define	OBJECT_INDEX_FUNC(OBJNAME,OBJSTRUCT,MAX_NUM) \
static OBJSTRUCT *	OBJNAME##_struct_array = 0; \
static int			OBJNAME##_struct_max = 0; \
static int			OBJNAME##_struct_alloc_id = 0; \
static SpinLockObj	OBJNAME##_struct_spinlock; \
static void OBJNAME##_sub_init(); \
void OBJNAME##_init(void) \
{ \
	if ( 0 < MAX_NUM ) { \
		OBJNAME##_struct_array = __st_malloc_align(sizeof(OBJSTRUCT) * MAX_NUM, NORMAL_ALIGN); \
	} \
	else { \
		OBJNAME##_struct_array = 0; \
	} \
	OBJNAME##_struct_max = MAX_NUM; \
	OBJNAME##_struct_alloc_id = 0; \
	OBJNAME##_struct_alloc_id = 0; \
	spin_init(&(OBJNAME##_struct_spinlock)); \
	OBJNAME##_sub_init(); \
}\
static int alloc_##OBJNAME##_id(void) \
{ \
	uint32_t irq_state; \
	irq_state = irq_save(); \
	spin_lock(&(OBJNAME##_struct_spinlock)); \
	int ret = RT_ERR; \
	if ( OBJNAME##_struct_alloc_id < OBJNAME##_struct_max ) { \
		ret = OBJNAME##_struct_alloc_id; \
		OBJNAME##_struct_array[ret].id_initialized = false; \
		OBJNAME##_struct_alloc_id++; \
	} \
	spin_unlock(&(OBJNAME##_struct_spinlock)); \
	irq_restore(irq_state); \
	return ret; \
} \
static OBJSTRUCT * OBJNAME##id2object(int id) \
{ \
	OBJSTRUCT * ret = NULL; \
	order_barrier(); \
	if ( (0 <= id) && (id < OBJNAME##_struct_alloc_id) && OBJNAME##_struct_array[id].id_initialized ) { \
		ret = &OBJNAME##_struct_array[id]; \
	} \
	return ret; \
} \
static OBJSTRUCT * OBJNAME##id2buffer(int id) \
{ \
	return &OBJNAME##_struct_array[id]; \
} \
static void free_##OBJNAME##_struct(int id) \
{ \
}

#endif /* _KOBJECT_H_ */
