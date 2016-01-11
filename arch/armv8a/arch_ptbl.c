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
#include "armv8.h"
#include "my_board.h"
#include "memmgr.h"

/*****************************************************************************/
/*
 * ページテーブルの抽象化
 *
 * アドレスの定義
 *     42                                 29                           16                  0
 *       ------------------------------------------------------------------------------------
 *  PTR  |  セクションテーブルインデックス  | ページテーブルインデックス | ページオフセット |
 *       |              13bit               |            13bit           |       16bit      |
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
#define	MAIR_SET_VALUE			(0uLL | \
								(MAIR_CACHE_VALUE<<(MAIR_CACHE_INDEX*8)) | \
								(MAIR_DEVICE_VALUE<<(MAIR_DEVICE_INDEX*8)) | \
								(MAIR_NOCACHE_VALUE<<(MAIR_NOCACHE_INDEX*8)) | \
								(MAIR_STRONGLY_VALUE<<(MAIR_STRONGLY_INDEX*8)))
#define	EATTR_XN				(0x3uLL<<53)
#define	ETAAR_SHARE				(0x3uLL<<8)
#define	EATTR_RONLY				(0x3uLL<<6)
#define	EATTR_RW				(0x1uLL<<6)
#define	EATTR_CACHE				(MAIR_CACHE_INDEX<<2) /* ref MAIR0:0 */
#define	EATTR_DEVICE			(MAIR_DEVICE_INDEX<<2) /* ref MAIR0:1 */
#define	EATTR_NOCACHE			(MAIR_NOCACHE_INDEX<<2) /* ref MAIR0:2 */
#define	EATTR_STRONGLY			(MAIR_STRONGLY_INDEX<<2) /* ref MAIR0:3 */

static const uint64_t	attr_conv_tbl[2][3] = {
		[ATTRL_SECT][ATTR_TEXT] = 0x0000000000000401LL |EATTR_RONLY|ETAAR_SHARE|EATTR_CACHE,
		[ATTRL_SECT][ATTR_DATA] = 0x0000000000000401LL |EATTR_XN|EATTR_RW|ETAAR_SHARE|EATTR_CACHE,
		[ATTRL_SECT][ATTR_DEV]  = 0x0000000000000401LL |EATTR_XN|EATTR_RW|ETAAR_SHARE|EATTR_DEVICE,
		[ATTRL_PAGE][ATTR_TEXT] = 0x0000000000000403LL |EATTR_RONLY|ETAAR_SHARE|EATTR_CACHE,
		[ATTRL_PAGE][ATTR_DATA] = 0x0000000000000403LL |EATTR_XN|EATTR_RW|ETAAR_SHARE|EATTR_CACHE,
		[ATTRL_PAGE][ATTR_DEV]  = 0x0000000000000403LL |EATTR_XN|EATTR_RW|ETAAR_SHARE|EATTR_DEVICE,
};

/* アーキテクチャー依存部 */
#define	MAKE_SECT_TABLE_ENTRY			MAKE_PAGE_TABLE_ENTRY

typedef	uint64_t						SectEntry_t;
typedef	uint64_t						PageEntry_t;
#define	SECT_TABLE_ALIGN				(0x10000u)
#define	PAGE_TABLE_ALIGN				(0x10000u)
/* テーブルエントリ生成マクロ  */
#define	MAKE_PAGE_TABLE_ENTRY(addr)		((SectEntry_t)addr | 0x03)		/* ページテーブルを指すエントリ */
#define	MAKE_SECT_ENTRY(addr,attr)		((SectEntry_t)(addr) | (attr_conv_tbl[ATTRL_SECT][attr]))	/* セクションを指すエントリ */
#define	MAKE_PAGE_ENTRY(addr,attr)		((PageEntry_t)(addr) | (attr_conv_tbl[ATTRL_PAGE][attr]))	/* ページを指すエントリ */
/* エントリ値からアドレスを取得 */
#define	SECTENTRY2SECTADDR(entry)		/* not use */
#define	SECTENTRY2TABLEADDR(entry)		((void*)((entry) & 0x000003ffffff0000LL))
#define	PAGEENTRY2PAGEADDR(entry)		/* not use */

/*****************************/


/* アーキテクチャー非依存部 */
#define	ADDR_INDEX_BITS					(42)
#define	SECT_INDEX_BITS					(13)
#define	PAGE_INDEX_BITS					(13)
#define	SECT_AREA_SIZE					(0x01uLL << (ADDR_INDEX_BITS-SECT_INDEX_BITS))
#define	PAGE_AREA_SIZE					(0x01uLL << (ADDR_INDEX_BITS-SECT_INDEX_BITS-PAGE_INDEX_BITS))
#define	SECT_TABLE_SIZE					((0x01uLL << SECT_INDEX_BITS) * sizeof(SectEntry_t))
#define	PAGE_TABLE_SIZE					((0x01uLL << PAGE_INDEX_BITS) * sizeof(PageEntry_t))

/* アドレスからセクションテーブル/ページテーブルのエントリインデックスを取得 */
#define	ADDR2SECTINDEX(addr)			(((PtrInt_t)(addr)) >> (ADDR_INDEX_BITS-SECT_INDEX_BITS))
#define	ADDR2PAGEINDEX(addr)			((((PtrInt_t)(addr)) >> (ADDR_INDEX_BITS-SECT_INDEX_BITS-PAGE_INDEX_BITS)) & (((PtrInt_t)1u << PAGE_INDEX_BITS) - 1))

volatile SectEntry_t*		section_tbl;
/*****************************/

/* セクションテーブルの割り当て */
static SectEntry_t* secttbl_alloc(void)
{
	/* 16Kバイトアラインメント */
	SectEntry_t* tbl = (SectEntry_t*)st_malloc_align(SECT_TABLE_SIZE, SECT_TABLE_ALIGN);
	memset(tbl, 0, SECT_TABLE_SIZE);
	return tbl;
}

/* ページテーブルの割り当て */
static PageEntry_t* pagetbl_alloc(void)
{
	/* 1Kバイトアラインメント */
	PageEntry_t* tbl = (PageEntry_t*)st_malloc_align(PAGE_TABLE_SIZE, PAGE_TABLE_ALIGN);
	memset(tbl, 0, PAGE_TABLE_SIZE);
	return tbl;
}

static void mmgr_mmu_init(void)
{
	/* MMU関連レジスタ初期化 */
	MAIR_EL1_set(MAIR_SET_VALUE);
	TCR_EL1_set(0x00000005C0806516LL);
	TTBR0_EL1_set((uint64_t)section_tbl);
	TTBR1_EL1_set((uint64_t)0LL);

	/* いったん命令キャッシュをdisable */
	uint32_t ctrl = SCTLR_EL1_get();
	SCTLR_EL1_set(ctrl & ~(0x1<<12));
	__isb();

	/* キャッシュをすべてinvalidate */
	IC_IALLUIS_set();

	/* TLBをinvalidate */
	TLBI_VMALLE1_set();

	/* MMU/キャッシュenable */
	__dsb();
	SCTLR_EL1_set(ctrl | ((0x1<<12)|(0x1<<2)|(0x1<<1)|(0x1<<0)));
	__isb();
	tprintf("SCTLR = %08X\n", SCTLR_EL1_get());
}

void arch_ptbl_init(void)
{
	section_tbl = secttbl_alloc();
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
	int ix = 0;
	while ( 0 < size ) {
		//tprintf("ADDR=%08X%08X\n", hword(st_addr), lword(st_addr));
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
				//tprintf("Warnning: multiple define addr=%08X ptbl=%08X index=%d\n", st_addr, page_tbl, ADDR2PAGEINDEX(st_addr));
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
