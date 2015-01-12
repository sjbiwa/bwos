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

static TaskStruct	init_task_struct;	/* 初期タスク構造体 */
static uint32_t		init_task_stack[1024];
static Link		task_time_out_list = {&task_time_out_list, &task_time_out_list};

static TaskStruct*	task_obj_cnv_tbl[TASK_MAX_NUM+1];

extern void schedule(void);
extern void init_task(void);

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
	if ( link != &task_time_out_list ) {
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

static void task_sleep_stub(TaskStruct* task)
{
	task_remove_queue(task);
	link_clear(&(task->link));
	task->wait_obj = 0;
	task->wait_func = 0;
	task->task_state = TASK_WAIT;
}

void task_add_timeout(TaskStruct* task, TimeOut tm)
{
	task->timeout = get_tick_count() + tm;
	task_add_timeout_queue(task);
}

void task_wakeup_stub(TaskStruct* task, int32_t result_code)
{
	uint32_t		cpsr;

	if ( task->task_state == TASK_WAIT ) {
		/* 待ちリストに登録されている場合はリストから削除 */
		if ( !link_is_empty(&(task->link)) ) {
			link_remove(&(task->link));
			link_clear(&(task->link));
		}

		/* タイムアウトキューに登録されている場合はリストから削除 */
		if ( !link_is_empty(&(task->tlink)) ) {
			link_remove(&(task->tlink));
			link_clear(&(task->tlink));
		}

		task->result_code = result_code;
		task->task_state = TASK_READY;
		task_add_queue(task);

		/* タスク起床時にコールバック関数呼び出し */
		if ( task->wait_func != 0 ) {
			(task->wait_func)(task);
		}
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
	/* キューは昇順に登録されているので先頭のみチェックしていれば良い */
	/* 先頭タスクが起床されたら次のタスクが先頭タスクになるため */
	while ( (link = task_time_out_list.next) != &task_time_out_list ) {
		task = containerof(link, TaskStruct, tlink);
		/* タスクの起床時間が大きかったらそこで終了 */
		if ( tick_count < task->timeout ) {
			break;
		}
		task_wakeup_stub(task, RT_TIMEOUT);
		req_sched = true;
	}
	if ( req_sched ) {
		schedule();
	}
#if defined(USE_TICKLESS)
	link = task_time_out_list.next;
	if ( link != &task_time_out_list ) {
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

static inline void task_init_struct(TaskStruct* task, uint8_t* name, uint32_t task_attr, void* entry,
					void* init_sp, uint32_t stack_size, uint32_t priority)
{
	link_clear(&task->link);
	task->save_sp = 0;
	task->arch_tls = 0;
	if ( name ) {
		strncpy(task->name, name, sizeof(task->name)-1);
	}
	else {
		task->name[0] = '\0';
	}
	task->task_attr = task_attr;
	task->entry = entry;
	task->init_sp = init_sp;
	task->stack_size = stack_size;
	task->priority = priority;
	task->tls = 0;
	task->tls_size = 0;
	task->task_state = TASK_STANDBY;
	link_clear(&task->tlink);
	task->timeout = 0;
	task->wait_obj = 0;
	task->wait_func = 0;
	task->result_code = 0;
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

	/* 初期タスクの生成 */
	task_init_struct(&init_task_struct,
						"INIT_TASK",
						TASK_ACT,
						init_task,
						init_task_stack,
						sizeof(init_task_stack),
						0);
	arch_init_task_create(&init_task_struct);
	init_task_struct.task_state = TASK_READY;
	task_add_queue(&init_task_struct);
}

OSAPI int task_create(TaskStruct* task, TaskCreateInfo* info)
{
	task_init_struct(task,
						info->name,
						info->task_attr,
						info->entry,
						info->init_sp,
						info->stack_size,
						info->priority);

	/* setup stack pointer */
	if ( task->init_sp == 0 ) {
		/* stackをヒープから確保 */
		task->init_sp = sys_malloc_align(task->stack_size, 8);
	}
	/* TLS alloc */
	if ( (task->tls == NULL) && (task->tls_size != 0) ) {
		task->tls = sys_malloc_align(task->tls_size, 8);
		memset(task->tls, 0, task->tls_size);
	}

	arch_task_create(task); /* ARCH depend create */

	if ( task->task_attr & TASK_ACT ) {
		uint32_t		cpsr;
		irq_save(cpsr);
		task->task_state = TASK_READY;
		task_add_queue(task);
		schedule();
		irq_restore(cpsr);
	}

	return RT_OK;
}

OSAPI int task_active(TaskStruct* task)
{
	uint32_t		cpsr;
	irq_save(cpsr);
	if ( task->task_state == TASK_STANDBY ) {
		task->task_state = TASK_READY;
		task_add_queue(task);
		schedule();
	}
	irq_restore(cpsr);

	return RT_OK;
}

OSAPI int task_sleep(void)
{
	uint32_t		cpsr;
	irq_save(cpsr);
	task_sleep_stub(_ctask);
	schedule();
	irq_restore(cpsr);
	return RT_OK;
}

OSAPI int task_wakeup(TaskStruct* task)
{
	uint32_t		cpsr;
	irq_save(cpsr);
	task_wakeup_stub(task, RT_WAKEUP);
	schedule();
	irq_restore(cpsr);
	return RT_OK;
}

OSAPI int task_tsleep(TimeOut tm)
{
	uint32_t		cpsr;
	irq_save(cpsr);
	task_sleep_stub(_ctask);
	_ctask->timeout = get_tick_count() + tm;
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
