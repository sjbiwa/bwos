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
	uint32_t	value;
} FlagStruct;

OSAPI int flag_create(FlagStruct* flag)
{
	link_clear(&flag->link);
	flag->value = 0;
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
		flag->value = 0;
		schedule();
	}
	else {
		flag->value = 1;
	}
	irq_restore(cpsr);
}

OSAPI int flag_wait(FlagStruct* flag, uint32_t tmout)
{
	uint32_t		cpsr;
	irq_save(cpsr);
	if ( (flag->value != 0) && (flag->link.prev == flag->link.next) ) {

	}
	else {
		task_remove_queue(_ctask);
		_ctask->task_state = TASK_WAIT;
		link_add_last(&(flag->link), _ctask);
		schedule();
	}
	irq_restore(cpsr);
}
