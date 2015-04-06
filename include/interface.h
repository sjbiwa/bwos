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


/* 割り込みハンドラ関連API */
#define	IRQ_DISABLE		(0)
#define	IRQ_ENABLE		(1)


#endif /* INTERFACE_H_ */
