/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

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
#include "smp.h"

#define	TASK_PRIORITY_NUM		(32)	/* タスク優先度レベル数 */

/* cpu/task情報へのアクセスマクロ */
#define	RUN_QUEUE()			(cpu_struct[CPUID_get()].run_queue)
#define	TIMEOUT_LIST()		(cpu_struct[CPUID_get()].task_time_out_list)
#define	CTASK()				(cpu_struct[CPUID_get()].ctask)
#define	NTASK()				(cpu_struct[CPUID_get()].ntask)

struct tagCpuStruct;

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
#if !defined(NO_USE_SVC_STACK)
	void*		init_sp;			/* Initialize SP (SVC) */
	MemSize_t	stack_size;			/* Stack Size (SVC) */
#endif
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
	void		(*wait_func)(struct tagTaskStruct* task, void* wait_obj); /* 待ち状態解除時コールバック */
	int32_t		result_code;		/* API完了コード */
	bool		id_initialized;		/* 初期完了フラグ */
	struct tagCpuStruct* cpu_struct;
} TaskStruct;

typedef struct tagRunQueue {
	int32_t		pri_highest;			/* 最高優先度タスク */
	uint32_t	pri_bits;				/* 優先度毎の有効ビット */
	Link		task[TASK_PRIORITY_NUM];/* 優先度毎のRUNキュー */
} RunQueue;

typedef	struct tagCpuStruct {
	TaskStruct*		ctask;
	TaskStruct*		ntask;
#if USE_SMP==1
	SpinLockObj		spin_lock;
#endif
	uint32_t		cpuid;
	Link			task_time_out_list;
	Link			timer_list;
	RunQueue		run_queue;
} CpuStruct;

extern CpuStruct cpu_struct[CPU_NUM];

extern bool can_dispatch(void);
extern void task_remove_queue(TaskStruct* task);
extern void task_wakeup_stub(TaskStruct* task, int32_t result_code);
extern void task_add_timeout(TaskStruct* task, TimeOut tm);
extern bool schedule(CpuStruct* cpu);
extern uint32_t schedule_any(uint32_t wakeup_cpu_list);
extern void self_request_dispatch(void);

static inline void task_set_wait(TaskStruct* task, void* wait_obj, void (*wait_func)(struct tagTaskStruct* task, void* wait_obj))
{
	task->wait_obj = wait_obj;
	task->wait_func = wait_func;
	task->task_state = TASK_WAIT;
}

static inline TaskStruct* task_self(void)
{
	return cpu_struct[CPUID_get()].ctask;
}

#endif /* TASK_H_ */
