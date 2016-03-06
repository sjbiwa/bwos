/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * arch_ptbl_lpae.c
 *   support ARMv7-A LPAE(LargePhysicalAddressExtension)
 *
 *  Created on: 2015/10/07
 *      Author: biwa
 */

#include "kernel.h"
#include "arm.h"
#include "mpcore.h"
#include "cp15reg.h"
#include "memmgr.h"


/*****************************************************************************/
/*
 * ページテーブルの抽象化
 *
 * アドレスの定義
 *       ------------------------------------------------------------------------------------
 *  PTR  |  セクションテーブルインデックス  | ページテーブルインデックス | ページオフセット |
 *       ------------------------------------------------------------------------------------
 *         or
 *       ------------------------------------------------------------------------------------
 *  PTR  |  セクションテーブルインデックス  |              ページオフセット                 |
 *       ------------------------------------------------------------------------------------
 *
 * 変換テーブル
 *         セクションテーブル
 *  TOP  ->-------------------            セクション領域
 *         |                 |--------> -------------------
 *         -------------------          |                 |
 *         |                 |          |  TEXT/DATA etc  |
 *         -------------------          |                 |
 *         |                 |          |                 |
 *         -------------------          -------------------
 *         |                 |
 *         -------------------            ページテーブル
 *         |                 | -------> -------------------              ページ領域
 *         -------------------          |                 | --------> -------------------
 *         |                 |          -------------------           |                 |
 *         -------------------          |                 |           | TEXT/DATA etc   |
 *         |                 |          -------------------           |                 |
 *         -------------------          |                 |           |                 |
 *         |                 |          -------------------           -------------------
 *         -------------------          |                 |
 *         |                 |          -------------------
 *         -------------------          |                 |
 *                                      -------------------
 *                                      |                 |
 *                                      -------------------
 *  ※セクションテーブルサイズ = セクションテーブルインデックス数 * セクションテーブルエントリサイズ
 *  ※ページテーブルサイズ = ページテーブルインデックス数 * ページテーブルエントリサイズ
 *
 *
 * 実装検討
 * 	・TOPより手前の部分はアーキテクチャー依存
 * 	・セクションテーブルは1個なので静的に確保あるいは起動時に確保する(全領域を起動時点で確保しておく)
 * 	・ページテーブルは複数個になり得るので、１つ毎に動的に確保する。
 * 	・各テーブル生成にあたり以下の項目がアーキテクチャー依存と考える
 * 	　- セクションテーブルインデックス/ページテーブルインデックスのビット数
 * 	  - セクションテーブル/ページテーブルのベースアドレスアラインメント
 * 	  - セクション/ページエントリの型(バイトサイズ)
 * 	  - セクション/ページエントリの生成方法
 */

#define	MAIR_CACHE_INDEX		(0uLL)
#define	MAIR_DEVICE_INDEX		(1uLL)
#define	MAIR_NOCACHE_INDEX		(2uLL)
#define	MAIR_STRONGLY_INDEX		(3uLL)
#define	MAIR_CACHE_VALUE		(0xffu)
#define	MAIR_DEVICE_VALUE		(0x04u)
#define	MAIR_NOCACHE_VALUE		(0x44u)
#define	MAIR_STRONGLY_VALUE		(0x00u)
#define	MAIR_SET_VALUE			(\
								(MAIR_CACHE_VALUE<<(MAIR_CACHE_INDEX*8)) | \
								(MAIR_DEVICE_VALUE<<(MAIR_DEVICE_INDEX*8)) | \
								(MAIR_NOCACHE_VALUE<<(MAIR_NOCACHE_INDEX*8)) | \
								(MAIR_STRONGLY_VALUE<<(MAIR_STRONGLY_INDEX*8)) | \
								0)

#define	EATTR_XN				(0x3uLL<<53)
#define	ETAAR_SHARE				(0x3uLL<<8)
#define	EATTR_RONLY				(0x3uLL<<6)
#define	EATTR_RW				(0x1uLL<<6)
#define	EATTR_CACHE				(MAIR_CACHE_INDEX<<2) /* ref MAIR0:0 */
#define	EATTR_DEVICE			(MAIR_DEVICE_INDEX<<2) /* ref MAIR0:1 */
#define	EATTR_NOCACHE			(MAIR_NOCACHE_INDEX<<2) /* ref MAIR0:2 */
#define	EATTR_STRONGLY			(MAIR_STRONGLY_INDEX<<2) /* ref MAIR0:3 */

static const uint64_t	attr_conv_tbl[ATTRL_E_NUM][ATTR_E_NUM] = {
		[ATTRL_SECT][ATTR_TEXT]     = 0x0000000000000401LL |EATTR_RONLY|ETAAR_SHARE|EATTR_CACHE,
		[ATTRL_SECT][ATTR_DATA]     = 0x0000000000000401LL |EATTR_XN|EATTR_RW|ETAAR_SHARE|EATTR_CACHE,
		[ATTRL_SECT][ATTR_DEV]      = 0x0000000000000401LL |EATTR_XN|EATTR_RW|ETAAR_SHARE|EATTR_DEVICE,
		[ATTRL_SECT][ATTR_NONCACHE] = 0x0000000000000401LL |EATTR_XN|EATTR_RW|ETAAR_SHARE|EATTR_NOCACHE,
		[ATTRL_PAGE][ATTR_TEXT]     = 0x0000000000000403LL |EATTR_RONLY|ETAAR_SHARE|EATTR_CACHE,
		[ATTRL_PAGE][ATTR_DATA]     = 0x0000000000000403LL |EATTR_XN|EATTR_RW|ETAAR_SHARE|EATTR_CACHE,
		[ATTRL_PAGE][ATTR_DEV]      = 0x0000000000000403LL |EATTR_XN|EATTR_RW|ETAAR_SHARE|EATTR_DEVICE,
		[ATTRL_PAGE][ATTR_NONCACHE] = 0x0000000000000403LL |EATTR_XN|EATTR_RW|ETAAR_SHARE|EATTR_NOCACHE,
};

/* アーキテクチャー依存部 */
#define	TOP_INDEX_BITS					(2)
#define	MAKE_SECT_TABLE_ENTRY			MAKE_PAGE_TABLE_ENTRY

typedef	uint64_t						SectEntry_t;
typedef	uint64_t						PageEntry_t;
#define	SECT_TABLE_ALIGN				(0x1000u)
#define	PAGE_TABLE_ALIGN				(0x1000u)
/* テーブルエントリ生成マクロ  */
#define	MAKE_PAGE_TABLE_ENTRY(addr)		((SectEntry_t)addr | 0x03)		/* ページテーブルを指すエントリ */
#define	MAKE_SECT_ENTRY(addr,attr)		((SectEntry_t)(addr) | (attr_conv_tbl[ATTRL_SECT][attr]))	/* セクションを指すエントリ */
#define	MAKE_PAGE_ENTRY(addr,attr)		((PageEntry_t)(addr) | (attr_conv_tbl[ATTRL_PAGE][attr]))	/* ページを指すエントリ */
/* エントリ値からアドレスを取得 */
#define	SECTENTRY2SECTADDR(entry)		/* not use */
#define	SECTENTRY2TABLEADDR(entry)		((void*)((entry) & 0x00000000fffff000LL))
#define	PAGEENTRY2PAGEADDR(entry)		/* not use */

volatile uint64_t			top_tbl[(0x1u<<TOP_INDEX_BITS)] __attribute__((aligned(32)));
/*****************************/


/* アーキテクチャー非依存部 */
#define	ADDR_INDEX_BITS					(32)
#define	SECT_INDEX_BITS					(11)
#define	PAGE_INDEX_BITS					(9)
#define	SECT_AREA_SIZE					(0x01u << (ADDR_INDEX_BITS-SECT_INDEX_BITS))
#define	PAGE_AREA_SIZE					(0x01u << (ADDR_INDEX_BITS-SECT_INDEX_BITS-PAGE_INDEX_BITS))
#define	SECT_TABLE_SIZE					((0x01u << SECT_INDEX_BITS) * sizeof(SectEntry_t))
#define	PAGE_TABLE_SIZE					((0x01u << PAGE_INDEX_BITS) * sizeof(PageEntry_t))

/* アドレスからセクションテーブル/ページテーブルのエントリインデックスを取得 */
#define	ADDR2SECTINDEX(addr)			(((PtrInt_t)(addr)) >> (ADDR_INDEX_BITS-SECT_INDEX_BITS))
#define	ADDR2PAGEINDEX(addr)			((((PtrInt_t)(addr)) >> (ADDR_INDEX_BITS-SECT_INDEX_BITS-PAGE_INDEX_BITS)) & (((PtrInt_t)1u << PAGE_INDEX_BITS) - 1))

volatile SectEntry_t*		section_tbl;
/*****************************/

/* セクションテーブルの割り当て */
static SectEntry_t* secttbl_alloc(void)
{
	/* 16Kバイトアラインメント */
	SectEntry_t* tbl = (SectEntry_t*)__st_malloc_align(SECT_TABLE_SIZE, SECT_TABLE_ALIGN);
	memset(tbl, 0, SECT_TABLE_SIZE);
	return tbl;
}

/* ページテーブルの割り当て */
static PageEntry_t* pagetbl_alloc(void)
{
	/* 1Kバイトアラインメント */
	PageEntry_t* tbl = (PageEntry_t*)__st_malloc_align(PAGE_TABLE_SIZE, PAGE_TABLE_ALIGN);
	memset(tbl, 0, PAGE_TABLE_SIZE);
	return tbl;
}

static void mmgr_mmu_init(void)
{
	/* MMU関連レジスタ初期化 */
	MAIR0_set(MAIR_SET_VALUE);
	MAIR1_set(0);
	TTBCR_set(0x80000000);
	TTBRX0_set((uint64_t)top_tbl);
	TTBRX1_set((uint64_t)top_tbl);

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
	section_tbl = secttbl_alloc();

	/* firstテーブル初期化(ここは未だアーキテクチャー依存) */
	for ( int ix = 0; ix < (0x1u<<TOP_INDEX_BITS); ix++ ) {
		top_tbl[ix] = MAKE_SECT_TABLE_ENTRY(&section_tbl[(0x1u<<SECT_INDEX_BITS-TOP_INDEX_BITS) * ix]);
	}
}

void arch_ptbl_enable(void)
{
	mmgr_mmu_init();
}

void mmgr_add_entry(void* addr, MemSize_t size, uint32_t attr)
{
	PtrInt_t st_addr = (PtrInt_t)PRE_ALIGN_BY(addr, PAGE_AREA_SIZE);
	size = ((PtrInt_t)addr + size) - st_addr;
	size = POST_ALIGN_BY(size, PAGE_AREA_SIZE);
	while ( 0 < size ) {
		//tprintf("ADDR=%08X\n", st_addr);
		/* アドレスがsectionアライン/sectionが割り当てられていない/残りサイズ1MB以上ならsection割り当て */
		if ( ((st_addr & (SECT_AREA_SIZE-1)) == 0) && (section_tbl[ADDR2SECTINDEX(st_addr)] == 0) && (SECT_AREA_SIZE <= size) ) {
			section_tbl[ADDR2SECTINDEX(st_addr)] = MAKE_SECT_ENTRY(st_addr, attr);
			//tprintf("S:SECT:%d:%08X%08X\n", ADDR2SECTINDEX(st_addr), (uint32_t)(section_tbl[ADDR2SECTINDEX(st_addr)] >> 32), (uint32_t)(section_tbl[ADDR2SECTINDEX(st_addr)]));
			st_addr += SECT_AREA_SIZE;
			size -= SECT_AREA_SIZE;
		}
		else {
			/* ページエントリ割り当て */
			PageEntry_t* page_tbl;
			if ( section_tbl[ADDR2SECTINDEX(st_addr)] == 0 ) {
				page_tbl = pagetbl_alloc();
				section_tbl[ADDR2SECTINDEX(st_addr)] = MAKE_PAGE_TABLE_ENTRY(page_tbl);
				//tprintf("P:SECT:%d:%08X%08X\n", ADDR2SECTINDEX(st_addr), (uint32_t)(section_tbl[ADDR2SECTINDEX(st_addr)] >> 32), (uint32_t)(section_tbl[ADDR2SECTINDEX(st_addr)]));
			}
			else {
				page_tbl = SECTENTRY2TABLEADDR(section_tbl[ADDR2SECTINDEX(st_addr)]);
			}
			/* ページエントリ設定 */
			if ( page_tbl[ADDR2PAGEINDEX(st_addr)] != 0 ) {
				tprintf("Warnning: multiple define addr=%08X ptbl=%08X index=%d\n", st_addr, page_tbl, ADDR2PAGEINDEX(st_addr));
			}
			page_tbl[ADDR2PAGEINDEX(st_addr)] = MAKE_PAGE_ENTRY(st_addr, attr);
			//tprintf("P:PAGE:%d:%08X%08X\n", ADDR2PAGEINDEX(st_addr), (uint32_t)(page_tbl[ADDR2PAGEINDEX(st_addr)] >> 32), (uint32_t)(page_tbl[ADDR2PAGEINDEX(st_addr)]));
			st_addr += PAGE_AREA_SIZE;
			if ( size <= PAGE_AREA_SIZE ) {
				size = 0;
			}
			else {
				size -= PAGE_AREA_SIZE;
			}
		}
	}
}
