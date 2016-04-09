/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * bwos.h
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */

#ifndef INC_BWOS_H_
#define INC_BWOS_H_

#include "common.h"
#include "interface.h"
#include "link.h"

#if defined(NO_USE_SVC_CALL)
#include "no_svc_call.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* タスク関連API */
OSAPI int task_create(TaskCreateInfo* info);
OSAPI int task_active(int id, void* act_param);
OSAPI int task_sleep(void);
OSAPI int task_wakeup(int id);
OSAPI int task_tsleep(TimeOut tm);
OSAPI int task_dormant(void);
OSAPI int task_get_tls(int id, void** ptr);
#if USE_SMP == 1
OSAPI int task_set_affinity(uint32_t aff);
#endif

/* フラグ関連API */
OSAPI int flag_create(void);
OSAPI int flag_set(int id, uint32_t pattern);
OSAPI int flag_wait(int id, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern);
OSAPI int flag_twait(int id, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern, TimeOut tmout);
OSAPI int flag_clear(int id, uint32_t pattern);

/* ミューテックス関連API */
OSAPI int mutex_create(void);
OSAPI int mutex_unlock(int id);
OSAPI int mutex_lock(int id);
OSAPI int mutex_tlock(int id, TimeOut tmout);

/* セマフォ関連API */
OSAPI int sem_create(uint32_t max);
OSAPI int sem_request(int id, uint32_t num);
OSAPI int sem_trequest(int id, uint32_t num, TimeOut tmout);
OSAPI int sem_release(int id, uint32_t num);

/* メッセージキュー関連API */
OSAPI int msgq_create(uint32_t length);
OSAPI int msgq_send(int id, void* ptr, uint32_t length);
OSAPI int msgq_tsend(int id, void* ptr, uint32_t length, TimeOut tmout);
OSAPI int msgq_recv(int id, void* ptr, uint32_t length);
OSAPI int msgq_trecv(int id, void* ptr, uint32_t length, TimeOut tmout);

/* 固定長メモリブロック関連API */
OSAPI int fixmb_create(uint32_t mb_size, uint32_t length);
OSAPI int fixmb_request(int id, void** ptr);
OSAPI int fixmb_trequest(int id, void** ptr, TimeOut tmout);
OSAPI int fixmb_release(int id, void* ptr);

/* ヒープメモリ関連API */
OSAPI void* sys_malloc(uint32_t size);
OSAPI void sys_free(void* ptr);
OSAPI void* sys_malloc_align(uint32_t size, uint32_t align);
#if defined(SYS_MALLOC_REGION_NUM)
OSAPI void* sys_malloc_align_rgn(MemSize_t size, uint32_t align, uint32_t rgn);
OSAPI void* sys_malloc_rgn(MemSize_t size, uint32_t rgn);
#endif

/* 割り込みハンドラ関連API */
OSAPI void irq_add_handler(uint32_t irqno, IRQ_HANDLER func, void* info);
OSAPI void irq_set_enable(uint32_t irqno, int setting, uint32_t irq_attr);
OSAPI int irq_get_enable(uint32_t irqno);

/* タイマハンドラ関連API */
OSAPI int timer_create(void);
OSAPI int timer_set(int id, TimerInfo* info);
OSAPI int timer_enable(int id, bool enable);

/* 初期タスクから呼び出される関数群 */
extern void main_task(void); /* ユーザ側で最初に呼び出される関数 */
extern void arch_init_task_depend(void);

#ifdef __cplusplus
}
#endif

#endif /* _BWOS_H_ */
