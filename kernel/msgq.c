/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

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
static void msgq_sub_init(void) {}
OBJECT_INDEX_FUNC(msgq,MsgqStruct,MSGQ_MAX_NUM);
OBJECT_SPINLOCK_FUNC(msgq,MsgqStruct);
OBJECT_SPINLOCK_FUNC(cpu,CpuStruct);

#if MSGQ_MAX_NUM != 0
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

/* 本関数呼び出し時 msgqはspinlock中であること */
uint32_t msgq_task_wakeup_body(MsgqStruct* msgq, bool (*check_and_copy)(MsgqStruct*,MsgqInfoStruct*))
{
	uint32_t	wakeup_cpu_list = 0;

	/* 送信待ちタスクでデータが格納できるものはすべて起床する */
	while ( !link_is_empty(&(msgq->link)) ) {
		Link* link = msgq->link.next;
		TaskStruct* task = containerof(link, TaskStruct, link);
		CpuStruct* cpu = task->cpu_struct;
		cpu_spinlock(cpu);
		MsgqInfoStruct* msgq_info = (MsgqInfoStruct*)(task->wait_obj);

		/* コピーされなかったら終了 */
		if ( !(*check_and_copy)(msgq, msgq_info) ) {
			cpu_spinunlock(cpu);
			break;
		}

		task->wait_func = 0; /* 再度同じ処理が走らないようにクリアする */
		task_wakeup_stub(task, RT_OK);
		wakeup_cpu_list |= (0x01u << task->cpu_struct->cpuid);
		cpu_spinunlock(cpu);
	}
	return wakeup_cpu_list;
}

uint32_t msgq_send_wakeup_body(MsgqStruct* msgq)
{
	return msgq_task_wakeup_body(msgq, msgq_send_check_and_copy);
}

uint32_t msgq_recv_wakeup_body(MsgqStruct* msgq)
{
	return msgq_task_wakeup_body(msgq, msgq_recv_check_and_copy);
}

/* msgq待ちのタスクがタイムアウトした場合 */
static void msgq_wait_func(TaskStruct* task, void* wait_obj)
{
	uint32_t	wakeup_cpu_list = 0;
	MsgqStruct* msgq = ((MsgqInfoStruct*)wait_obj)->msgq;
	CpuStruct* cpu = task->cpu_struct;

	/* タイムアウトしたタスクを起床させる(既に起床している場合は task_wakeup_stubは何もしない) */
	msgq_spinlock(msgq);
	cpu_spinlock(cpu);
	task_wakeup_stub(task, RT_TIMEOUT);
	cpu_spinunlock(cpu);

	if ( is_send_wait_mode(msgq) ) {
		wakeup_cpu_list = msgq_send_wakeup_body(msgq);
	}
	else if ( is_recv_wait_mode(msgq) ) {
		wakeup_cpu_list = msgq_recv_wakeup_body(msgq);
	}

	msgq_spinunlock(msgq);

	/* 起床/休止した全タスクに対応するcpuについて再スケジュール */
	wakeup_cpu_list = schedule_any(wakeup_cpu_list);

	if ( USE_SMP == 1 ) {
		/* 自コアは除いておく */
		uint32_t other_cpu_list = wakeup_cpu_list & ~(0x01u<<CPUID_get());
		if ( other_cpu_list ) {
			/* スケジュールされた全コアに 割り込み通知する */
			ipi_request_dispatch(other_cpu_list);
		}
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
			msgq_spininit(msgq);
			sync_barrier();
			ret = RT_OK;
		}
	}
	return ret;
}

int _kernel_msgq_tsend(MsgqStruct* msgq, void* ptr, uint32_t length, TimeOut tmout)
{
	uint32_t	irq_state;
	uint32_t	wakeup_cpu_list = 0;
	int ret = RT_OK;

retry_lock:
	irq_state = msgq_spinlock_irq_save(msgq);

	/* 要求サイズチェック */
	if ( length <= (msgq->length / 2) ) {

		if ( (length <= queue_space(msgq)) &&  !is_send_wait_mode(msgq) ) {
			/* 要求サイズ以上に空きがあり 送信待ちタスクがない場合は必ず書き込みできるので待ち状態にはならない */

			copy_to_queue(msgq, ptr, length);

			/* 受信待ちタスクでデータが満たされるものをすべて起床する */
			wakeup_cpu_list = msgq_recv_wakeup_body(msgq);
		}
		else if ( (tmout == TMO_POLL) || !can_dispatch() ) {
			/* ポーリングなのでタイムアウトエラーとする */
			ret = RT_TIMEOUT;
		}
		else {
			/* キューに空きがない状態 または すでに送信待ちタスクが存在する 待ち状態に遷移する */
			/* 最初に自cpuをlock */
			TaskStruct* task = task_self();
			CpuStruct* cpu = task->cpu_struct;
			if ( !cpu_spintrylock(cpu) ) {
				/* cpuがlockできなければいったんmsgq開放して再取得 */
				msgq_spinunlock_irq_restore(msgq, irq_state);
				goto retry_lock;
			}
			/* msgq + cpu をlock完了 */

			MsgqInfoStruct msgq_info;
			msgq_info.msgq = msgq;
			msgq_info.req_ptr = ptr;
			msgq_info.req_length = length;
			task_set_wait(task, &msgq_info, msgq_wait_func);
			task_remove_queue(task);
			link_add_last(&(msgq->link), &(task->link));
			if ( tmout != TMO_FEVER ) {
				/* タイムアウトリストに追加 */
				task_add_timeout(task, tmout);
			}
			wakeup_cpu_list = (0x01u << task->cpu_struct->cpuid);
			cpu_spinunlock(cpu);
		}
	}
	else {
		ret = RT_ERR;
	}
	msgq_spinunlock(msgq);

	/* 起床/休止した全タスクに対応するcpuについて再スケジュール */
	wakeup_cpu_list = schedule_any(wakeup_cpu_list);

	if ( USE_SMP == 1 ) {
		/* 自コアは除いておく */
		uint32_t other_cpu_list = wakeup_cpu_list & ~(0x01u<<CPUID_get());

		if ( other_cpu_list ) {
			/* スケジュールされた全コアに 割り込み通知する */
			ipi_request_dispatch(other_cpu_list);
		}
	}

	if ( wakeup_cpu_list & (0x01u<<CPUID_get()) ) {
		self_request_dispatch();
		ret = task_self()->result_code;
	}

	irq_restore(irq_state);

	return ret;
}

int _kernel_msgq_send(MsgqStruct* msgq, void* ptr, uint32_t length)
{
	return _kernel_msgq_tsend(msgq, ptr, length, TMO_FEVER);
}

int _kernel_msgq_trecv(MsgqStruct* msgq, void* ptr, uint32_t length, TimeOut tmout)
{
	uint32_t	irq_state;
	uint32_t	wakeup_cpu_list = 0;
	int ret = RT_OK;

retry_lock:
	irq_state = msgq_spinlock_irq_save(msgq);

	/* 要求サイズチェック */
	if ( length <= (msgq->length / 2) ) {

		if ( (length <= msgq->data_num) && !is_recv_wait_mode(msgq) ) {
			/* キューのデータが要求数以上あり かつ 受信待ちタスクがない場合は 必ず読み込みできるので待ち状態にはならない */

			copy_from_queue(msgq, ptr, length);

			/* 送信待ちタスクでデータが格納できるものはすべて起床する */
			wakeup_cpu_list = msgq_send_wakeup_body(msgq);
		}
		else if ( (tmout == TMO_POLL) || !can_dispatch() ) {
			/* ポーリングなのでタイムアウトエラーとする */
			ret = RT_TIMEOUT;
		}
		else {
			/* キューにデータがない -> 待ち状態になる */
			/* 最初に自cpuをlock */
			TaskStruct* task = task_self();
			CpuStruct* cpu = task->cpu_struct;
			if ( !cpu_spintrylock(cpu) ) {
				/* cpuがlockできなければいったんmsgq開放して再取得 */
				msgq_spinunlock_irq_restore(msgq, irq_state);
				goto retry_lock;
			}
			/* msgq + cpu をlock完了 */
			MsgqInfoStruct msgq_info;
			msgq_info.msgq = msgq;
			msgq_info.req_ptr = ptr;
			msgq_info.req_length = length;
			task_set_wait(task, &msgq_info, msgq_wait_func);
			task_remove_queue(task);
			link_add_last(&(msgq->link), &(task->link));
			if ( tmout != TMO_FEVER ) {
				/* タイムアウトリストに追加 */
				task_add_timeout(task, tmout);
			}
			wakeup_cpu_list = (0x01u << task->cpu_struct->cpuid);
			cpu_spinunlock(cpu);
		}
	}
	else {
		ret = RT_ERR;
	}

	msgq_spinunlock(msgq);

	/* 起床/休止した全タスクに対応するcpuについて再スケジュール */
	wakeup_cpu_list = schedule_any(wakeup_cpu_list);

	if ( USE_SMP == 1 ) {
		/* 自コアは除いておく */
		uint32_t other_cpu_list = wakeup_cpu_list & ~(0x01u<<CPUID_get());

		if ( other_cpu_list ) {
			/* スケジュールされた全コアに 割り込み通知する */
			ipi_request_dispatch(other_cpu_list);
		}
	}

	if ( wakeup_cpu_list & (0x01u<<CPUID_get()) ) {
		self_request_dispatch();
		ret = task_self()->result_code;
	}

	irq_restore(irq_state);

	return ret;
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
		MsgqStruct* msgq = msgqid2buffer(msgq_id);
		ret = _kernel_msgq_create(msgq, length);
		if ( ret == RT_OK ) {
			order_barrier();
			msgq->id_initialized = true;
			order_barrier();
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

#else
OSAPI int __msgq_create(uint32_t length)
{
	return RT_ERR;
}

OSAPI int __msgq_send(int id, void* ptr, uint32_t length)
{
	return RT_ERR;
}

OSAPI int __msgq_tsend(int id, void* ptr, uint32_t length, TimeOut tmout)
{
	return RT_ERR;
}

OSAPI int __msgq_recv(int id, void* ptr, uint32_t length)
{
	return RT_ERR;
}

OSAPI int __msgq_trecv(int id, void* ptr, uint32_t length, TimeOut tmout)
{
	return RT_ERR;
}
#endif
