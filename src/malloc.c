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

void sys_malloc_init(void)
{
	link_clear(&mb_space_link);
}

void sys_malloc_add_block(void* addr, uint32_t size)
{
	/* 最低限必要なメモリ容量のチェック */
	if ( size < (sizeof(MBIdentify)*2 + sizeof(MBSpaceProlog) + sizeof(MBSpaceEpilog)) ) {
		return;
	}

	/* メモリブロックの先頭と最終に「使用中」識別子を書き込んで */
	/* 空きブロックを挟み込む。これで空き領域の境界を確保 */
	((MBIdentify*)addr)->signature = ((MBIdentify*)((uint8_t*)addr+size)-1)->signature = MB_SIGNATURE;
	((MBIdentify*)addr)->status = ((MBIdentify*)((uint8_t*)addr+size)-1)->status = MB_USE;

	/* 空きメモリブロック初期化 */
	size -= sizeof(MBIdentify) * 2; /* 前後の使用中識別子分だけ減らす */
	MBSpaceProlog* mb_prolog = (MBSpaceProlog*)((uint8_t*)addr + sizeof(MBIdentify));
	MBSpaceEpilog* mb_epilog = (MBSpaceEpilog*)((uint8_t*)mb_prolog + size) - 1;
	mb_prolog->identify.signature = mb_epilog->identify.signature = MB_SIGNATURE;
	mb_prolog->identify.status = mb_epilog->identify.status = MB_SPACE;
	mb_prolog->mb_size = mb_epilog->mb_size = size;

	/* 空きメモリブロックをリンクリストに挿入 */
	link_add_last(&mb_space_link, &(mb_prolog->link));
}

OSAPI void* sys_malloc(uint32_t size)
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
		MBUseEpilog* mb_use_epilog = (MBUseEpilog*)((uint8_t*)mb_space + size) - 1;
		mb_use_epilog->identify.signature = MB_SIGNATURE;
		mb_use_prolog->identify.status = mb_use_epilog->identify.status = MB_USE;
		mb_use_prolog->mb_size = size;

		ret = (void*)(((uint8_t*)mb_use_prolog) + sizeof(MBUseProlog));
	}
	return ret;
}

OSAPI void sys_free(void* ptr)
{
	/* シグネチャチェック */
	MBUseProlog* mb_use_prolog = (MBUseProlog*)ptr - 1;
	if ( (mb_use_prolog->identify.signature != MB_SIGNATURE) || (mb_use_prolog->identify.status != MB_USE) ) {
		goto err_ret;
	}
	MBUseEpilog* mb_use_epilog = (MBUseEpilog*)((uint8_t*)mb_use_prolog + mb_use_prolog->mb_size) - 1;
	if ( (mb_use_epilog->identify.signature != MB_SIGNATURE) || (mb_use_epilog->identify.status != MB_USE) ) {
		goto err_ret;
	}

	MBIdentify* mb_front_id = (MBIdentify*)mb_use_prolog - 1;
	MBIdentify* mb_back_id = (MBIdentify*)(mb_use_epilog + 1);
	/* 対象ブロック前後のブロックのシグネチャチェック */
	if ( (mb_front_id->signature != MB_SIGNATURE) || (mb_back_id->signature != MB_SIGNATURE) ) {
		goto err_ret;
	}

	MBSpaceProlog* mb_space_prolog = NULL;
	MBSpaceEpilog* mb_space_epilog = NULL;
	/* 対象ブロックの前方ブロックチェック */
	if ( mb_front_id->status == MB_SPACE ) {
		/* 前方ブロックは空きブロックなので対象ブロックと連結する */
		mb_space_epilog = (MBSpaceEpilog*)(mb_front_id + 1) - 1; /* 前方ブロックのepilog */
		mb_space_prolog = (MBSpaceProlog*)((uint8_t*)mb_use_prolog - mb_space_epilog->mb_size);
		mb_space_epilog = (MBSpaceEpilog*)mb_back_id - 1; /* 対象ブロックのepilog(新しいepilog) */
		mb_space_prolog->identify.status = mb_space_epilog->identify.status = MB_SPACE;
		mb_space_prolog->mb_size += mb_use_prolog->mb_size;
		mb_space_epilog->mb_size = mb_space_prolog->mb_size;
	}
	else {
		/* 前方ブロックは使用中なので対象ブロックの空きブロック先頭にする */
		/* signatureとmb_sizeは変わらないので書き換えない */
		mb_space_prolog = (MBSpaceProlog*)mb_use_prolog;
		mb_space_epilog = (MBSpaceEpilog*)((uint8_t*)mb_space_prolog + mb_space_prolog->mb_size) - 1;
		mb_space_prolog->identify.status = mb_space_epilog->identify.status = MB_SPACE;
		mb_space_epilog->mb_size = mb_space_prolog->mb_size;
		/* 空きリンクリストに追加 */
		link_add_last(&mb_space_link, &(mb_space_prolog->link));
	}

	/* 対象ブロックの後方ブロックチェック */
	if ( mb_back_id->status == MB_SPACE ) {
		/* 後方ブロックが空きブロックならブロックを連結する */
		MBSpaceProlog* mb_back_prolog = (MBSpaceProlog*)mb_back_id;
		MBSpaceEpilog* mb_back_epilog = (MBSpaceEpilog*)((uint8_t*)mb_back_prolog + mb_back_prolog->mb_size) - 1;
		mb_space_prolog->mb_size += mb_back_prolog->mb_size;
		mb_back_epilog->mb_size = mb_space_prolog->mb_size;
		/* 後方ブロックは対象ブロックに連結するので空きリンクリストから外す */
		link_remove(&(mb_back_prolog->link));
	}
	else {
		/* 後方ブロックが使用中なら処理は無し */
	}

err_ret:
	return;
}

void dump_space()
{
	printf("DUMP_SPACE\n");
	Link* link = mb_space_link.next;
	while ( link != &mb_space_link ) {
		MBSpaceProlog* mb_prolog = containerof(link, MBSpaceProlog, link);
		MBSpaceEpilog* mb_epilog = (MBSpaceEpilog*)((uint8_t*)mb_prolog + mb_prolog->mb_size) - 1;
		if ( (mb_prolog->identify.signature != MB_SIGNATURE) ||  (mb_epilog->identify.signature != MB_SIGNATURE) ||
				(mb_prolog->identify.status != MB_SPACE) || (mb_epilog->identify.status != MB_SPACE) ||
				(mb_prolog->mb_size != mb_epilog->mb_size) ) {
			printf("error\n");
			break;
		}
		printf("SPACE:%08X-%08X (len=%08X)\n", mb_prolog, mb_epilog+1, mb_prolog->mb_size);
		link =link->next;
	}
}

void dump_use(void* ptr)
{
	printf("DUMP_USE\n");
	MBUseProlog* mb_prolog = (MBUseProlog*)ptr - 1;
	MBUseEpilog* mb_epilog = (MBUseEpilog*)((uint8_t*)(mb_prolog) + mb_prolog->mb_size) - 1;
	if ( (mb_prolog->identify.signature != MB_SIGNATURE) ||  (mb_epilog->identify.signature != MB_SIGNATURE) ||
			(mb_prolog->identify.status != MB_USE) || (mb_epilog->identify.status != MB_USE) ) {
		printf("error\n");
	}
	printf("USE:%08X (len=%08X)\n", mb_prolog, mb_prolog->mb_size);
}
