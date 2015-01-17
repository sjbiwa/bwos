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

OSAPI int fixmb_create(FixmbStruct* fixmb, uint32_t mb_size, uint32_t length)
{
	link_clear(&fixmb->link);
	fixmb->mb_size = POST_ALIGN_BY(mb_size, 16); /* サイズは16バイトの倍数にする */
	fixmb->mb_length = length;
	fixmb->mblock = sys_malloc_align(fixmb->mb_size * length, 16);
	return 0;
}


OSAPI int fixmb_trequest(FixmbStruct* fixmb, void** ptr, TimeOut tmout)
{
	uint32_t cpsr;
	irq_save(cpsr);


	irq_restore(cpsr);
	return 0;
}

OSAPI int fixmb_request(FixmbStruct* fixmb, void** ptr)
{
	return fixmb_trequest(fixmb, ptr, TMO_FEVER);
}

OSAPI int fixmb_release(FixmbStruct* fixmb, void* ptr)
{
	return 0;
}
