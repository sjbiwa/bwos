/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * arch_ptbl.c
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
//static uint32_t section_tbl[4096] __attribute__((aligned(16*1024)));
static uint32_t* section_tbl;


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
		/* セクションテーブル初期化 */
		section_tbl = sectiontbl_alloc();
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
