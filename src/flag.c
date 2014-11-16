/*
 * flag.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "common.h"
#include "task.h"


typedef	struct tagFlagStruct {
	Link		link;
} FlagStruct;

OSAPI int flag_create(FlagStruct* flag)
{
	link_clear(&flag->link);
	return 0;
}

OSAPI void flag_set(FlagStruct* flag)
{
	uint32_t		cpsr;
	irq_save(cpsr);
	Link* link = flag->link.next;
	if ( link ) {
		link_remove(link);
		TaskStruct* task = containerof(link, TaskStruct, link);
		task_wakeup_stub(task, RT_EVENT);
	}
	irq_restore(cpsr);
}

OSAPI int flag_wait(FlagStruct* flag, uint32_t tmout)
{
	uint32_t		cpsr;
	irq_save(cpsr);
	task_remove_queue(_ctask);
	_ctask->task_state = TASK_WAIT;

	Link* link = flag->link.next;
	if ( link ) {
		link_remove(link);
		TaskStruct* task = containerof(link, TaskStruct, link);
		task->task_state = TASK_READY;
		task->result_code = RT_EVENT;
		schedule();
	}
	irq_restore(cpsr);
}
