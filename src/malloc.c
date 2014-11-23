/*
 * malloc.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */

#include <stdint.h>
#include "common.h"
#include "malloc.h"
#include "link.h"

#define	MB_SIGNATURE			(0xA55Au)
#define	MB_SPACE				(0x0001u)
#define	MB_USE					(0x0002u)

#define	MB_SPACE_INFO_SIZE		(sizeof(MBSpaceProlog)+sizeof(MBSpaceEpilog))
#define	MB_USE_INFO_SIZE		(sizeof(MBUseProlog)+sizeof(MBUseEpilog))

/* メモリブロック状態 */
typedef	struct {
	uint16_t	signature;		/* 管理ブロックシグネチャ */
	uint16_t	status;			/* 管理ブロック状態 */
} MBIdentify;

/* 空きブロック先頭構造 */
typedef	struct {
	MBIdentify		identify;		/* メモリブロックID */
	uint32_t		mb_size;
	Link			link;
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
} MBUseProlog;

/* 使用ブロック最終構造 */
typedef	struct {
	MBIdentify		identify;		/* メモリブロックID */
} MBUseEpilog;


/* 空きメモリブロック先頭 */
static 	Link	mb_space_link;

void malloc_init(void)
{
	link_clear(&mb_space_link);
}

void malloc_add_block(void* addr, uint32_t size)
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

	/* 空きメモリブロックをリンクリストに挿入 */
	link_add_last(&mb_space_link, &(mb_prolog->link));
}

OSAPI void* malloc(uint32_t size)
{
	void* ret = NULL;
	Link* find_link = NULL;
	MBSpaceProlog* mb_space = NULL;

	/* サイズを4バイトアラインに適用し、管理領域サイズを足しておく */
	size = ((size + 3) & ~0x00000003u) + MB_USE_INFO_SIZE;

	/* 指定サイズ以上の空きブロックを探す */
	find_link = mb_space_link.next;
	while ( find_link != &mb_space_link ) {
		MBSpaceProlog* mb_temp = containerof(find_link, MBSpaceProlog, link);
		if ( size <= mb_temp->mb_size ) {
			mb_space = mb_temp;
			break;
		}
		find_link = find_link->next;
	}
	if ( mb_space ) {
		/* 対象空きブロックをリンクリストから外す */
		link_remove(&(mb_space->link));

		/* 確保後の残りサイズチェック(余りが４以下ならすべてを割り当てる) */
		uint32_t remain_size = mb_space->mb_size - size;
		if ( MB_SPACE_INFO_SIZE <= remain_size ) {
			/* 残りブロックを新たに空きブロックとする */
			MBSpaceProlog* mb_remain_prolog = (MBSpaceProlog*)(((uint8_t*)mb_space) + size);
			MBSpaceEpilog* mb_remain_epilog = (MBSpaceEpilog*)(((uint8_t*)mb_remain_prolog) + (remain_size - sizeof(MBSpaceEpilog)));
			mb_remain_prolog->identify.signature = MB_SIGNATURE;
			mb_remain_prolog->identify.status = MB_SPACE;
			mb_remain_prolog->mb_size = mb_remain_epilog->mb_size = remain_size;
			link_add_last(&mb_space_link, &(mb_remain_prolog->link));
		}

		/* 確保した領域の初期化 */
		MBUseProlog* mb_use_prolog = (MBUseProlog*)mb_space;
		MBUseEpilog* mb_use_epilog = (MBUseEpilog*)(((uint8_t*)mb_space) + size - sizeof(MBUseEpilog));
		mb_use_epilog->identify.signature = MB_SIGNATURE;
		mb_use_prolog->identify.status = mb_use_epilog->identify.status = MB_USE;
		mb_use_prolog->mb_size = size;

		ret = (void*)(((uint8_t*)mb_use_prolog) + sizeof(MBUseProlog));
	}
	return ret;
}

OSAPI void free(void* ptr)
{
	return;
}
