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
#include "interface.h"

/* タスク関連API */
OSAPI int task_create(TaskCreateInfo* info);
OSAPI int task_active(int id, void* act_param);
OSAPI int task_sleep(void);
OSAPI int task_wakeup(int id);
OSAPI int task_tsleep(TimeOut tm);
OSAPI int task_dormant(void);
OSAPI int task_get_tls(int id, void** ptr);

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
OSAPI int msgq_trecv(int id, void** ptr, uint32_t length, TimeOut tmout);

/* 固定長メモリブロック関連API */
OSAPI int fixmb_create(uint32_t mb_size, uint32_t length);
OSAPI int fixmb_request(int id, void** ptr);
OSAPI int fixmb_trequest(int id, void** ptr, TimeOut tmout);
OSAPI int fixmb_release(int id, void* ptr);

/* ヒープメモリ関連API */
OSAPI void* sys_malloc(uint32_t size);
OSAPI void sys_free(void* ptr);
OSAPI void* sys_malloc_align(uint32_t size, uint32_t align);

/* 割り込みハンドラ関連API */
OSAPI void irq_add_handler(uint32_t irqno, IRQ_HANDLER func, void* info);
OSAPI void irq_set_enable(uint32_t irqno, int setting);
OSAPI int irq_get_enable(uint32_t irqno);

/* タイマハンドラ関連API */
OSAPI int timer_create(void);
OSAPI int timer_set(int id, TimerInfo* info);
OSAPI int timer_enable(int id, bool enable);

#endif /* INC_API_H_ */
