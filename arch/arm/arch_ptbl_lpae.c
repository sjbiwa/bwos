/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * arch_ptbl_lpae.c
 *   support ARMv7-A LargePhysicalAddressExtension
 *
 *  Created on: 2015/10/07
 *      Author: biwa
 */

#include "kernel.h"
#include "arm.h"
#include "mpcore.h"
#include "cp15reg.h"
#include "my_board.h"
#include "memmgr.h"

#define	FIRST_TBL_NUM			(4)
#define	SECOND_TABLE_SIZE		(0x1000u)
#define	THIRD_TABLE_SIZE		(0x1000u)
#define	SECOND_TABLE_NUM		(SECOND_TABLE_SIZE/sizeof(uint64_t))
#define	THIRD_TABLE_NUM			(THIRD_TABLE_SIZE/sizeof(uint64_t))

#define	SHARE_SECT				(0x01u<<16)
#define	SHARE_PAGE				(0x01u<<10)

#define	SECT_SIZE				(0x100000u)	/* １セクションのサイズ (1MB) */
#define	PAGE_SIZE				(0x1000u)	/* １ページのサイズ (4KB) */
#define	SECT_TABLE_SIZE			(4096*4)	/* セクションテーブルのサイズ (4096エントリ) */
#define	PAGE_TABLE_SIZE			(1024)		/* ページテーブルのサイズ (256エントリ/1KB) */

/* アドレスからセクションテーブル/ページテーブルのエントリインデックスを取得 */
#define	ADDR2SECT(addr)			(((uint32_t)(addr)) >> 20)
#define	ADDR2PAGE(addr)			((((uint32_t)(addr)) >> 12) & 0xff)

/* テーブルエントリ生成マクロ */
#define	PAGE_TABLE_ENTRY(addr)	((uint32_t)addr | 0x01)		/* ページテーブルを指すエントリ */
#define	SECT_ENTRY(addr,attr)	((uint32_t)(addr) | (attr))	/* セクションを指すエントリ */
#define	PAGE_ENTRY(addr,attr)	((uint32_t)(addr) | (attr))	/* ページを指すエントリ */

/* ページ属性を抽象化レベルから実際レベルに変換するテーブル */
#define	S_AP(v)					((((v)&0x4)<<13)|(((v)&0x3)<<10))	/* 2-0 -> 15:11:10 */
#define	S_TEX(v)				((((v)&0x1C)<<10)|(((v)&0x3)<<2))	/* 4-0 -> 14:13:12 3:2(C/B) */
#define	P_AP(v)					((((v)&0x4)<<7)|(((v)&0x3)<<4))		/* 2-0 -> 9:5:4 */
#define	P_TEX(v)				((((v)&0x1C)<<4)|(((v)&0x3)<<2))	/* 4-0 -> 8:7:6 3:2(C/B) */

#define	AP_TEXT					(0x3)
#define	AP_DATA					(0x3)
#define	AP_DEV					(0x3)
#define	TEX_TEXT				(0x07)
#define	TEX_DATA				(0x07)
#define	TEX_DEV					(0x01)

static const uint32_t	attr_conv_tbl[][2] = {
	[ATTR_TEXT][ATTRL_SECT] = 0x00000002|S_AP(AP_TEXT)|S_TEX(TEX_TEXT)|SHARE_SECT,
	[ATTR_TEXT][ATTRL_PAGE] = 0x00000002|P_AP(AP_TEXT)|P_TEX(TEX_TEXT)|SHARE_PAGE,
	[ATTR_DATA][ATTRL_SECT] = 0x00000012|S_AP(AP_DATA)|S_TEX(TEX_DATA)|SHARE_SECT,
	[ATTR_DATA][ATTRL_PAGE] = 0x00000003|P_AP(AP_DATA)|P_TEX(TEX_DATA)|SHARE_PAGE,
	[ATTR_DEV][ATTRL_SECT]  = 0x00000012|S_AP(AP_DEV)|S_TEX(TEX_DEV)|SHARE_SECT,
	[ATTR_DEV][ATTRL_PAGE]  = 0x00000003|P_AP(AP_DEV)|P_TEX(TEX_DEV)|SHARE_PAGE,
};
/* section table */

/* first-level PageTable */
static uint64_t		first_page_tbl[FIRST_TBL_NUM] __attribute__((aligned(32)));
static uint64_t*	second_page_tbl;


static bool mmgr_is_section(uint32_t entry)
{
	bool ret = false;
	if ( (entry & 0x00000003) == 0x00000001 ) {
		ret = true;
	}
	return ret;
}

/* secondテーブルの割り当て */
/*  全secondテーブルを割り当てる */
static uint64_t* second_tbl_alloc(void)
{
	/* 4Kバイトアラインメント */
	void* tbl = st_malloc_align(SECOND_TABLE_SIZE * FIRST_TBL_NUM, SECOND_TABLE_SIZE);
	memset(tbl, 0, SECOND_TABLE_SIZE * FIRST_TBL_NUM);
	return (uint64_t*)tbl;
}

/* thirdテーブルの割り当て */
static uint64_t* third_tbl_alloc(void)
{
	/* 4Kバイトアラインメント */
	void* tbl = st_malloc_align(THIRD_TABLE_SIZE, THIRD_TABLE_SIZE);
	memset(tbl, 0, PAGE_TABLE_SIZE);
	return (uint64_t*)tbl;
}

static void mmgr_mmu_init(void)
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

void arch_ptbl_init(void)
{
	/* second page table領域を確保 */
	uint64_t* ptbl = second_tbl_alloc();

	/* firstテーブル初期化 */
	for ( int ix = 0; ix < FIRST_TBL_NUM; ix++ ) {
		first_page_tbl[ix] = FIRST_ENTRY(&ptbl[SECOND_TABLE_NUM * ix]);
	}
}

void arch_ptbl_enable(void)
{
	mmgr_mmu_init();
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
