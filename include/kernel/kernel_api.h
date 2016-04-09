/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * kernel_api.h
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */

#ifndef _KERNEL_API_H_
#define _KERNEL_API_H_

#include <stdint.h>
#include "common.h"
#include "interface.h"
#include "kernel/task.h"
#include "kernel/flag.h"
#include "kernel/mutex.h"
#include "kernel/sem.h"
#include "kernel/msgq.h"
#include "kernel/fixmb.h"
#include "kernel/smp.h"

/********************************************************/
/* OSAPISTUB											*/
/*	タスク/割り込みハンドラ(カーネル外)から呼び出される	*/
/********************************************************/
/* タスク関連API */
OSAPISTUB int __task_create(TaskCreateInfo* info);
OSAPISTUB int __task_active(int id, void* act_param);
OSAPISTUB int __task_sleep(void);
OSAPISTUB int __task_wakeup(int id);
OSAPISTUB int __task_tsleep(TimeOut tm);
OSAPISTUB int __task_dormant(void);
OSAPISTUB int __task_get_tls(int id, void** ptr);
OSAPISTUB int __task_set_affinity(uint32_t aff);

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
#if defined(SYS_MALLOC_REGION_NUM)
OSAPISTUB void* __sys_malloc_align_rgn(MemSize_t size, uint32_t align, uint32_t rgn);
OSAPISTUB void* __sys_malloc_rgn(MemSize_t size, uint32_t rgn);
#endif

/* 割り込みハンドラ関連API */
OSAPISTUB void __irq_add_handler(uint32_t irqno, IRQ_HANDLER func, void* info);
OSAPISTUB void __irq_set_enable(uint32_t irqno, int setting, uint32_t irq_attr);
OSAPISTUB int __irq_get_enable(uint32_t irqno);

/* タイマハンドラ関連API */
OSAPISTUB int __timer_create(void);
OSAPISTUB int __timer_set(int id, TimerInfo* info);
OSAPISTUB int __timer_enable(int id, bool enable);

/********************************************************/
/* KERNEL_API											*/
/*	カーネル内部から呼び出される						*/
/********************************************************/
/* タスク関連API */
KERNAPI int _kernel_task_create(TaskStruct* task, TaskCreateInfo* info);
KERNAPI int _kernel_task_active(TaskStruct* task, void* act_param);
KERNAPI int _kernel_task_sleep(void);
KERNAPI int _kernel_task_wakeup(TaskStruct* task);
KERNAPI int _kernel_task_tsleep(TimeOut tm);
KERNAPI int _kernel_task_dormant(void);
KERNAPI int _kernel_task_get_tls(TaskStruct* task, void** ptr);

/* フラグ関連API */
KERNAPI int _kernel_flag_create(FlagStruct* flag);
KERNAPI int _kernel_flag_set(FlagStruct* flag, uint32_t pattern);
KERNAPI int _kernel_flag_wait(FlagStruct* flag, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern);
KERNAPI int _kernel_flag_twait(FlagStruct* flag, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern, TimeOut tmout);
KERNAPI int _kernel_flag_clear(FlagStruct* flag, uint32_t pattern);

/* ミューテックス関連API */
KERNAPI int _kernel_mutex_create(MutexStruct* mtx);
KERNAPI int _kernel_mutex_unlock(MutexStruct* mtx);
KERNAPI int _kernel_mutex_lock(MutexStruct* mtx);
KERNAPI int _kernel_mutex_tlock(MutexStruct* mtx, TimeOut tmout);

/* セマフォ関連API */
KERNAPI int _kernel_sem_create(SemStruct* sem, uint32_t max);
KERNAPI int _kernel_sem_request(SemStruct* sem, uint32_t num);
KERNAPI int _kernel_sem_trequest(SemStruct* sem, uint32_t num, TimeOut tmout);
KERNAPI int _kernel_sem_release(SemStruct* sem, uint32_t num);

/* メッセージキュー関連API */
KERNAPI int _kernel_msgq_create(MsgqStruct* msgq, uint32_t length);
KERNAPI int _kernel_msgq_send(MsgqStruct* msgq, void* ptr, uint32_t length);
KERNAPI int _kernel_msgq_tsend(MsgqStruct* msgq, void* ptr, uint32_t length, TimeOut tmout);
KERNAPI int _kernel_msgq_recv(MsgqStruct* msgq, void* ptr, uint32_t length);
KERNAPI int _kernel_msgq_trecv(MsgqStruct* msgq, void* ptr, uint32_t length, TimeOut tmout);

/* 固定長メモリブロック関連API */
KERNAPI int _kernel_fixmb_create(FixmbStruct* fixmb, uint32_t mb_size, uint32_t length);
KERNAPI int _kernel_fixmb_request(FixmbStruct* fixmb, void** ptr);
KERNAPI int _kernel_fixmb_trequest(FixmbStruct* fixmb, void** ptr, TimeOut tmout);
KERNAPI int _kernel_fixmb_release(FixmbStruct* fixmb, void* ptr);

/* 低レベルメモリ割り当て関連 */
KERNAPI extern void  __sys_malloc_init(void);
KERNAPI extern void  __sys_malloc_add_block(void* addr, MemSize_t size);
KERNAPI extern void* __sys_malloc_body(MemSize_t size);
KERNAPI extern void* __sys_malloc_align_body(MemSize_t size, uint32_t align);
#if defined(SYS_MALLOC_REGION_NUM)
KERNAPI void __sys_malloc_add_block_rgn(void* start_addr, MemSize_t size, uint32_t rgn);
KERNAPI void* __sys_malloc_align_body_rgn(MemSize_t size, uint32_t align, uint32_t rgn);
KERNAPI void* __sys_malloc_body_rgn(MemSize_t size, uint32_t rgn);
#endif
/**/
KERNAPI extern void  __st_malloc_init(void* mem_addr, MemSize_t mem_size);
KERNAPI extern void* __st_malloc_align(MemSize_t alloc_size, uint32_t align);
KERNAPI extern void  __st_malloc_normalize(void);

#endif /* _KERNEL_API_H_ */
