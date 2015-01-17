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

typedef	struct {
	FlagStruct*		obj;			/* フラグオブジェクト */
	uint32_t		pattern;		/* waitパターン */
	uint32_t		wait_mode;		/* waitモード */
	uint32_t*		ret_pattern;	/* completeフラグ値 */
} FlagInfoStruct;

static inline bool is_complete(uint32_t result, uint32_t pattern, uint32_t wait_mode)
{
	bool ret = false;
	if ( wait_mode & FLAG_OR ) {
		if ( (result & pattern) != 0 ) {
			ret = true;
		}
	}
	else if ( wait_mode & FLAG_AND ) {
		if ( result == pattern ) {
			ret = true;
		}
	}
	return ret;
}

static inline bool check_and_result(FlagStruct* flag, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern)
{
	bool ret = false;
	uint32_t result_pattern = flag->value & pattern;
	if ( is_complete(result_pattern, pattern, wait_mode) ) {
		/* completeしたのでフラグクリア処理 */
		if ( wait_mode & FLAG_CLR ) {
			flag->value = 0;
		}
		else if ( wait_mode & FLAG_BITCLR ) {
			flag->value &= ~result_pattern;
		}
		*ret_pattern = result_pattern;
		ret = true;
	}
	return ret;
}


OSAPI int flag_create(FlagStruct* flag)
{
	link_clear(&flag->link);
	flag->value = 0;
	return RT_OK;
}

OSAPI int flag_set(FlagStruct* flag, uint32_t pattern)
{
	Link* link;
	uint32_t cpsr;
	irq_save(cpsr);

	flag->value |= pattern;

	/* リストの全タスクをチェック */
	/* flag値が０ならwakeupしないので終了 */
	link = flag->link.next;
	while ( (link != &(flag->link)) && (flag->value != 0) ) {
		TaskStruct* task = containerof(link, TaskStruct, link);
		link = link->next; /* 現エントリがリストから外される可能性があるので先に次のエントリを取得しておく */
		FlagInfoStruct* flag_info = (FlagInfoStruct*)(task->wait_obj);
		if ( check_and_result(flag, flag_info->pattern, flag_info->wait_mode, flag_info->ret_pattern) ) {
			/* カレントタスクがcomplate */
			link_remove(&(task->link));
			task_wakeup_stub(task, RT_OK);
		}
	}
	schedule();

	irq_restore(cpsr);
	return RT_OK;
}

OSAPI int flag_twait(FlagStruct* flag, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern, TimeOut tmout)
{
	uint32_t		cpsr;
	irq_save(cpsr);

	/* リターンコード設定 */
	_ctask->result_code = RT_OK;

	/* フラグ値とタスクの待ちパターンとで処理する */
	if ( !check_and_result(flag, pattern, wait_mode, ret_pattern) ) {
		if ( tmout == TMO_POLL ) {
			/* ポーリングでcompleteしない場合 */
			_ctask->result_code = RT_TIMEOUT;
		}
		else {
			/* completeしない/ポーリングではない場合 */
			/* 待ちリストに追加 */
			FlagInfoStruct flag_info;
			flag_info.obj = flag;
			flag_info.pattern = pattern;
			flag_info.wait_mode = wait_mode;
			flag_info.ret_pattern = ret_pattern;
			_ctask->wait_obj = (void*)(&flag_info);
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
	}

	irq_restore(cpsr);
	return _ctask->result_code;
}

OSAPI int flag_wait(FlagStruct* flag, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern)
{
	return flag_twait(flag, pattern, wait_mode, ret_pattern, TMO_FEVER);
}

OSAPI int flag_clear(FlagStruct* flag, uint32_t pattern)
{
	uint32_t		cpsr;
	irq_save(cpsr);
	flag->value &= ~pattern;
	irq_restore(cpsr);
	return RT_OK;
}
