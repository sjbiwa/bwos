/*
 * task.c
 *
 *  Created on: 2012/12/01
 *      Author: biwa
 */

#include "common.h"
#include "arm.h"
#include "api.h"
#include "link.h"
#include "task.h"
#include "timer.h"

/* configuration end */
/***********************/

#define	TASK_MAX_NUM			(100)	/* 最大タスクID番号 */
#define	TASK_PRIORITY_NUM		(32)	/* タスク優先度レベル数 */

#define	EXE_DISPATCH()		\
				if ( (_ctask != _ntask) && (0 == _nodispatch_level) ) { \
					uint32_t flags = cpsr_get(); \
					irq_disable(); \
					_dispatch(); \
					cpsr_set(flags); \
				}

static struct {
	uint32_t	pri_bits;				/* 優先度毎の有効ビット */
	Link		task[TASK_PRIORITY_NUM];/* 優先度毎のRUNキュー */
} run_queue;

TaskStruct*		_ctask = NULL;			/* 現在実行中のタスク */
TaskStruct*		_ntask = NULL;			/* ディスパッチが要求されたタスク */
uint32_t		_nodispatch_level = 0;	/* 非ディスパッチレベル */

Link			task_time_out_list = {&task_time_out_list, &task_time_out_list};

static TaskStruct*	task_obj_cnv_tbl[TASK_MAX_NUM+1];

extern void schedule(void);

static inline int32_t lowest_bit(uint32_t value)
{
	int32_t ret = 0;
	if ( (value & 0x0000ffff) == 0 ) {
		ret = 16;
		value >>= 16;
	}
	if ( (value & 0x00ff) == 0 ) {
		ret += 8;
		value >>= 8;
	}
	if ( (value & 0x0f) == 0 ) {
		ret += 4;
		value >>= 4;
	}
	if ( (value & 0x3) == 0 ) {
		ret += 2;
		value >>= 2;
	}
	if ( (value & 0x1) == 0 ) {
		ret += 1;
	}
	return ret;
}

void task_remove_queue(TaskStruct* task)
{
	uint32_t	pri = task->priority;
	link_remove(&(task->link));
	/* 優先度キューに登録がなければ該当ビットをクリア */
	if ( link_is_empty(&run_queue.task[pri]) ) {
		run_queue.pri_bits &= ~(0x00000001u << pri);
	}
}

static void task_add_queue(TaskStruct* task)
{
	Link*		top;
	uint32_t	pri = task->priority;
	if ( pri < TASK_PRIORITY_NUM ) {
		top = &run_queue.task[pri];
		link_add_last(top, &(task->link));
		run_queue.pri_bits |= (0x00000001u << pri);
	}
}

static void task_remove_timeout_queue(TaskStruct* task)
{
	link_remove(&(task->tlink));
}

static void task_add_timeout_queue(TaskStruct* task)
{
	Link*		top;

	top = &task_time_out_list;
	link_add_last(top, &(task->tlink));
}

static void task_rotate_queue(uint32_t pri)
{
	Link*		curr;
	if ( (pri < TASK_PRIORITY_NUM)  && !link_is_empty(&run_queue.task[pri]) ) {
		curr = run_queue.task[pri].next;
		task_remove_queue(containerof(curr, TaskStruct, link));
		task_add_queue(containerof(curr, TaskStruct, link));
		schedule();
	}
}

void task_wakeup_stub(TaskStruct* task, int32_t result_code)
{
	uint32_t		cpsr;

	if ( task->task_state == TASK_WAIT ) {
		/* remove timeout queue */
		if ( !link_is_empty(&(task->tlink)) ) {
			task_remove_timeout_queue(task);
			link_clear(&(task->tlink));
		}
		task->result_code = result_code;
		task->task_state = TASK_READY;
		task_add_queue(task);
	}
}

void task_tick(void)
{
	Link*			link;
	TaskStruct*		task;
	bool			req_sched = false;
	link = task_time_out_list.next;
	while ( link != &task_time_out_list ) {
		task = containerof(link, TaskStruct, tlink);
		link = link->next; /* 次のタスクを取得しておく */
		if ( task->timeout <= tick_count ) {
			task_wakeup_stub(task, RT_TIMEOUT);
			req_sched = true;
		}
	}
	if ( req_sched ) {
		schedule();
	}
}

void schedule(void)
{
	uint32_t	cpsr;

	irq_save(cpsr);
	_ntask = NULL;
	if ( run_queue.pri_bits != 0 ) {
		uint32_t bits = lowest_bit(run_queue.pri_bits);
		_ntask = (TaskStruct*)(run_queue.task[bits].next);
	}
	EXE_DISPATCH();
	irq_restore(cpsr);
}

void task_init(void)
{
	int			ix;
	run_queue.pri_bits = 0x00000000;
	for (ix=0; ix<TASK_PRIORITY_NUM; ix++) {
		link_clear(&run_queue.task[ix]);
	}
	_ctask = NULL;
	_ntask = NULL;
}

OSAPI int task_create(TaskStruct* task)
{
	extern void	_entry_stub(void);
	uint32_t*		ptr;

	link_clear(&task->link);
	link_clear(&task->tlink);
	/* setup stack pointer */
	ptr = (uint32_t*)((uint32_t)(task->save_sp) + task->stack_size - TASK_FRAME_SIZE);
	task->save_sp = (void*)ptr;
	/* setup task-context */
	ptr[TASK_FRAME_STUB] = (void*)_entry_stub;
	ptr[TASK_FRAME_PC] = (uint32_t)task->start_entry;
	ptr[TASK_FRAME_PSR] = (cpsr_get() | FLAG_T ) & ~FLAG_I;
	/* setup TaskStruct */
	task->task_state = TASK_READY;

	task_add_queue(task);

	return RT_OK;
}

void task_sleep_stub(TaskStruct* task)
{
	task_remove_queue(task);
	task->task_state = TASK_WAIT;
}

OSAPI void task_sleep(void)
{
	uint32_t		cpsr;
	irq_save(cpsr);
	task_sleep_stub(_ctask);
	schedule();
	irq_restore(cpsr);
}

OSAPI void task_wakeup(TaskStruct* task)
{
	uint32_t		cpsr;
	irq_save(cpsr);
	task_wakeup_stub(task, RT_WAKEUP);
	schedule();
	irq_restore(cpsr);
}

OSAPI int32_t task_tsleep(uint32_t tm)
{
	uint32_t		cpsr;
	irq_save(cpsr);
	task_sleep_stub(_ctask);
	_ctask->timeout = tick_count + tm;
	_ctask->result_code = RT_TIMEOUT; /* デフォルトはタイムアウトに設定 */
	task_add_timeout_queue(_ctask);
	schedule();
	irq_restore(cpsr);
	return _ctask->result_code;
}
