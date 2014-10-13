/*
 * task.c
 *
 *  Created on: 2012/12/01
 *      Author: biwa
 */

#include "common.h"
#include "arm.h"
#include "task.h"

/* configuration task */
extern void task1(void);
extern void task2(void);
extern void task3(void);
extern void task4(void);
extern void task5(void);
static uint		stack[5][1024] __attribute__((aligned(16)));
TaskStruct		task_info[] = {
		{{0,0}, task1, stack[0], 1024, "TASK1", 1, 0},
		{{0,0}, task2, stack[1], 1024, "TASK2", 1, 0},
		{{0,0}, task3, stack[2], 1024, "TASK3", 1, 0},
		{{0,0}, task4, stack[3], 1024, "TASK4", 1, 0},
		{{0,0}, task5, stack[4], 1024, "TASK5", 1, 0},
};
/* configuration end */
/***********************/

#define	TASK_QUEUE_NUM		16

#define	EXE_DISPATCH()		\
				if ( (_ctask != _ntask) && (0 == _nodispatch_level) ) { \
					uint32_t flags = cpsr_get(); \
					irq_disable(); \
					_dispatch(); \
					cpsr_set(flags); \
				}

static struct {
	Link	task[TASK_QUEUE_NUM];
} run_queue;

TaskStruct*		_ctask = NULL;
TaskStruct*		_ntask = NULL;
uint32_t		_nodispatch_level = 0;

Link			task_time_out_list = {&task_time_out_list, &task_time_out_list};


extern void schedule(void);

static void
task_remove_queue(TaskStruct* task)
{
	Link*		top;
	Link*		end;
	Link*		curr = (Link*)task;

	top = curr->prev;
	end = curr->next;

	top->next = end;
	end->prev = top;
}

static void
task_add_queue(TaskStruct* task, uint32_t pri)
{
	Link*		top;
	Link*		end;
	Link*		curr = (Link*)task;

	if ( pri < TASK_QUEUE_NUM ) {
		top = &run_queue.task[pri];
		end = top->prev;
		curr->next = top;
		curr->prev = end;
		top->prev = curr;
		end->next = curr;
	}
}

static void
task_remove_timeout_queue(Link* task)
{
	Link*		top;
	Link*		end;
	Link*		curr = task;

	top = curr->prev;
	end = curr->next;

	top->next = end;
	end->prev = top;
}

static void
task_add_timeout_queue(Link* task)
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

static void
task_rotate_queue(uint32_t pri)
{
	Link*		curr;
	if ( (pri < TASK_QUEUE_NUM)  && (run_queue.task[pri].next != &(run_queue.task[pri])) ) {
		curr = run_queue.task[pri].next;
		task_remove_queue((TaskStruct*)curr);
		task_add_queue((TaskStruct*)curr, pri);
		schedule();
	}
}

static void
task_create(TaskStruct* tinfo, uint32_t taskid)
{
	extern void	_entry_stub(void);
	uint32_t*		ptr;
	Link_clear(&tinfo->link);
	Link_clear(&tinfo->tlink);
	/* setup stack pointer */
	ptr = (uint32_t*)((uint32_t)(tinfo->save_sp) + tinfo->stack_size - TASK_FRAME_SIZE);
	tinfo->save_sp = (void*)ptr;
	tprintf("task_create:%s sp=%08X\n", ptr);
	/* setup task-context */
	ptr[TASK_FRAME_STUB] = (void*)_entry_stub;
	ptr[TASK_FRAME_PC] = (uint32_t)tinfo->start_entry;
	ptr[TASK_FRAME_PSR] = (cpsr_get() | FLAG_T ) & ~FLAG_I;
	tprintf("task %d: PC:%08X CPSR:%08X\n", taskid, ptr[TASK_FRAME_PC], ptr[TASK_FRAME_PSR]);
	/* setup TaskStruct */
	tinfo->taskid = taskid;
	tinfo->task_state = TASK_READY;

	task_add_queue(tinfo, tinfo->priority);
}

static void
dump_rq(void)
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

static void
dump_list(void)
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

void
task_tick(void)
{
	Link*			link;
	TaskStruct*		task;

	dump_list();
	for (link=task_time_out_list.next; link != &task_time_out_list; link = link->next) {
		task = containerof(link, TaskStruct, tlink);
		if ( task->timeout <= tick_count ) {
			link = link->prev;
			task_wakeup(task->taskid);
		}
	}
}

void
schedule(void)
{
	uint32_t	cpsr;
	int			ix;

	irq_save(cpsr);
	_ntask = NULL;
	for (ix=0; ix<TASK_QUEUE_NUM; ix++) {
		if ( run_queue.task[ix].next != &(run_queue.task[ix]) ) {
			_ntask = (TaskStruct*)(run_queue.task[ix].next);
			break;
		}
	}
	EXE_DISPATCH();
	irq_restore(cpsr);
}

void
task_sleep(void)
{
	uint32_t		cpsr;
	irq_save(cpsr);
	task_remove_queue(_ctask);
	_ctask->task_state = TASK_WAIT;
	schedule();
	irq_restore(cpsr);
}

void
task_wakeup(int32_t tskid)
{
	uint32_t		cpsr;
	TaskStruct*		task;

	irq_save(cpsr);
	tskid--;
	task = &task_info[tskid];
	if ( task->task_state == TASK_WAIT ) {
		/* remove timeout queue */
		if ( task->tlink.next != NULL ) {
			task_remove_timeout_queue(&(task->tlink));
			task->tlink.next = task->tlink.prev = NULL;
		}

		task->task_state = TASK_READY;
		task_add_queue(task, task->priority);
		schedule();
	}
	irq_restore(cpsr);
}

void
task_tsleep(uint32_t tm)
{
	uint32_t		cpsr;
	irq_save(cpsr);
	task_remove_queue(_ctask);
	_ctask->task_state = TASK_WAIT;
	_ctask->timeout = tick_count + tm;
	task_add_timeout_queue(&(_ctask->tlink));
	schedule();
	irq_restore(cpsr);
}

void
task_init(void)
{
	int			ix;
	for (ix=0; ix<TASK_QUEUE_NUM; ix++) {
		run_queue.task[ix].next = run_queue.task[ix].prev = &run_queue.task[ix];
	}
	_ctask = NULL;
	_ntask = NULL;

	for (ix=0; ix<arrayof(task_info); ix++) {
		task_create(&task_info[ix], ix+1);
	}
}
