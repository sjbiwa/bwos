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

/* 終了ステータス */
#define	RT_OK			(0)
#define	RT_ERR			(-1)
#define	RT_TIMEOUT		(-2)
#define	RT_WAKEUP		(-3)
#define	RT_EVENT		(-4)

/* タイムアウト指定 */
#define	TMO_POLL		((TimeOut)0)
#define	TMO_FEVER		((TimeOut)(-1))

/* タスク生成パラメータ */
typedef	struct {
	uint8_t*	name;				/* Task Name */
	uint32_t	task_attr;			/* Task属性 */
	void		(*entry)(void);		/* Start Entry */
	void*		usr_init_sp;		/* Initialize SP */
	uint32_t	usr_stack_size;		/* Stack Size */
	uint32_t	tls_size;			/* TLS size */
	uint32_t	priority;			/* Task Priority */
} TaskCreateInfo;

/* タスク属性定義 */
#define	TASK_ACT			(0x00000001u<<0)	/* タスク生成時にレディーキューに登録する */
#define	TASK_FPU			(0x00000001u<<1)	/* FPUを使用できるタスク */
#define	TASK_SYS			(0x00000001u<<2)	/* SYSTEMモード(特権)タスク */

#define	TASK_SELF			(0)

/* フラグ関連API wait_mode */
#define	FLAG_OR			(0x00000001u<<0)
#define	FLAG_AND		(0x00000001u<<1)
#define	FLAG_CLR		(0x00000001u<<2)
#define	FLAG_BITCLR		(0x00000001u<<3)


/* タスク関連API */
OSAPI int task_create(TaskCreateInfo* info);
OSAPI int task_active(int id);
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
OSAPI void irq_set_enable(uint32_t irqno);
OSAPI void irq_set_disable(uint32_t irqno);

#endif /* INC_API_H_ */
