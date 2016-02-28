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
#include "armv8.h"
#include "my_board.h"
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
		mmgr_add_entry((void*)(&__text_start), (uint64_t)(&__text_end)-(uint64_t)(&__text_start), ATTR_TEXT);
		/* DATA/BSS/HEAP領域 */
		mmgr_add_entry((void*)(&__data_start), (uint64_t)(END_MEM_ADDR+1) - (uint64_t)(&__data_start), ATTR_DATA);
		/* GIC */
		mmgr_add_entry((void*)(GIC_GICD_BASE), (uint64_t)(0x10000LL), ATTR_DEV);
		mmgr_add_entry((void*)(GIC_GICR_BASE), (uint64_t)(0x20000LL), ATTR_DEV);
		mmgr_add_entry((void*)(GIC_GICC_BASE), (uint64_t)(0x1000LL), ATTR_DEV);

		/* ボード固有領域 */
		board_mmgr_init();
	}

	/* ページテーブルの有効化(MMU ON) */
	arch_ptbl_enable();
}
