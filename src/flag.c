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
		/* linkの先頭タスクをwakeup */
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
	(void)tmout; /* 現在は未実装 */
	uint32_t		cpsr;
	irq_save(cpsr);
	_ctask->result_code = RT_WAKEUP;
	if ( (flag->value != 0) && link_is_empty(&(flag->link)) ) {
		/* flag==WAKEUPで待ちタスクが無い場合は待たないで復帰する */
		flag->value = 0;
	}
	else {
		task_remove_queue(_ctask);
		_ctask->task_state = TASK_WAIT;
		link_add_last(&(flag->link), _ctask);
		schedule();
	}
	irq_restore(cpsr);
	return _ctask->result_code;
}
