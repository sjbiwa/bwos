/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * memmgr.c
 *
 *  Created on: 2014/12/07
 *      Author: biwa
 */

#include "kernel.h"
#include "arm.h"
#include "mpcore.h"
#include "cp15reg.h"
#include "memmgr.h"

extern void board_mmgr_init(void);

void mmgr_init(uint32_t cpuid)
{
extern char __text_start;
extern char __text_end;
extern char __data_start;

	if ( cpuid == MASTER_CPU_ID ) {
		/* ページテーブル初期化 */
		arch_ptbl_init();

		/****************************************************/
		/*	MMUの設定方法									*/
		/*													*/
		/*	__text_start ～ __test_end		リードオンリ	*/
		/*	__data_start ～ END_MEM_ADDR	R/W				*/
		/*													*/
		/****************************************************/

		/* TEXT領域 */
		mmgr_add_entry((void*)(&__text_start), (uint32_t)(&__text_end)-(uint32_t)(&__text_start), ATTR_TEXT);
		/* DATA/BSS/HEAP領域 */
		mmgr_add_entry((void*)(&__data_start), (END_MEM_ADDR+1) - (uint32_t)(&__data_start), ATTR_DATA);
		/* MPCORE領域 */
		mmgr_add_entry((void*)(MPCORE_SCU_BASE), 0x8000, ATTR_DEV);

		/* ボード固有領域 */
		board_mmgr_init();
	}

	/* ページテーブルの有効化(MMU ON) */
	arch_ptbl_enable();
}
