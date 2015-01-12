/*
 * msgq.c
 *
 *  Created on: 2015/01/07
 *      Author: biwa
 */

#include "common.h"
#include "task.h"
#include "msgq.h"
#include "link.h"
#include "api.h"

typedef	struct {
	void*			ptr;
} MsgqInfoStruct;

OSAPI int msgq_create(MsgqStruct* msgq, uint32_t length)
{
	int ret = RT_ERR;
	if ( 0 < length ) {
		msgq->data = sys_malloc_align(sizeof(void*) * length, sizeof(void*));
		if ( msgq->data ) {
			link_clear(&msgq->link);
			msgq->length = length;
			msgq->data_top = 0;
			msgq->data_num = 0;
			ret = RT_OK;
		}
	}
	return ret;
}

OSAPI int msgq_tsend(MsgqStruct* msgq, void* ptr, TimeOut tmout)
{
	uint32_t		cpsr;

	irq_save(cpsr);

	if ( msgq->data_num < msgq->length ) {
		/* キューに空きがある->必ず書き込みできるので待ち状態にはならない */
		/* 受信待ちタスクがいるかチェック */
		if ( !link_is_empty(&(msgq->link)) ) {
			/* 受信待ちタスクがいるのでそのタスクにデータを直接渡して起床させる */
			Link* link = msgq->link.next;
			link_remove(link);
			TaskStruct* task = containerof(link, TaskStruct, link);
			task_wakeup_stub(task, RT_OK);
			((MsgqInfoStruct*)(task->wait_obj))->ptr = ptr;
			schedule();
		}
		else {
			/* 受信待ちタスクがいないのでキューに書き込んで終了 */
			((void**)msgq->data)[(msgq->data_top + msgq->data_num) % msgq->length] = ptr;
			msgq->data_num++;
		}
		_ctask->result_code = RT_OK;
	}
	else {
		/* キューに空きがない状態->待ち状態に遷移する */
		MsgqInfoStruct msgq_info;
		msgq_info.ptr = ptr;
		_ctask->wait_obj = &msgq_info;
		_ctask->wait_func = 0;
		_ctask->task_state = TASK_WAIT;
		task_remove_queue(_ctask);
		link_add_last(&(msgq->link), &(_ctask->link));
		if ( tmout != TMO_FEVER ) {
			/* タイムアウトリストに追加 */
			task_add_timeout(_ctask, tmout);
		}
		schedule();
	}

	irq_restore(cpsr);

	return _ctask->result_code;
}

OSAPI int msgq_send(MsgqStruct* msgq, void* ptr)
{
	return msgq_tsend(msgq, ptr, TMO_FEVER);
}

OSAPI int msgq_trecv(MsgqStruct* msgq, void** ptr, TimeOut tmout)
{
	uint32_t cpsr;

	irq_save(cpsr);

	/* 送信待ちの場合は処理リトライするのでループにする */
	if ( 0 < msgq->data_num ) {
		/* キューにデータがある -> 読み込みできるので待ち状態にならない */
		*ptr = ((void**)msgq->data)[msgq->data_top];
		msgq->data_num--;
		msgq->data_top = (msgq->data_top + 1) % msgq->length;

		/* 送信待ちタスクがいる場合は先頭タスクが書き込むデータをキューに入れてタスク起床 */
		if ( !link_is_empty(&(msgq->link)) ) {
			Link* link = msgq->link.next;
			link_remove(link);
			TaskStruct* task = containerof(link, TaskStruct, link);
			task_wakeup_stub(task, RT_OK);
			(msgq->data)[(msgq->data_top + msgq->data_num) % msgq->length] =
											((MsgqInfoStruct*)(task->wait_obj))->ptr;
			msgq->data_num++;
			schedule();
		}
		_ctask->result_code = RT_OK;
	}
	else {
		/* キューにデータがない -> 待ち状態になる */
		MsgqInfoStruct msgq_info;
		_ctask->wait_obj = &msgq_info;
		_ctask->wait_func = 0;
		_ctask->task_state = TASK_WAIT;
		task_remove_queue(_ctask);
		link_add_last(&(msgq->link), &(_ctask->link));
		if ( tmout != TMO_FEVER ) {
			/* タイムアウトリストに追加 */
			task_add_timeout(_ctask, tmout);
		}
		schedule();
		if ( _ctask->result_code == RT_OK ) {
			*ptr = msgq_info.ptr;
		}
	}

	irq_restore(cpsr);

	return _ctask->result_code;
}

OSAPI int msgq_recv(MsgqStruct* msgq, void** ptr)
{
	return msgq_trecv(msgq, ptr, TMO_FEVER);
}

