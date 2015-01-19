/*
 * fixmb.c
 *
 *  Created on: 2015/01/16
 *      Author: biwa
 */
#include "common.h"
#include "task.h"
#include "fixmb.h"
#include "link.h"
#include "api.h"

#define	NO_ENTRY	(0xfffffffu)

static bool check_bitmap(uint32_t* bitmap, uint32_t cu_index)
{
	bool ret = false;
	if ( bitmap[cu_index/32] & (0x00000001u << (cu_index%32)) ) {
		ret = true;
	}
	return ret;
}

static void update_bitmap(uint32_t* bitmap, uint32_t cu_index, bool flag)
{
	if ( flag ) {
		bitmap[cu_index/32] |= (0x00000001u << (cu_index%32);
	}
	else {
		bitmap[cu_index/32] &= ~(0x00000001u << (cu_index%32));
	}
}

static void* fixmb_get_mb(FixmbStruct* fixmb, uint32_t index)
{
	return (void*)((uint32_t*)(fixmb->mb_area) + (fixmb->mb_size * index));
}

OSAPI int fixmb_create(FixmbStruct* fixmb, uint32_t mb_size, uint32_t length)
{
	uint32_t ix;
	uint32_t bitmap_num;

	link_clear(&fixmb->link);
	if ( mb_size < sizeof(FixmbList) ) {
		mb_size = sizeof(FixmbList);
	}
	fixmb->mb_size = POST_ALIGN_BY(mb_size, 16); /* サイズは16バイトの倍数にする */
	fixmb->mb_length = length;
	fixmb->mb_area = sys_malloc_align(fixmb->mb_size * length, 16);

	fixmb->is_list_mode = false;
	fixmb->list.top.index = fixmb->list.last.index = NO_ENTRY;
	fixmb->mb_array_index = 0;
	fixmb->use_count = 0;

	bitmap_num = (length + 31) / 32);
	fixmb->use_bitmap = sys_malloc_align(bitmap_num * 4, 4);
	for ( ix=0; ix < bitmap_num; ix++ ) {
		fixmb->use_bitmap[ix] = 0;
	}

	return 0;
}


OSAPI int fixmb_trequest(FixmbStruct* fixmb, void** ptr, TimeOut tmout)
{
	uint32_t alloc_index;
	uint32_t cpsr;
	irq_save(cpsr);

	_ctask->result_code = RT_OK;

	if ( fixmb->use_count < fixmb->mb_length ) {
		/* 割り当てブロックがある */
		if ( !fixmb->is_list_mode ) {
			/* 配列から割り当て */
			alloc_index = fixmb->mb_array_index;
			fixmb->mb_array_index++;
			if ( fixmb->mb_length <= fixmb->mb_array_index ) {
				fixmb->is_list_mode = true;
			}
		}
		else {
			/* リストから割り当て */
			alloc_index = fixmb->list.top.next;
		}
		*ptr = fixmb_get_mb(fixmb, alloc_index);
		update_bitmap(fixmb->use_bitmap, alloc_index, true);
		fixmb->use_count++;
		if ( fixmb->mb_length <= fixmb->use_count ) {
			/* 空きがなくなった */
			fixmb->list.top.index = fixmb->list.last.index = NO_ENTRY;
		}
	}
	else if ( tmout == TMO_POLL ) {
		/* 割り当てブロックがない */
		/* ポーリングなのでタイムアウトエラーとする */
		_ctask->result_code = RT_TIMEOUT;
	}
	else {
		/* 割り当てブロックがないので待ち状態に遷移 */
		_ctask->wait_obj = 0;
		_ctask->wait_func = 0;
		_ctask->task_state = TASK_WAIT;
		task_remove_queue(_ctask);
		link_add_last(&(fixmb->link), &(_ctask->link));
		if ( tmout != TMO_FEVER ) {
			/* タイムアウトリストに追加 */
			task_add_timeout(_ctask, tmout);
		}
		schedule();
	}

	irq_restore(cpsr);
	return _ctask->result_code;
}

OSAPI int fixmb_request(FixmbStruct* fixmb, void** ptr)
{
	return fixmb_trequest(fixmb, ptr, TMO_FEVER);
}

OSAPI int fixmb_release(FixmbStruct* fixmb, void* ptr)
{
	uint32_t area_size;
	uint32_t		cpsr;
	int				ret = RT_OK;
	irq_save(cpsr);

	/* 解放するアドレスの妥当性チェック */
	uint32_t rel_index = (uint32_t)((uint8_t*)ptr - (uint8_t*)(fixmb->mb_area)) / fixmb->mb_size;
	if ( (fixmb->mb_length <= rel_index) || !check_bitmap(fixmb->use_bitmap, cu_index) ) {
		/* 範囲外なのでエラー */
		ret = RT_ERR;
	}
	else {
		if ( link_is_empty(&(fixmb->link)) ) {
			/* 解放待ちタスクがないので空きリストに返却 */
			update_bitmap(fixmb->use_bitmap, rel_index, false);
			fixmb->use_count--;
			if ( fixmb->list.top.index == NO_ENTRY ) {
				/* リストに新規登録 */
				fixmb->list.top.index = fixmb->list.last.index = rel_index;
			}
			else {
				/* リストに追記登録 */
				FixmbList* rel_entry = fixmb_get_mb(fixmb, rel_index);
				FixmbList* last_entry = fixmb_get_mb(fixmb, fixmb->list.last.next);
				last_entry->index = rel_index;
				fixmb->list.last.index = rel_entry;
			}
		}
		else {
			/* 解放待ちタスクがあるのでwakeup */
			Link* link = fixmb->link.next;
			link_remove(link);
			TaskStruct* task = containerof(link, TaskStruct, link);
			task_wakeup_stub(task, RT_OK);
			schedule();
		}
	}

	irq_restore(cpsr);
	return ret;
}
