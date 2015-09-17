/*
 * Copyright (C) 2015 syujibiwa All rights reserved.
 */

/*
 * interface.h
 *
 *  Created on: 2015/03/07
 *      Author: biwa
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "stdint.h"
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
	void		(*entry)();			/* Start Entry */
	void*		usr_init_sp;		/* Initialize SP */
	uint32_t	usr_stack_size;		/* Stack Size */
	uint32_t	tls_size;			/* TLS size */
	uint32_t	priority;			/* Task Priority */
	void*		cre_param;			/* Create Parameter */
} TaskCreateInfo;

/* タスク属性定義 */
#define	TASK_ACT			(0x00000001u<<0)	/* タスク生成時にレディーキューに登録する */
#define	TASK_FPU			(0x00000001u<<1)	/* FPUを使用できるタスク */
#define	TASK_SYS			(0x00000001u<<2)	/* SYSTEMモード(特権)タスク */
#define	CPU_MASK			(0xFFu<<24)
#define	CPU_SELF			(0xFFu<<24)
#define	CPU_CORE0			(0x00u<<24)
#define	CPU_CORE1			(0x01u<<24)
#define	CPU_CORE2			(0x02u<<24)
#define	CPU_CORE3			(0x03u<<24)
#define	CPU_GET(f)			(((f)&CPU_MASK)>>24)

#define	TASK_SELF			(0)


/* フラグ関連API wait_mode */
#define	FLAG_OR			(0x00000001u<<0)
#define	FLAG_AND		(0x00000001u<<1)
#define	FLAG_CLR		(0x00000001u<<2)
#define	FLAG_BITCLR		(0x00000001u<<3)


/* 割り込みハンドラ関連API */
#define	IRQ_DISABLE		(0)
#define	IRQ_ENABLE		(1)


/* タイマハンドラ関連API */
typedef	struct {
	TimeOut		tmout;					/* タイムアウト/最初のcyclic 時間 */
	TimeOut		cyclic;					/* 2回目以降のcyclic 時間 */
	uint32_t	kind;					/* タイマ種別 */
	void		(*handler)(void* param);/* タイマexpire時に呼び出される関数 */
	void*		param;					/* handlerに渡されるパラメータ */
} TimerInfo;

#define	TIMER_ONESHOT	(0)			/* tmoutのみ設定 */
#define	TIMER_CYCLIC	(1)			/* tmout/cyclic両方設定 */


extern uint32_t cpsr_get(void);
extern void cpsr_set(uint32_t flags);


#endif /* INTERFACE_H_ */
