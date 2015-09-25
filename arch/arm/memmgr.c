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
#include "my_board.h"
#include "memmgr.h"

#define	SHARE_SECT			(0x01u<<16)
#define	SHARE_PAGE			(0x01u<<10)

static const uint32_t	attr_conv_tbl[][2] = {
	[ATTR_TEXT][ATTRL_SECT] = 0x00000002|S_AP(AP_TEXT)|S_TEX(TEX_TEXT)|SHARE_SECT,
	[ATTR_TEXT][ATTRL_PAGE] = 0x00000002|P_AP(AP_TEXT)|P_TEX(TEX_TEXT)|SHARE_PAGE,
	[ATTR_DATA][ATTRL_SECT] = 0x00000012|S_AP(AP_DATA)|S_TEX(TEX_DATA)|SHARE_SECT,
	[ATTR_DATA][ATTRL_PAGE] = 0x00000003|P_AP(AP_DATA)|P_TEX(TEX_DATA)|SHARE_PAGE,
	[ATTR_DEV][ATTRL_SECT]  = 0x00000012|S_AP(AP_DEV)|S_TEX(TEX_DEV)|SHARE_SECT,
	[ATTR_DEV][ATTRL_PAGE]  = 0x00000003|P_AP(AP_DEV)|P_TEX(TEX_DEV)|SHARE_PAGE,
};
/* section table */
//static uint32_t section_tbl[4096] __attribute__((aligned(16*1024)));
static uint32_t* section_tbl;

extern void* heap_start_addr;


static bool mmgr_is_section(uint32_t entry)
{
	bool ret = false;
	if ( (entry & 0x00000003) == 0x00000001 ) {
		ret = true;
	}
	return ret;
}

/* セクションテーブルの割り当て */
static uint32_t* sectiontbl_alloc(void)
{
	/* 16Kバイトアラインメント */
	void* tbl = st_malloc_align(SECT_TABLE_SIZE, SECT_TABLE_SIZE);
	memset(tbl, 0, SECT_TABLE_SIZE);
	return (uint32_t*)tbl;
}

/* ページテーブルの割り当て */
static uint32_t* tbl_alloc(void)
{
	/* 1Kバイトアラインメント */
	void* tbl = st_malloc_align(PAGE_TABLE_SIZE, PAGE_TABLE_SIZE);
	memset(tbl, 0, PAGE_TABLE_SIZE);
	return (uint32_t*)tbl;
}

void mmgr_add_entry(void* addr, MemSize_t size, uint32_t attr)
{
	uint32_t st_addr = (uint32_t)PRE_ALIGN_BY(addr, PAGE_SIZE);
	size = ((uint32_t)addr + size) - st_addr;
	size = POST_ALIGN_BY(size, PAGE_SIZE);
	while ( 0 < size ) {
		/* アドレスが1MBアライン/sectionが割り当てられていない/残りサイズ1MB以上ならsection割り当て */
		if ( ((st_addr & (SECT_SIZE-1)) == 0) && (section_tbl[ADDR2SECT(st_addr)] == 0) && (SECT_SIZE <= size) ) {
			section_tbl[ADDR2SECT(st_addr)] = SECT_ENTRY(st_addr, attr_conv_tbl[attr][ATTRL_SECT]);
			st_addr += SECT_SIZE;
			size -= SECT_SIZE;
		}
		else {
			/* ページエントリ割り当て */
			uint32_t* page_tbl;
			if ( section_tbl[ADDR2SECT(st_addr)] == 0 ) {
				page_tbl = tbl_alloc();
				section_tbl[ADDR2SECT(st_addr)] = PAGE_TABLE_ENTRY(page_tbl);
			}
			else {
				page_tbl = section_tbl[ADDR2SECT(st_addr)] & ~(PAGE_TABLE_SIZE-1);
			}
			/* ページエントリ設定 */
			if ( page_tbl[ADDR2PAGE(st_addr)] != 0 ) {
				tprintf("Warnning: multiple define addr=%08X ptbl=%08X index=%d\n", st_addr, page_tbl, ADDR2PAGE(st_addr));
			}
			page_tbl[ADDR2PAGE(st_addr)] = PAGE_ENTRY(st_addr, attr_conv_tbl[attr][ATTRL_PAGE]);
			st_addr += PAGE_SIZE;
			if ( size <= PAGE_SIZE ) {
				size = 0;
			}
			else {
				size -= PAGE_SIZE;
			}
		}
	}
}

void mmgr_mmu_init(void)
{
	/* MMU関連レジスタ初期化 */
	DACR_set(0xffffffff);
	TTBCR_set(0x00000000);
	TTBR0_set((uint32_t)section_tbl);
	TTBR1_set(0);

	/* いったん命令キャッシュをdisable */
	uint32_t ctrl = SCTLR_get();
	SCTLR_set(ctrl & ~((0x1<<12)|(0x1<<11)));
	__isb();

	/* キャッシュをすべてinvalidate */
	ICIALLUIS_set(0);
	BPIALLIS_set(0);

	/* TLBをinvalidate */
	TLBIALLIS_set(0);

	/* MMU/キャッシュenable */
	__dsb();
	SCTLR_set(ctrl | ((0x1<<12)|(0x1<<11)|(0x1<<2)|(0x1<<0)));
	__isb();
	tprintf("SCTLR = %08X\n", SCTLR_get());
}

void mmgr_init(uint32_t cpuid)
{
extern char __text_start;
extern char __text_end;
extern char __data_start;

	if ( cpuid == MASTER_CPU_ID ) {
		/* セクションテーブル初期化 */
		section_tbl = sectiontbl_alloc();

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

	mmgr_mmu_init();
}
