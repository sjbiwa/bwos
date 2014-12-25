/*
 * memmgr.c
 *
 *  Created on: 2014/12/07
 *      Author: biwa
 */

#include "common.h"
#include "arm.h"
#include "mpcore.h"
#include "cp15reg.h"
#include "my_board.h"

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
#define	S_AP(v)		((((v)&0x4)<<13)|(((v)&0x3)<<10))	/* 2-0 -> 15:11:10 */
#define	S_TEX(v)	((((v)&0x1C)<<10)|(((v)&0x3)<<2))	/* 4-0 -> 14:13:12 3:2(C/B) */
#define	P_AP(v)		((((v)&0x4)<<7)|(((v)&0x3)<<4))		/* 2-0 -> 9:5:4 */
#define	P_TEX(v)	((((v)&0x1C)<<4)|(((v)&0x3)<<2))	/* 4-0 -> 8:7:6 3:2(C/B) */

#define	AP_TEXT			(0x3)
#define	AP_DATA			(0x3)
#define	AP_DEV			(0x3)
#define	TEX_TEXT		(0x07)
#define	TEX_DATA		(0x07)
#define	TEX_DEV			(0x01)

enum {
	ATTRL_SECT = 0,
	ATTRL_PAGE,
};
enum {
	ATTR_TEXT = 0,
	ATTR_DATA,
	ATTR_DEV,
};
static const uint32_t	attr_conv_tbl[][2] = {
	[ATTR_TEXT][ATTRL_SECT] = 0x00000002|S_AP(AP_TEXT)|S_TEX(TEX_TEXT),
	[ATTR_TEXT][ATTRL_PAGE] = 0x00000002|P_AP(AP_TEXT)|P_TEX(TEX_TEXT),
	[ATTR_DATA][ATTRL_SECT] = 0x00000012|S_AP(AP_DATA)|S_TEX(TEX_DATA),
	[ATTR_DATA][ATTRL_PAGE] = 0x00000003|P_AP(AP_DATA)|P_TEX(TEX_DATA),
	[ATTR_DEV][ATTRL_SECT]  = 0x00000012|S_AP(AP_DEV)|S_TEX(TEX_DEV),
	[ATTR_DEV][ATTRL_PAGE]  = 0x00000003|P_AP(AP_DEV)|P_TEX(TEX_DEV),
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
	uint32_t tbl = POST_ALIGN_BY(heap_start_addr, SECT_TABLE_SIZE);
	heap_start_addr = (void*)(tbl + SECT_TABLE_SIZE);
	memset((void*)tbl, 0, SECT_TABLE_SIZE);
	return (uint32_t*)tbl;
}

/* ページテーブルの割り当て */
static uint32_t* tbl_alloc(void)
{
	/* 1Kバイトアラインメント */
	uint32_t tbl = POST_ALIGN_BY(heap_start_addr, PAGE_TABLE_SIZE);
	heap_start_addr = (void*)(tbl + PAGE_TABLE_SIZE);
	memset((void*)tbl, 0, PAGE_TABLE_SIZE);
	return (uint32_t*)tbl;
}

void mmgr_add_entry(void* addr, uint32_t size, uint32_t attr)
{
	uint32_t st_addr = PRE_ALIGN_BY(addr, PAGE_SIZE);
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


void mmgr_init(void)
{
extern char __text_start;
extern char __text_end;
extern char __data_start;
	int ix, iy;

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
	mmgr_add_entry((void*)(MPCORE_SCU_BASE), 0x2000, ATTR_DEV);
	/* UART領域 */
	mmgr_add_entry((void*)(0x10009000), 0x1000, ATTR_DEV);

	/* MMU関連レジスタ初期化 */
	DACR_set(0xffffffff);
	TTBCR_set(0x00000000);
	TTBR0_set((uint32_t)section_tbl);
	TTBR1_set(0);

	/* いったん命令キャッシュをdisable */
	uint32_t ctrl = SCTLR_get();
	SCTLR_set(ctrl & ~((0x1<<12)|(0x1<<11)));

	/* キャッシュをすべてinvalidate */
	ICIALLUIS_set(0);
	BPIALLIS_set(0);
	//DCISW_set(0);

	/* MMU/キャッシュenable */
	SCTLR_set(ctrl | ((0x1<<12)|(0x1<<11)|(0x1<<2)|(0x1<<0)));
	__isb();

	tprintf("SCTLR = %08X\n", SCTLR_get());
#if 0
	for ( ix=0; ix < arrayof(section_tbl); ix++ ) {
		tprintf("sect[%d] = %08X\n", ix, section_tbl[ix]);
		if ( mmgr_is_section(section_tbl[ix]) ) {
			uint32_t* page_tbl = (uint32_t*)(section_tbl[ix] & ~0x3ff);
			for ( iy=0; iy < 256; iy++ ) {
				tprintf("    page[%d] = %08X\n", iy, page_tbl[iy]);
			}
		}
	}
	for (;;);
#endif

}
