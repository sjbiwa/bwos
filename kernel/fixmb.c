/*
 * fixmb.c
 *
 *  Created on: 2015/01/16
 *      Author: biwa
 */
#include "kernel.h"

typedef	struct {
	FixmbStruct*	obj;			/* MBオブジェクト */
	void**			ptr;			/* 確保したMBアドレス格納エリアへのポインタ */
} FixmbInfoStruct;

#define	NO_ENTRY	(0xfffffffu)

/* オブジェクト<->インデックス変換用 */
OBJECT_INDEX_FUNC(fixmb,FixmbStruct,FIXMB_MAX_NUM);


/* 該当ブロックの使用中状態チェック */
static bool check_bitmap(uint32_t* bitmap, uint32_t cu_index)
{
	bool ret = false;
	if ( bitmap[cu_index/32] & (0x00000001u << (cu_index%32)) ) {
		ret = true;
	}
	return ret;
}

/* 該当ブロックの使用中状態更新 */
static void update_bitmap(uint32_t* bitmap, uint32_t cu_index, bool flag)
{
	if ( flag ) {
		bitmap[cu_index/32] |= (0x00000001u << (cu_index%32));
	}
	else {
		bitmap[cu_index/32] &= ~(0x00000001u << (cu_index%32));
	}
}

/* ブロックインデックスからポインタへ変換 */
static void* fixmb_idx2ptr(FixmbStruct* fixmb, uint32_t index)
{
	return (void*)((uint8_t*)(fixmb->mb_area) + (fixmb->mb_size * index));
}

/* ポインタからブロックインデックスへ変換 */
static uint32_t fixmb_ptr2idx(FixmbStruct* fixmb, void* ptr)
{
	return (uint32_t)(((uint8_t*)ptr - (uint8_t*)(fixmb->mb_area)) / fixmb->mb_size);
}

int _kernel_fixmb_create(FixmbStruct* fixmb, uint32_t mb_size, uint32_t length)
{
	link_clear(&fixmb->link);
	if ( mb_size < sizeof(FixmbList) ) {
		mb_size = sizeof(FixmbList);
	}
	fixmb->mb_size = SIZE_POST_ALIGN_BY(mb_size, NORMAL_ALIGN);
	fixmb->mb_length = length;
	fixmb->mb_area = __sys_malloc_align(fixmb->mb_size * length, NORMAL_ALIGN); /* TBD: このメモリはユーザー空間の必要がある */

	if ( fixmb->mb_area ) {
		uint32_t bitmap_num;
		fixmb->is_list_mode = false;
		fixmb->list.top.index = fixmb->list.last.index = NO_ENTRY;
		fixmb->mb_array_index = 0;
		fixmb->use_count = 0;

		/* 使用中ビットマップをクリア */
		bitmap_num = (length + 31) / 32;
		fixmb->use_bitmap = __sys_malloc_align(bitmap_num * 4, 4);
		if ( fixmb->use_bitmap ) {
			uint32_t ix;
			for ( ix=0; ix < bitmap_num; ix++ ) {
				fixmb->use_bitmap[ix] = 0;
			}
		}
		else {
			goto ERR_RET1;
		}
	}
	else {
		goto ERR_RET2;
	}
	return RT_OK;

ERR_RET1:
	__sys_free(fixmb->mb_area);
ERR_RET2:
	return RT_ERR;
}


int _kernel_fixmb_trequest(FixmbStruct* fixmb, void** ptr, TimeOut tmout)
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
			alloc_index = fixmb->list.top.index;
		}
		*ptr = fixmb_idx2ptr(fixmb, alloc_index);
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
		FixmbInfoStruct fixmb_info;
		fixmb_info.obj = fixmb;
		fixmb_info.ptr = ptr;
		task_set_wait(_ctask, (void*)(&fixmb_info), 0);
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

int _kernel_fixmb_request(FixmbStruct* fixmb, void** ptr)
{
	return _kernel_fixmb_trequest(fixmb, ptr, TMO_FEVER);
}

int _kernel_fixmb_release(FixmbStruct* fixmb, void* ptr)
{
	uint32_t area_size;
	uint32_t		cpsr;
	int				ret = RT_OK;
	irq_save(cpsr);

	/* 解放するアドレスの妥当性チェック */
	uint32_t rel_index = fixmb_ptr2idx(fixmb, ptr);
	if ( (fixmb->mb_length <= rel_index) || !check_bitmap(fixmb->use_bitmap, rel_index) ) {
		/* 範囲外または使用中ではないのでエラー */
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
				FixmbList* rel_entry = fixmb_idx2ptr(fixmb, rel_index);
				FixmbList* last_entry = fixmb_idx2ptr(fixmb, fixmb->list.last.index);
				last_entry->index = rel_index;
				fixmb->list.last.index = rel_index;
			}
		}
		else {
			/* 解放待ちタスクがあるのでwakeup */
			/* 開放するMBをそのままwakeupするタスクに渡す */
			FixmbInfoStruct* fixmb_info;
			Link* link = fixmb->link.next;
			link_remove(link);
			TaskStruct* task = containerof(link, TaskStruct, link);
			fixmb_info = (FixmbInfoStruct*)(task->wait_obj);
			*(fixmb_info->ptr) = ptr;
			task_wakeup_stub(task, RT_OK);
			schedule();
		}
	}

	irq_restore(cpsr);
	return ret;
}

OSAPISTUB int __fixmb_create(uint32_t mb_size, uint32_t length)
{
	int ret = RT_ERR;
	int fixmb_id = alloc_fixmb_id();
	if ( 0 <= fixmb_id ) {
		FixmbStruct* fixmb = fixmbid2object(fixmb_id);
		ret = _kernel_fixmb_create(fixmb, mb_size, length);
		if ( ret == RT_OK ) {
			ret = fixmb_id;
		}
		else {
			free_fixmb_struct(fixmb_id);
		}
	}
	return ret;
}

OSAPISTUB int __fixmb_request(int id, void** ptr)
{
	FixmbStruct* fixmb = fixmbid2object(id);
	return _kernel_fixmb_request(fixmb, ptr);
}

OSAPISTUB int __fixmb_trequest(int id, void** ptr, TimeOut tmout)
{
	FixmbStruct* fixmb = fixmbid2object(id);
	return _kernel_fixmb_trequest(fixmb, ptr, tmout);
}

OSAPISTUB int __fixmb_release(int id, void* ptr)
{
	FixmbStruct* fixmb = fixmbid2object(id);
	return _kernel_fixmb_release(fixmb, ptr);
}
