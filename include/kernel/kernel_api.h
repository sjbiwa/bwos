/*
 * kernel_api.h
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */

#ifndef INC_KERNEL_API_H_
#define INC_KERNEL_API_H_

#include <stdint.h>
#include "common.h"
#include "interface.h"
#include "kernel/task.h"
#include "kernel/flag.h"
#include "kernel/mutex.h"
#include "kernel/sem.h"
#include "kernel/msgq.h"
#include "kernel/fixmb.h"
#include "kernel/malloc.h"

/***********************************************/
/* オブジェクト<->インデックス変換用マクロ一式 */
/***********************************************/
#define	OBJECT_INDEX_FUNC(OBJNAME,OBJSTRUCT,MAX_NUM) \
static OBJSTRUCT *	OBJNAME##_struct_array = 0; \
static int			OBJNAME##_struct_max = 0; \
static int			OBJNAME##_struct_alloc_id = 0; \
static int alloc_##OBJNAME##_id(void) \
{ \
	int ret = RT_ERR; \
	if ( OBJNAME##_struct_alloc_id < OBJNAME##_struct_max ) { \
		ret = OBJNAME##_struct_alloc_id; \
		OBJNAME##_struct_alloc_id++; \
	} \
	return ret; \
} \
static OBJSTRUCT * OBJNAME##id2object(int id) \
{ \
	return &OBJNAME##_struct_array[id]; \
} \
static void free_##OBJNAME##_struct(int id) \
{ \
} \
void OBJNAME##_init(void) \
{ \
	OBJNAME##_struct_array = st_malloc_align(sizeof(OBJSTRUCT) * MAX_NUM, NORMAL_ALIGN); \
	OBJNAME##_struct_max = MAX_NUM; \
	OBJNAME##_struct_alloc_id = 0; \
}

/********************************************************/
/* OSAPISTUB											*/
/*	タスク/割り込みハンドラ(カーネル外)から呼び出される	*/
/********************************************************/
/* タスク関連API */
OSAPISTUB int __task_create(TaskCreateInfo* info);
OSAPISTUB int __task_active(int id);
OSAPISTUB int __task_sleep(void);
OSAPISTUB int __task_wakeup(int id);
OSAPISTUB int __task_tsleep(TimeOut tm);
OSAPISTUB int __task_dormant(void);
OSAPISTUB int __task_get_tls(int id, void** ptr);

/* フラグ関連API */
OSAPISTUB int __flag_create(void);
OSAPISTUB int __flag_set(int id, uint32_t pattern);
OSAPISTUB int __flag_wait(int id, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern);
OSAPISTUB int __flag_twait(int id, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern, TimeOut tmout);
OSAPISTUB int __flag_clear(int id, uint32_t pattern);

/* ミューテックス関連API */
OSAPISTUB int __mutex_create(void);
OSAPISTUB int __mutex_unlock(int id);
OSAPISTUB int __mutex_lock(int id);
OSAPISTUB int __mutex_tlock(int id, TimeOut tmout);

/* セマフォ関連API */
OSAPISTUB int __sem_create(uint32_t max);
OSAPISTUB int __sem_request(int id, uint32_t num);
OSAPISTUB int __sem_trequest(int id, uint32_t num, TimeOut tmout);
OSAPISTUB int __sem_release(int id, uint32_t num);

/* メッセージキュー関連API */
OSAPI int __msgq_create(uint32_t length);
OSAPI int __msgq_send(int id, void* ptr, uint32_t length);
OSAPI int __msgq_tsend(int id, void* ptr, uint32_t length, TimeOut tmout);
OSAPI int __msgq_recv(int id, void* ptr, uint32_t length);
OSAPI int __msgq_trecv(int id, void* ptr, uint32_t length, TimeOut tmout);

/* 固定長メモリブロック関連API */
OSAPISTUB int __fixmb_create(uint32_t mb_size, uint32_t length);
OSAPISTUB int __fixmb_request(int id, void** ptr);
OSAPISTUB int __fixmb_trequest(int id, void** ptr, TimeOut tmout);
OSAPISTUB int __fixmb_release(int id, void* ptr);

/* ヒープメモリ関連API */
OSAPISTUB void* __sys_malloc(MemSize_t size);
OSAPISTUB void __sys_free(void* ptr);
OSAPISTUB void* __sys_malloc_align(MemSize_t size, uint32_t align);

/* 割り込みハンドラ関連API */
OSAPISTUB void __irq_add_handler(uint32_t irqno, IRQ_HANDLER func, void* info);
OSAPISTUB void __irq_set_enable(uint32_t irqno, int setting);
OSAPISTUB int __irq_get_enable(uint32_t irqno);


/********************************************************/
/* KERNEL_API											*/
/*	カーネル内部から呼び出される						*/
/********************************************************/
/* タスク関連API */
int _kernel_task_create(TaskStruct* task, TaskCreateInfo* info);
int _kernel_task_active(TaskStruct* task);
int _kernel_task_sleep(void);
int _kernel_task_wakeup(TaskStruct* task);
int _kernel_task_tsleep(TimeOut tm);
int _kernel_task_dormant(void);
int _kernel_task_get_tls(TaskStruct* task, void** ptr);

/* フラグ関連API */
int _kernel_flag_create(FlagStruct* flag);
int _kernel_flag_set(FlagStruct* flag, uint32_t pattern);
int _kernel_flag_wait(FlagStruct* flag, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern);
int _kernel_flag_twait(FlagStruct* flag, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern, TimeOut tmout);
int _kernel_flag_clear(FlagStruct* flag, uint32_t pattern);

/* ミューテックス関連API */
int _kernel_mutex_create(MutexStruct* mtx);
int _kernel_mutex_unlock(MutexStruct* mtx);
int _kernel_mutex_lock(MutexStruct* mtx);
int _kernel_mutex_tlock(MutexStruct* mtx, TimeOut tmout);

/* セマフォ関連API */
int _kernel_sem_create(SemStruct* sem, uint32_t max);
int _kernel_sem_request(SemStruct* sem, uint32_t num);
int _kernel_sem_trequest(SemStruct* sem, uint32_t num, TimeOut tmout);
int _kernel_sem_release(SemStruct* sem, uint32_t num);

/* メッセージキュー関連API */
int _kernel_msgq_create(MsgqStruct* msgq, uint32_t length);
int _kernel_msgq_send(MsgqStruct* msgq, void* ptr, uint32_t length);
int _kernel_msgq_tsend(MsgqStruct* msgq, void* ptr, uint32_t length, TimeOut tmout);
int _kernel_msgq_recv(MsgqStruct* msgq, void* ptr, uint32_t length);
int _kernel_msgq_trecv(MsgqStruct* msgq, void* ptr, uint32_t length, TimeOut tmout);

/* 固定長メモリブロック関連API */
int _kernel_fixmb_create(FixmbStruct* fixmb, uint32_t mb_size, uint32_t length);
int _kernel_fixmb_request(FixmbStruct* fixmb, void** ptr);
int _kernel_fixmb_trequest(FixmbStruct* fixmb, void** ptr, TimeOut tmout);
int _kernel_fixmb_release(FixmbStruct* fixmb, void* ptr);


#endif /* INC_KERNEL_API_H_ */