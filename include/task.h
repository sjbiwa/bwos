/*
 * task.h
 *
 *  Created on: 2012/12/01
 *      Author: biwa
 */

#ifndef TASK_H_
#define TASK_H_

#include "common.h"
#include "link.h"

#define	TASK_SELF			(0)
#define	TASK_RESERVE		{0,0},0,0

/* タスク属性定義 */
#define	TASK_ACT			(0x00000001u<<0)	/* タスク生成時にレディーキューに登録する */
#define	TASK_FPU			(0x00000001u<<1)	/* FPUを使用できるタスク */
#define	TASK_SYS			(0x00000001u<<2)	/* SYSTEMモード(特権)タスク */

typedef	enum { TASK_STANDBY, TASK_READY, TASK_WAIT, TASK_DORMANT } TaskState;
typedef	struct tagTaskStruct {
	/* Fixed Position */
	Link		link;				/* ReadQueue/EventQueue LinkList */
	void*		save_sp;			/* Current SP (This is Arch depend ?) */
	void*		arch_tls;			/* ARCH依存 TaskLocalStorage */
	/******************/
	/* ユーザー定義領域 */
	uint8_t		name[32];			/* Task Name */
	uint32_t	task_attr;			/* Task属性 */
	void		(*entry)(void);		/* Start Entry */
	void*		init_sp;			/* Initialize SP (SVC) */
	MemSize_t	stack_size;			/* Stack Size (SVC) */
	void*		usr_init_sp;		/* Initial USER SP (USR or SYS) */
	MemSize_t	usr_stack_size;		/* Initial Stack Size (USR or SYS) */
	uint32_t	priority;			/* Task Priority */
	void*		tls;				/* TaskLocalStorage */
	MemSize_t	tls_size;			/* TLS size */
	/******************/
	/* カーネル使用領域 */
	TaskState	task_state;			/* Task State */
	Link		tlink;				/* TimeOut LinkList */
	TimeSpec	timeout;			/* TimeOut Time */
	void*		wait_obj;			/* 待ち状態となった対象オブジェクト */
	void		(*wait_func)(struct tagTaskStruct* task); /* 待ち状態解除時コールバック */
	int32_t		result_code;		/* API完了コード */
} TaskStruct;

extern	TaskStruct*			_ctask;
extern	TaskStruct*			_ntask;

extern void task_remove_queue(TaskStruct* task);
extern void task_wakeup_stub(TaskStruct* task, int32_t result_code);

extern void task_add_timeout(TaskStruct* task, TimeOut tm);

static inline task_set_wait(TaskStruct* task, void* wait_obj, void (*wait_func)(struct tagTaskStruct* task))
{
	task->wait_obj = wait_obj;
	task->wait_func = wait_func;
	task->task_state = TASK_WAIT;
}

#endif /* TASK_H_ */
