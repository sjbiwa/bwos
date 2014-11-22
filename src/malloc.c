/*
 * malloc.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */

#include <stdint.h>
#include "common.h"
#include "malloc.h"

#define	MB_SIGNATURE			(0xA55Au)
#define	MB_SPACE				(0x0001u)
#define	MB_USE					(0x0002u)

/* メモリブロック状態 */
typedef	struct {
	uint16_t	signature;		/* 管理ブロックシグネチャ */
	uint16_t	status;			/* 管理ブロック状態 */
} MBIdentify;

/* 空きブロック先頭構造 */
typedef	struct tagMBProlog {
	MBIdentify		identify;		/* メモリブロックID */
	uint32_t		mb_size;
	struct tagMBProlog link;
} MBSpaceProlog;

/* 空きブロック最終構造 */
typedef	struct {
	uint32_t		mb_size;
	MBIdentify		identify;		/* メモリブロックID */
} MBSpaceEpilog;

/* 使用ブロック先頭構造 */
typedef	struct {
	MBIdentify		identify;		/* メモリブロックID */
	uint32_t		mb_size;
} MBUsedProlog;

/* 使用ブロック最終構造 */
typedef	struct {
	MBIdentify		identify;		/* メモリブロックID */
} MBUseEpilog;


/* 空きメモリブロック先頭 */
static MBSpaceProlog*	mb_space_link = NULL;

void malloc_init(void* addr, uint32_t size)
{
	/* 最低限必要なメモリ容量のチェック */
	if ( (sizeof(MBIdentify)*2 + sizeof(MBSpaceProlog) + sizeof(MBSpaceEpilog)) < size ) {
		return;
	}

	/* メモリブロックの先頭と最終に「使用中」識別子を書き込んで */
	/* 空きブロックを挟み込む。これで空き領域の境界を確保 */
	((MBIdentify*)addr)->signature = ((MBIdentify*)((uint8_t*)addr+size))->signature = MB_SIGNATURE;
	((MBIdentify*)addr)->status = ((MBIdentify*)((uint8_t*)addr+size))->status = MB_USE;

	/* 空きメモリブロック初期化 */
	MBSpaceProlog* mb_prolog = (MBSpaceProlog*)((uint8_t*)addr + sizeof(MBIdentify));
	MBSpaceProlog* mb_epilog = (MBSpaceProlog*)((uint8_t*)addr + sizeof(MBIdentify));
	size -= sizeof(MBIdentify) * 2;

	mb_prolog->identify.signature = mb_epilog->identify.signature = MB_SIGNATURE;
	mb_prolog->identify.status = mb_epilog->identify.status = MB_SPACE;
	mb_prolog->mb_size = mb_epilog->mb_size = size;

	/* 空きメモリブロックをリンクリストの先頭に挿入 */
	mb_prolog->link = mb_space_link;
	mb_space_link = mb_prolog;
}

OSAPI void* malloc(uint32_t size)
{
	return NULL;
}

OSAPI void free(void* ptr)
{
	return;
}
