/*
 * msgq.c
 *
 *  Created on: 2015/01/07
 *      Author: biwa
 */

#include "kernel.h"

/****************************************************************************/
/*	msgqの制限事項															*/
/*	send/recvで指定するlengthはcreateで作成したサイズ(length)/2以下とする	*/
/*	※これは、queueとsend/recvバッファ間のコピーが複数回に分割されることが	*/
/*	ないようにするためである。(バッファのラップアラウンドは除く)			*/
/****************************************************************************/

typedef	struct {
	MsgqStruct*		msgq;		/* 要求オブジェクト */
	void*			req_ptr;	/* 要求エリア */
	uint32_t		req_length;	/* 要求バイト数 */
} MsgqInfoStruct;

/* オブジェクト<->インデックス変換用 */
OBJECT_INDEX_FUNC(msgq,MsgqStruct,MSGQ_MAX_NUM);


static uint32_t queue_space(MsgqStruct* msgq)
{
	return msgq->length - msgq->data_num;
}

static bool is_send_wait_mode(MsgqStruct* msgq)
{
	return (!link_is_empty(&(msgq->link)) && ((msgq->length/2) < msgq->data_num)) ? true : false;
}

static bool is_recv_wait_mode(MsgqStruct* msgq)
{
	return (!link_is_empty(&(msgq->link)) && (msgq->data_num < (msgq->length/2))) ? true : false;
}

static void copy_to_queue(MsgqStruct* msgq, void* ptr, uint32_t length)
{
	uint32_t st_index = (msgq->data_top + msgq->data_num) % msgq->length;
	uint32_t ed_index = (st_index + length) % msgq->length;
	if ( st_index <= ed_index ) {
		/* 1回のコピー */
		memcpy(&msgq->data[st_index], ptr, length);
	}
	else {
		/* 2回のコピー */
		uint32_t f_length = msgq->length - st_index;
		memcpy(&msgq->data[st_index], ptr, f_length);
		memcpy(&msgq->data[0], ((uint8_t*)ptr) + f_length, ed_index);
	}
	msgq->data_num += length;
}

static void copy_from_queue(MsgqStruct* msgq, void* ptr, uint32_t length)
{
	uint32_t st_index = msgq->data_top;
	uint32_t ed_index = (st_index + length) % msgq->length;
	if ( st_index <= ed_index ) {
		/* 1回のコピー */
		memcpy(ptr, &msgq->data[st_index], length);
	}
	else {
		/* 2回のコピー */
		uint32_t f_length = msgq->length - st_index;
		memcpy(ptr, &msgq->data[st_index], f_length);
		memcpy(((uint8_t*)ptr) + f_length, &msgq->data[0], ed_index);
	}
	msgq->data_top = (msgq->data_top + length) % msgq->length;
	msgq->data_num -= length;
}

static bool msgq_send_check_and_copy(MsgqStruct* msgq, MsgqInfoStruct* msgq_info)
{
	bool ret = true;
	/* 要求数をチェック */
	if ( queue_space(msgq) < msgq_info->req_length ) {
		/* 足りないので終了 */
		ret = false;
	}
	else {
		/* 要求数を満たすのでバッファにコピー */
		copy_to_queue(msgq, msgq_info->req_ptr, msgq_info->req_length);
	}
	return ret;
}

static bool msgq_recv_check_and_copy(MsgqStruct* msgq, MsgqInfoStruct* msgq_info)
{
	bool ret = true;
	if ( msgq->data_num < msgq_info->req_length ) {
		/* 足りないので終了 */
		ret = false;
	}
	else {
		/* 要求数を満たすのでバッファからコピー */
		copy_from_queue(msgq, msgq_info->req_ptr, msgq_info->req_length);
	}
	return ret;
}

static void msgq_task_wakeup_body(MsgqStruct* msgq, bool (*check_and_copy)(MsgqStruct*,MsgqInfoStruct*))
{
	/* 送信待ちタスクでデータが格納できるものはすべて起床する */
	while ( !link_is_empty(&(msgq->link)) ) {
		Link* link = msgq->link.next;
		TaskStruct* task = containerof(link, TaskStruct, link);
		MsgqInfoStruct* msgq_info = (MsgqInfoStruct*)(task->wait_obj);

		/* コピーされなかったら終了 */
		if ( !(*check_and_copy)(msgq, msgq_info) ) {
			break;
		}

		link_remove(link);
		task->wait_func = 0; /* 再度同じ処理が走らないようにクリアする */
		task_wakeup_stub(task, RT_OK);
	}
}

static void msgq_send_wakeup_body(MsgqStruct* msgq)
{
	msgq_task_wakeup_body(msgq, msgq_send_check_and_copy);
}

static void msgq_recv_wakeup_body(MsgqStruct* msgq)
{
	msgq_task_wakeup_body(msgq, msgq_recv_check_and_copy);
}

/* msgq待ちのタスクがタイムアウトした場合 */
static void msgq_wait_func(TaskStruct* task)
{
	MsgqStruct* msgq = ((MsgqInfoStruct*)task->wait_obj)->msgq;

	if ( is_send_wait_mode(msgq) ) {
		msgq_send_wakeup_body(msgq);
	}
	else if ( is_recv_wait_mode(msgq) ) {
		msgq_recv_wakeup_body(msgq);
	}
	else {
		/* タイムアウトしたmsgq待ちタスクが存在するにもかかわらず */
		/* 送信/受信いずれも待っていない場合は不整合 */
		tprintf("MSGQ:damange\n");
	}
}

int _kernel_msgq_create(MsgqStruct* msgq, uint32_t length)
{
	int ret = RT_ERR;
	if ( 0 < length ) {
		msgq->data = __sys_malloc_align(length, sizeof(void*));
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

int _kernel_msgq_tsend(MsgqStruct* msgq, void* ptr, uint32_t length, TimeOut tmout)
{
	uint32_t		cpsr;

	irq_save(cpsr);

	/* 要求サイズチェック */
	if ( length <= (msgq->length / 2) ) {

		if ( (length <= queue_space(msgq)) &&  !is_send_wait_mode(msgq) ) {
			/* 要求サイズ以上に空きがあり 送信待ちタスクがない場合は必ず書き込みできるので待ち状態にはならない */

			copy_to_queue(msgq, ptr, length);

			/* 受信待ちタスクでデータが満たされるものをすべて起床する */
			msgq_recv_wakeup_body(msgq);

			schedule();
			_ctask->result_code = RT_OK;
		}
		else if ( tmout == TMO_POLL ) {
			/* ポーリングなのでタイムアウトエラーとする */
			_ctask->result_code = RT_OK;
		}
		else {
			/* キューに空きがない状態 または すでに送信待ちタスクが存在する 待ち状態に遷移する */
			MsgqInfoStruct msgq_info;
			msgq_info.msgq = msgq;
			msgq_info.req_ptr = ptr;
			msgq_info.req_length = length;
			task_set_wait(_ctask, &msgq_info, msgq_wait_func);
			task_remove_queue(_ctask);
			link_add_last(&(msgq->link), &(_ctask->link));
			if ( tmout != TMO_FEVER ) {
				/* タイムアウトリストに追加 */
				task_add_timeout(_ctask, tmout);
			}
			schedule();
		}
	}
	else {
		_ctask->result_code = RT_ERR;
	}

	irq_restore(cpsr);

	return _ctask->result_code;
}

int _kernel_msgq_send(MsgqStruct* msgq, void* ptr, uint32_t length)
{
	return _kernel_msgq_tsend(msgq, ptr, length, TMO_FEVER);
}

int _kernel_msgq_trecv(MsgqStruct* msgq, void* ptr, uint32_t length, TimeOut tmout)
{
	uint32_t cpsr;

	irq_save(cpsr);

	/* 要求サイズチェック */
	if ( length <= (msgq->length / 2) ) {

		if ( (length <= msgq->data_num) && !is_recv_wait_mode(msgq) ) {
			/* キューのデータが要求数以上あり かつ 受信待ちタスクがない場合は 必ず読み込みできるので待ち状態にはならない */

			copy_from_queue(msgq, ptr, length);

			/* 送信待ちタスクでデータが格納できるものはすべて起床する */
			msgq_send_wakeup_body(msgq);

			schedule();
			_ctask->result_code = RT_OK;
		}
		else if ( tmout == TMO_POLL ) {
			/* ポーリングなのでタイムアウトエラーとする */
			_ctask->result_code = RT_OK;
		}
		else {
			/* キューにデータがない -> 待ち状態になる */
			MsgqInfoStruct msgq_info;
			msgq_info.msgq = msgq;
			msgq_info.req_ptr = ptr;
			msgq_info.req_length = length;
			task_set_wait(_ctask, &msgq_info, msgq_wait_func);
			task_remove_queue(_ctask);
			link_add_last(&(msgq->link), &(_ctask->link));
			if ( tmout != TMO_FEVER ) {
				/* タイムアウトリストに追加 */
				task_add_timeout(_ctask, tmout);
			}
			schedule();
		}
	}
	else {
		_ctask->result_code = RT_ERR;
	}

	irq_restore(cpsr);

	return _ctask->result_code;
}

int _kernel_msgq_recv(MsgqStruct* msgq, void* ptr, uint32_t length)
{
	return _kernel_msgq_trecv(msgq, ptr, length, TMO_FEVER);
}

OSAPI int __msgq_create(uint32_t length)
{
	int ret = RT_ERR;
	int msgq_id = alloc_msgq_id();
	if ( 0 <= msgq_id ) {
		MsgqStruct* msgq = msgqid2object(msgq_id);
		ret = _kernel_msgq_create(msgq, length);
		if ( ret == RT_OK ) {
			ret = msgq_id;
		}
		else {
			free_msgq_struct(msgq_id);
		}
	}
	return ret;
}

OSAPI int __msgq_send(int id, void* ptr, uint32_t length)
{
	MsgqStruct* msgq = msgqid2object(id);
	return _kernel_msgq_send(msgq, ptr, length);
}

OSAPI int __msgq_tsend(int id, void* ptr, uint32_t length, TimeOut tmout)
{
	MsgqStruct* msgq = msgqid2object(id);
	return _kernel_msgq_tsend(msgq, ptr, length, tmout);
}

OSAPI int __msgq_recv(int id, void* ptr, uint32_t length)
{
	MsgqStruct* msgq = msgqid2object(id);
	return _kernel_msgq_recv(msgq, ptr, length);
}

OSAPI int __msgq_trecv(int id, void* ptr, uint32_t length, TimeOut tmout)
{
	MsgqStruct* msgq = msgqid2object(id);
	return _kernel_msgq_trecv(msgq, ptr, length, tmout);
}
