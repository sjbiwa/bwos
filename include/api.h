/*
 * api.h
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */

#ifndef INC_API_H_
#define INC_API_H_

#include <stdint.h>
#include "common.h"
#include "task.h"
#include "flag.h"
#include "mutex.h"
#include "sem.h"
#include "msgq.h"
#include "fixmb.h"
#include "malloc.h"
#include "sys_irq.h"

/* 終了ステータス */
#define	RT_OK			(0)
#define	RT_ERR			(-1)
#define	RT_TIMEOUT		(-2)
#define	RT_WAKEUP		(-3)
#define	RT_EVENT		(-4)

/* タイムアウト指定 */
#define	TMO_POLL		((TimeOut)0)
#define	TMO_FEVER		((TimeOut)(-1))

/* タスク関連API */
OSAPI int task_create(TaskStruct* task, TaskCreateInfo* info);
OSAPI int task_active(TaskStruct* task);
OSAPI int task_sleep(void);
OSAPI int task_wakeup(TaskStruct* task);
OSAPI int task_tsleep(TimeOut tm);
OSAPI int task_get_tls(TaskStruct* task, void** ptr);

/* フラグ関連API */
OSAPI int flag_create(FlagStruct* flag);
OSAPI int flag_set(FlagStruct* flag, uint32_t pattern);
OSAPI int flag_wait(FlagStruct* flag, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern);
OSAPI int flag_twait(FlagStruct* flag, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern, TimeOut tmout);
OSAPI int flag_clear(FlagStruct* flag, uint32_t pattern);

/* ミューテックス関連API */
OSAPI int mutex_create(MutexStruct* mtx);
OSAPI int mutex_unlock(MutexStruct* mtx);
OSAPI int mutex_lock(MutexStruct* mtx);
OSAPI int mutex_tlock(MutexStruct* mtx, TimeOut tmout);

/* セマフォ関連API */
OSAPI int sem_create(SemStruct* sem, uint32_t max);
OSAPI int sem_request(SemStruct* sem, uint32_t num);
OSAPI int sem_trequest(SemStruct* sem, uint32_t num, TimeOut tmout);
OSAPI int sem_release(SemStruct* sem, uint32_t num);

/* メッセージキュー関連API */
OSAPI int msgq_create(MsgqStruct* msgq, uint32_t length);
OSAPI int msgq_send(MsgqStruct* msgq, void* ptr);
OSAPI int msgq_tsend(MsgqStruct* msgq, void* ptr, TimeOut tmout);
OSAPI int msgq_recv(MsgqStruct* msgq, void** ptr);
OSAPI int msgq_trecv(MsgqStruct* msgq, void** ptr, TimeOut tmout);

/* 固定長メモリブロック関連API */
OSAPI int fixmb_create(FixmbStruct* fixmb, uint32_t mb_size, uint32_t length);
OSAPI int fixmb_request(FixmbStruct* fixmb, void** ptr);
OSAPI int fixmb_trequest(FixmbStruct* fixmb, void** ptr, TimeOut tmout);
OSAPI int fixmb_release(FixmbStruct* fixmb, void* ptr);

/* ヒープメモリ関連API */
OSAPI void* sys_malloc(uint32_t size);
OSAPI void sys_free(void* ptr);
OSAPI void* sys_malloc_align(uint32_t size, uint32_t align);

/* 割り込みハンドラ関連API */
OSAPI void irq_add_handler(uint32_t irqno, IRQ_HANDLER func, void* info);
OSAPI void irq_set_enable(uint32_t irqno);
OSAPI void irq_set_disable(uint32_t irqno);

#endif /* INC_API_H_ */
