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

extern void mmgr_add_entry(void* addr, MemSize_t size, uint32_t attr);
extern void board_mmgr_init(void);
