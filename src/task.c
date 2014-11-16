/*
 * task.c
 *
 *  Created on: 2012/12/01
 *      Author: biwa
 */

#include "common.h"
#include "arm.h"
#include "api.h"
#include "task.h"
#include "timer.h"

/* configuration end */
/***********************/

#define	TASK_MAX_NUM			(100)	/* 最大タスクID番号 */
#define	TASK_PRIORITY_NUM		(16)	/* タスク優先度レベル数 */

#define	EXE_DISPATCH()		\
				if ( (_ctask != _ntask) && (0 == _nodispatch_level) ) { \
					uint32_t flags = cpsr_get(); \
					irq_disable(); \
					_dispatch(); \
					cpsr_set(flags); \
				}

static struct {
	Link	task[TASK_PRIORITY_NUM];
} run_queue;

TaskStruct*		_ctask = NULL;			/* 現在実行中のタスク */
TaskStruct*		_ntask = NULL;			/* ディスパッチが要求されたタスク */
uint32_t		_nodispatch_level = 0;	/* 非ディスパッチレベル */

Link			task_time_out_list = {&task_time_out_list, &task_time_out_list};

static TaskStruct*	task_obj_cnv_tbl[TASK_MAX_NUM+1];

extern void schedule(void);

static void task_remove_queue(TaskStruct* task)
{
	Link*		top;
	Link*		end;
	Link*		curr = (Link*)task;

	top = curr->prev;
	end = curr->next;

	top->next = end;
	end->prev = top;
}

static void task_add_queue(TaskStruct* task, uint32_t pri)
{
	Link*		top;
	Link*		end;
	Link*		curr = (Link*)task;

	if ( pri < TASK_PRIORITY_NUM ) {
		top = &run_queue.task[pri];
		end = top->prev;
		curr->next = top;
		curr->prev = end;
		top->prev = curr;
		end->next = curr;
	}
}

static void task_remove_timeout_queue(Link* task)
{
	Link*		top;
	Link*		end;
	Link*		curr = task;

	top = curr->prev;
	end = curr->next;

	top->next = end;
	end->prev = top;
}

static void task_add_timeout_queue(Link* task)
{
	Link*		top;
	Link*		end;
	Link*		curr = task;

	top = &task_time_out_list;
	end = top->next;
	curr->next = end;
	curr->prev = top;
	top->next = curr;
	end->prev = curr;
}

static void task_rotate_queue(uint32_t pri)
{
	Link*		curr;
	if ( (pri < TASK_PRIORITY_NUM)  && (run_queue.task[pri].next != &(run_queue.task[pri])) ) {
		curr = run_queue.task[pri].next;
		task_remove_queue((TaskStruct*)curr);
		task_add_queue((TaskStruct*)curr, pri);
		schedule();
	}
}

static void dump_rq(void)
{
	Link*			link;
	TaskStruct*		task;
	tprintf("DUMP_RQ\n");
	for (link=run_queue.task[1].next; link != &run_queue.task[1]; link = link->next) {
		task = containerof(link, TaskStruct, link);
		tprintf("%s\n", task->name);
	}
	tprintf("DUMP_END\n");
}

static void dump_list(void)
{
	Link*			link;
	TaskStruct*		task;
#if 0
	for (link=task_time_out_list.next; link != &task_time_out_list; link = link->next) {
		task = containerof(link, TaskStruct, tlink);
		tprintf("IDLE %s\n", task->name);
	}
#endif
}

static void task_wakeup_stub(TaskStruct* task, int32_t result_code)
{
	uint32_t		cpsr;

	irq_save(cpsr);
	if ( task->task_state == TASK_WAIT ) {
		/* remove timeout queue */
		if ( task->tlink.next != NULL ) {
			task_remove_timeout_queue(&(task->tlink));
			task->tlink.next = task->tlink.prev = NULL;
		}
		task->result_code = result_code;
		task->task_state = TASK_READY;
		task_add_queue(task, task->priority);
		schedule();
	}
	irq_restore(cpsr);
}

void task_tick(void)
{
	Link*			link;
	TaskStruct*		task;

	for (link=task_time_out_list.next; link != &task_time_out_list; link = link->next) {
		task = containerof(link, TaskStruct, tlink);
		if ( task->timeout <= tick_count ) {
			link = link->prev;
			task_wakeup_stub(task, RT_TIMEOUT);
		}
	}
}

void schedule(void)
{
	uint32_t	cpsr;
	int			ix;

	irq_save(cpsr);
	_ntask = NULL;
	for (ix=0; ix<TASK_PRIORITY_NUM; ix++) {
		if ( run_queue.task[ix].next != &(run_queue.task[ix]) ) {
			_ntask = (TaskStruct*)(run_queue.task[ix].next);
			break;
		}
	}
	EXE_DISPATCH();
	irq_restore(cpsr);
}

void task_init(void)
{
	int			ix;
	for (ix=0; ix<TASK_PRIORITY_NUM; ix++) {
		run_queue.task[ix].next = run_queue.task[ix].prev = &run_queue.task[ix];
	}
	_ctask = NULL;
	_ntask = NULL;
}

OSAPI int task_create(TaskStruct* task)
{
	extern void	_entry_stub(void);
	uint32_t*		ptr;

	Link_clear(&task->link);
	Link_clear(&task->tlink);
	/* setup stack pointer */
	ptr = (uint32_t*)((uint32_t)(task->save_sp) + task->stack_size - TASK_FRAME_SIZE);
	task->save_sp = (void*)ptr;
	tprintf("task_create:%s sp=%08X\n", ptr);
	/* setup task-context */
	ptr[TASK_FRAME_STUB] = (void*)_entry_stub;
	ptr[TASK_FRAME_PC] = (uint32_t)task->start_entry;
	ptr[TASK_FRAME_PSR] = (cpsr_get() | FLAG_T ) & ~FLAG_I;
	tprintf("task : PC:%08X CPSR:%08X\n", ptr[TASK_FRAME_PC], ptr[TASK_FRAME_PSR]);
	/* setup TaskStruct */
	task->task_state = TASK_READY;

	task_add_queue(task, task->priority);

	return RT_OK;
}

OSAPI void task_sleep(void)
{
	uint32_t		cpsr;
	irq_save(cpsr);
	task_remove_queue(_ctask);
	_ctask->task_state = TASK_WAIT;
	schedule();
	irq_restore(cpsr);
}

OSAPI void task_wakeup(TaskStruct* task)
{
	task_wakeup_stub(task, RT_WAKEUP);
}

OSAPI int32_t task_tsleep(uint32_t tm)
{
	uint32_t		cpsr;
	irq_save(cpsr);
	task_remove_queue(_ctask);
	_ctask->task_state = TASK_WAIT;
	_ctask->timeout = tick_count + tm;
	_ctask->result_code = RT_TIMEOUT; /* デフォルトはタイムアウトに設定 */
	task_add_timeout_queue(&(_ctask->tlink));
	schedule();
	irq_restore(cpsr);
	return _ctask->result_code;
}
