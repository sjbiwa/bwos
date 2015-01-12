/*
 * flag.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "common.h"
#include "task.h"
#include "flag.h"
#include "link.h"
#include "api.h"

OSAPI int flag_create(FlagStruct* flag)
{
	link_clear(&flag->link);
	flag->value = 0;
	return RT_OK;
}

OSAPI int flag_set(FlagStruct* flag)
{
	uint32_t		cpsr;
	irq_save(cpsr);
	if ( !link_is_empty(&(flag->link)) ) {
		/* linkの先頭タスクをwakeup */
		Link* link = flag->link.next;
		link_remove(link);
		TaskStruct* task = containerof(link, TaskStruct, link);
		task_wakeup_stub(task, RT_OK);
		flag->value = 0;
		schedule();
	}
	else {
		flag->value = 1;
	}
	irq_restore(cpsr);
	return RT_OK;
}

OSAPI int flag_twait(FlagStruct* flag, TimeOut tmout)
{
	uint32_t		cpsr;
	irq_save(cpsr);

	/* リターンコード設定 */
	_ctask->result_code = RT_OK;

	if ( (flag->value != 0) && link_is_empty(&(flag->link)) ) {
		/* flag==WAKEUPで待ちタスクが無い場合は待たないで復帰する */
		flag->value = 0;
	}
	else {
		/* 待ちリストに追加 */
		_ctask->wait_obj = 0;
		_ctask->wait_func = 0;
		_ctask->task_state = TASK_WAIT;
		task_remove_queue(_ctask);
		link_add_last(&(flag->link), &(_ctask->link));
		if ( tmout != TMO_FEVER ) {
			/* タイムアウトリストに追加 */
			task_add_timeout(_ctask, tmout);
		}
		schedule();
	}
	irq_restore(cpsr);
	return _ctask->result_code;
}

OSAPI int flag_wait(FlagStruct* flag)
{
	return flag_twait(flag, TMO_FEVER);
}
