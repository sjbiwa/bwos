/*
 * memmgr.c
 *
 *  Created on: 2014/12/07
 *      Author: biwa
 */

#include "common.h"
#include "arm.h"
#include "cp15reg.h"
#include "my_board.h"

#define	SECT_SIZE				(0x100000u)	/* １セクションのサイズ (1MB) */
#define	PAGE_SIZE				(0x1000u)	/* １ページのサイズ (4KB) */
#define	PAGE_TABLE_SIZE			(1024)		/* ページテーブルのサイズ (256エントリ/1KB) */

#define	ATTR_TEXT				(0)
#define	ATTR_DATA				(1)
#define	ATTR_DEV				(2)

/* テーブルエントリ生成マクロ */
#define	INST_SECT_ENTRY(addr)		(0)
#define	INST_PAGE_ENTRY(addr)		(0)
#define	DEV_SECT_ENTRY(addr)		(0)
#define	DEV_PAGE_ENTRY(addr)		(0)
#define	NORM_SECT_ENTRY(addr)		(0)
#define	NORM_PAGE_ENTRY(addr)		(0)
#define	PAGE_TABLE_ENTRY(addr)		((uint32_t)addr | 0x33)
#define	SECT_ENTRY(addr)			((uint32_t)addr | 0x11)
#define	PAGE_ENTRY(addr,attr)		((uint32_t)addr | attr)


/* アドレスからセクションテーブル/ページテーブルのエントリインデックスを取得 */
#define	ADDR2SECT(addr)				(((uint32_t)(addr)) >> 20)
#define	ADDR2PAGE(addr)				((((uint32_t)(addr)) >> 12) & 0xff)

/* section table */
static uint32_t section_tbl[4096];

extern void* heap_start_addr;

/* ページテーブルの割り当て */
static uint32_t* tbl_alloc(void)
{
	/* 4Kバイトアラインメント */
	uint32_t tbl = POST_ALIGN_BY(heap_start_addr, PAGE_TABLE_SIZE);
	heap_start_addr = (void*)(tbl + PAGE_TABLE_SIZE);
	memset((void*)tbl, 0, PAGE_TABLE_SIZE);
	return (uint32_t*)tbl;
}

void mmgr_add_entry(void* addr, uint32_t size, uint32_t attr)
{
	uint32_t st_addr = PRE_ALIGN_BY(addr, PAGE_SIZE);
	size = ((uint32_t)addr + size) - st_addr;
	while ( 0 < size ) {
		/* アドレスが1MBアライン/sectionが割り当てられていない/残りサイズ1MB以上ならsection割り当て */
		if ( ((st_addr & (SECT_SIZE-1)) == 0) && (section_tbl[ADDR2SECT(st_addr)] == 0) && (SECT_SIZE <= size) ) {
			section_tbl[ADDR2SECT(st_addr)] = SECT_ENTRY(st_addr);
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
				lprintf("Warnning: multiple define addr=%08X ptbl=%08X index=%d\n", st_addr, page_tbl, ADDR2PAGE(st_addr));
			}
			page_tbl[ADDR2PAGE(st_addr)] = PAGE_ENTRY(st_addr, attr);
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
	/****************************************************/
	/*	MMUの設定方法									*/
	/*													*/
	/*	__text_start ～ __test_end		リードオンリ	*/
	/*	__data_start ～ END_MEM_ADDR	R/W				*/
	/*													*/
	/****************************************************/

	mmgr_add_entry((void*)(&__text_start), (uint32_t)(&__text_end)-(uint32_t)(&__text_start), ATTR_TEXT);
	mmgr_add_entry((void*)(&__data_start), END_MEM_ADDR - (uint32_t)(&__data_start), ATTR_DATA);
	mmgr_add_entry((void*)0xE0000000, 0x1000, ATTR_DEV);

#if 1
	for ( ix=0; ix < arrayof(section_tbl); ix++ ) {
		lprintf("sect[%d] = %08X\n", ix, section_tbl[ix]);
		if ( (section_tbl[ix] & 0xff) == 0x33 ) {
			uint32_t* page_tbl = (uint32_t*)(section_tbl[ix] & ~0x3ff);
			for ( iy=0; iy < 256; iy++ ) {
				lprintf("    page[%d] = %08X\n", iy, page_tbl[iy]);
			}
		}
	}
#endif
}
