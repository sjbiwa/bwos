/*
 * task.c
 *
 *  Created on: 2012/12/01
 *      Author: biwa
 */

#include "common.h"
#include "api.h"
#include "link.h"
#include "task.h"
#include "timer.h"

/* configuration end */
/***********************/

#define	TASK_MAX_NUM			(100)	/* 最大タスクID番号 */
#define	TASK_PRIORITY_NUM		(32)	/* タスク優先度レベル数 */

#define	EXE_DISPATCH()		\
				if ( can_dispatch() ) { \
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

Link			task_time_out_list = {&task_time_out_list, &task_time_out_list};

static TaskStruct*	task_obj_cnv_tbl[TASK_MAX_NUM+1];

extern void schedule(void);


static inline bool can_dispatch(void)
{
	bool ret = false;
	if ( (_ctask != _ntask) && arch_can_dispatch() ) {
		ret = true;
	}
	return ret;
}

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

/* タイムアウトキューにタスクを登録する */
static void task_add_timeout_queue(TaskStruct* task)
{
	TaskStruct*	q_task;
	Link*		link;
	/************************************************************/
	/* パターン													*/
	/*  1: タイムアウトキューに登録なし -> キューの先頭に登録	*/
	/*	2: タイムアウトキューの途中								*/
	/*	3: タイムアウトキュー全てより遅い ->　キューの最後尾	*/
	/************************************************************/
	link = task_time_out_list.next;
	while ( link != &task_time_out_list ) {
		q_task = containerof(link, TaskStruct, tlink);
		/* 対象タスクの起床時間が検索中キューのタスクより早ければ */
		/* その直前に登録する */
		if ( task->timeout <= q_task->timeout ) {
			/* linkの前が登録位置 */
			break;
		}
		link = link->next;
	}
	/* link_add_lastはlinkの直前に入れるので期待通り */
	link_add_last(link, &(task->tlink));

#if defined(USE_TICKLESS)
	link = task_time_out_list.next;
	while ( link != &task_time_out_list ) {
		q_task = containerof(link, TaskStruct, tlink);
		update_first_timeout(q_task->timeout);
	}
#endif
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
	uint32_t		cpsr;
	TimeSpec		tick_count;
	irq_save(cpsr);
	/* タイムアウトキューに登録されているタスクの起床処理 */
	tick_count = get_tick_count();
	link = task_time_out_list.next;
	while ( link != &task_time_out_list ) {
		task = containerof(link, TaskStruct, tlink);
		/* タスクの起床時間が大きかったらそこで終了 */
		if ( tick_count < task->timeout ) {
			break;
		}
		link = link->next;
		task_wakeup_stub(task, RT_TIMEOUT);
		req_sched = true;
	}
	if ( req_sched ) {
		schedule();
	}
#if defined(USE_TICKLESS)
	link = task_time_out_list.next;
	while ( link != &task_time_out_list ) {
		task = containerof(link, TaskStruct, tlink);
		update_first_timeout(task->timeout);
	}
#endif
	irq_restore(cpsr);
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
	link_clear(&task->link);
	link_clear(&task->tlink);

	/* setup TaskStruct */
	arch_task_create(task); /* ARCH depend create */
	task->task_state = TASK_READY;

#if 1
/* TLS alloc */
	if ( (task->tls == NULL) && (task->tls_size != 0) ) {
		task->tls = __sys_malloc_align(task->tls_size, 8);
		memset(task->tls, 0, task->tls_size);
	}
#endif
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

OSAPI int32_t task_tsleep(TimeOut tm)
{
	uint32_t		cpsr;
	irq_save(cpsr);
	task_sleep_stub(_ctask);
	_ctask->timeout = get_tick_count() + tm;
	_ctask->result_code = RT_TIMEOUT; /* デフォルトはタイムアウトに設定 */
	task_add_timeout_queue(_ctask);
	schedule();
	irq_restore(cpsr);
	return _ctask->result_code;
}

OSAPI void* task_get_tls(TaskStruct* task)
{
	if ( task == TASK_SELF ) {
		task = _ctask;
	}
	return task->tls;
}
