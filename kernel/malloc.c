/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * malloc.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */

#include <stdint.h>
#include "kernel.h"

#define	MB_ALIGN				(sizeof(PtrInt_t))	/* 空きブロック/使用中ブロックのアラインメント */

/* アラインメント演算用マクロ */
#define	PRE_ALIGN(x)			PRE_ALIGN_BY(x,MB_ALIGN)
#define	POST_ALIGN(x)			POST_ALIGN_BY(x,MB_ALIGN)

/* ポインタ演算用マクロ */
#define	PRE_PTRALIGN(x)			PRE_ALIGN(x)	/* アドレスの直前（自身を含む)のアラインメントアドレス */
#define	POST_PTRALIGN(x)		POST_ALIGN(x)	/* アドレスの直後（自身を含む)のアラインメントアドレス */

#define	MB_SIGNATURE			(0xA55Au)
#define	MB_SPACE				(0x0001u)
#define	MB_USE					(0x0002u)

#define	MB_SPACE_INFO_SIZE		(sizeof(MBSpaceProlog)+sizeof(MBSpaceEpilog))
#define	MB_USE_INFO_SIZE		(sizeof(MBUseProlog)+sizeof(MBUseEpilog))

/* メモリブロック状態 */
typedef	struct {
	uint16_t	signature;		/* 管理ブロックシグネチャ */
	uint16_t	status;			/* 管理ブロック状態 */
	uint8_t		dummy[sizeof(PtrInt_t)-sizeof(uint16_t)*2];
} MBIdentify;

/* 空きブロック先頭構造 */
typedef	struct {
	MBIdentify		identify;		/* メモリブロックID */
	MemSize_t		mb_size;
	Link			link;
} MBSpaceProlog;

/* 空きブロック最終構造 */
typedef	struct {
	MemSize_t		mb_size;
	MBIdentify		identify;		/* メモリブロックID */
} MBSpaceEpilog;

/* 使用ブロック先頭構造 */
typedef	struct {
	MBIdentify		identify;		/* メモリブロックID */
	MemSize_t		mb_size;
} MBUseProlog;

/* 使用ブロック最終構造 */
typedef	struct {
	MBIdentify		identify;		/* メモリブロックID */
} MBUseEpilog;


/* 空きメモリブロック先頭 */
static 	Link	mb_space_link;

/* 排他用mutex */
static MutexStruct	malloc_mutex;

void sys_malloc_init(void)
{
	link_clear(&mb_space_link);
	_kernel_mutex_create(&malloc_mutex);
}

#if defined(TEST_MODE)
static void* mb_start_addr;
static void* mb_end_addr;
#endif

void sys_malloc_add_block(void* start_addr, MemSize_t size)
{
	/* ブロックの先頭/最終にMBIdentifyを置いて、挟まれた初期空きブロックがALIGNされるstart/endを算出 */
	void* end_addr = PTRVAR(start_addr) + size;
	start_addr = POST_PTRALIGN(start_addr + sizeof(MBIdentify));
	end_addr = PRE_PTRALIGN(end_addr - sizeof(MBIdentify));
	size = PTRVAR(end_addr) - PTRVAR(start_addr); /* 初期の空きブロックのサイズ */

	/* 最低限必要なメモリ容量のチェック */
	if ( (PTRVAR(end_addr) < PTRVAR(start_addr)) || (size < (sizeof(MBSpaceProlog) + sizeof(MBSpaceEpilog))) ) {
		return;
	}

#if defined(TEST_MODE)
	mb_start_addr = start_addr;
	mb_end_addr = end_addr;
#endif

	/* メモリブロックの先頭と最終に「使用中」識別子を書き込む */
	((MBIdentify*)start_addr-1)->signature = ((MBIdentify*)(end_addr))->signature = MB_SIGNATURE;
	((MBIdentify*)start_addr-1)->status = ((MBIdentify*)(end_addr))->status = MB_USE;

	/* 空きメモリブロック初期化 */
	MBSpaceProlog* mb_prolog = (MBSpaceProlog*)start_addr;
	MBSpaceEpilog* mb_epilog = (MBSpaceEpilog*)(end_addr) - 1;
	mb_prolog->identify.signature = mb_epilog->identify.signature = MB_SIGNATURE;
	mb_prolog->identify.status = mb_epilog->identify.status = MB_SPACE;
	mb_prolog->mb_size = mb_epilog->mb_size = size;

	/* 空きメモリブロックをリンクリストに挿入 */
	link_add_last(&mb_space_link, &(mb_prolog->link));
}

/* sizeは使用中ヘッダ部を含めた必要サイズ */
static void* mb_alloc(MBSpaceProlog* mb_space_prolog, MemSize_t size)
{
	void* ret = NULL;

	/* 確保後の残りサイズチェック(余りが管理領域サイズ以下ならすべてを割り当てる) */
	MemSize_t remain_size = mb_space_prolog->mb_size - size;
	if ( MB_SPACE_INFO_SIZE <= remain_size ) {
		/* 残りブロックを新たに空きブロックとする */
		MBSpaceProlog* mb_remain_prolog = (MBSpaceProlog*)(PTRVAR(mb_space_prolog) + size);
		MBSpaceEpilog* mb_remain_epilog = (MBSpaceEpilog*)(PTRVAR(mb_remain_prolog) + remain_size) - 1;
		mb_remain_prolog->identify.signature = MB_SIGNATURE;
		mb_remain_prolog->identify.status = MB_SPACE;
		mb_remain_prolog->mb_size = mb_remain_epilog->mb_size = remain_size;
		link_add_last(&mb_space_link, &(mb_remain_prolog->link));
	}
	else {
		/* 残りが少ないのでブロック全体を割り当て */
		size = mb_space_prolog->mb_size;
	}

	/* 確保した領域の初期化 */
	MBUseProlog* mb_use_prolog = (MBUseProlog*)mb_space_prolog;
	MBUseEpilog* mb_use_epilog = (MBUseEpilog*)(PTRVAR(mb_use_prolog) + size) - 1;
	mb_use_epilog->identify.signature = MB_SIGNATURE;
	mb_use_prolog->identify.status = mb_use_epilog->identify.status = MB_USE;
	mb_use_prolog->mb_size = size;

	ret = (void*)(mb_use_prolog + 1);

	return ret;
}

void* sys_malloc_align_body(MemSize_t size, uint32_t align)
{
	void* ret = NULL;

	if ( size != 0 ) {
		/* 実際に必要となるメモリブロックサイズ (データ部+使用中ヘッダ) */
		size = POST_ALIGN(size + MB_USE_INFO_SIZE);

		/* 指定サイズ以上の空きブロックを探す */
		MBSpaceProlog* mb_space = NULL;
		Link* find_link = NULL;
		for ( find_link = mb_space_link.next; find_link != &mb_space_link; find_link = find_link->next ) {
			MBSpaceProlog* mb_space_prolog = containerof(find_link, MBSpaceProlog, link);
			/* 空きブロックを使用ブロックにしたときに実データがalignに合うかチェック */
			if ( ((PtrInt_t)((MBUseProlog*)mb_space_prolog+1) & (align-1)) == 0 ) {
				/* 指定サイズが確保できればそのまま使う */
				if ( size <= mb_space_prolog->mb_size ) {
					mb_space = mb_space_prolog;
					/* 空きリンクから外しておく */
					link_remove(&(mb_space->link));
					break;
				}
			}
			else {
				/* alignされた使用中ブロックとなる実データ領域のアドレスを求める */
				uint8_t* aligned_addr = PTRVAR(mb_space_prolog+1)+sizeof(MBSpaceEpilog)+sizeof(MBUseProlog);
				aligned_addr = PTRVAR(POST_ALIGN_BY(aligned_addr, align));
				/* 新たに空きブロックとなる領域のサイズ */
				MemSize_t f_size = (PTRVAR(aligned_addr) - sizeof(MBUseProlog)) - PTRVAR(mb_space_prolog);
				if ( (f_size + size) <= mb_space_prolog->mb_size ) {
					/* 領域分割してアラインド領域を確保 */
					MemSize_t second_mb_size = mb_space_prolog->mb_size - f_size;
					/* 最初のブロック */
					MBSpaceEpilog* mb_space_epilog = (MBSpaceEpilog*)(PTRVAR(mb_space_prolog) + f_size) - 1;
					mb_space_epilog->identify.signature = MB_SIGNATURE;
					mb_space_epilog->identify.status = MB_SPACE;
					mb_space_prolog->mb_size = mb_space_epilog->mb_size = f_size;
					/* ２番目のブロック (とりあえず空きブロックにする) */
					mb_space_prolog = (MBSpaceProlog*)(mb_space_epilog + 1);
					mb_space_epilog = (MBSpaceEpilog*)(PTRVAR(mb_space_prolog) + second_mb_size) - 1;
					mb_space_prolog->identify.signature = MB_SIGNATURE;
					mb_space_prolog->identify.status = MB_USE;
					mb_space_prolog->mb_size = mb_space_epilog->mb_size = second_mb_size;
					mb_space = mb_space_prolog;
					break;
				}
			}
		}

		if ( mb_space ) {
			/* 指定の空きブロックからメモリ割り当て */
			ret = mb_alloc(mb_space, size);
			if ( !ret ) {
				//tprintf("system error.(malloc\n");
			}
		}
	}

	return ret;


}

void* sys_malloc_body(MemSize_t size)
{
	void* ret = NULL;

	if ( size != 0 ) {
		/* 実際に必要となるメモリブロックサイズ (データ部+使用中ヘッダ) */
		size = POST_ALIGN(size + MB_USE_INFO_SIZE);

		/* 指定サイズ以上の空きブロックを探す */
		MBSpaceProlog* mb_space = NULL;
		Link* find_link = NULL;
		for ( find_link = mb_space_link.next; find_link != &mb_space_link; find_link = find_link->next ) {
			MBSpaceProlog* mb_temp = containerof(find_link, MBSpaceProlog, link);
			if ( size <= mb_temp->mb_size ) {
				mb_space = mb_temp;
				break;
			}
		}
		if ( mb_space ) {
			/* 対象空きブロックをリンクリストから外す */
			link_remove(&(mb_space->link));

			/* 指定の空きブロックからメモリ割り当て */
			ret = mb_alloc(mb_space, size);
			if ( !ret ) {
				//tprintf("system error.(malloc\n");
			}
		}
	}
	return ret;
}

OSAPISTUB void* __sys_malloc_align(MemSize_t size, uint32_t align)
{
	_kernel_mutex_lock(&malloc_mutex);
	void* ret = sys_malloc_align_body(size, align);
	_kernel_mutex_unlock(&malloc_mutex);

	return ret;

}

OSAPISTUB void* __sys_malloc(MemSize_t size)
{
	_kernel_mutex_lock(&malloc_mutex);
	void* ret = sys_malloc_body(size);
	_kernel_mutex_unlock(&malloc_mutex);

	return ret;
}

OSAPISTUB void __sys_free(void* ptr)
{
	_kernel_mutex_lock(&malloc_mutex);

	/* シグネチャチェック */
	MBUseProlog* mb_use_prolog = (MBUseProlog*)ptr - 1;
	if ( (mb_use_prolog->identify.signature != MB_SIGNATURE) || (mb_use_prolog->identify.status != MB_USE) ) {
		goto err_ret;
	}
	MBUseEpilog* mb_use_epilog = (MBUseEpilog*)(PTRVAR(mb_use_prolog) + mb_use_prolog->mb_size) - 1;
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
		mb_space_epilog = (MBSpaceEpilog*)mb_use_prolog - 1; /* 前方ブロックのepilog */
		mb_space_prolog = (MBSpaceProlog*)(PTRVAR(mb_use_prolog) - mb_space_epilog->mb_size);
		mb_space_epilog = (MBSpaceEpilog*)mb_back_id - 1; /* 対象ブロックのepilog(新しいepilog) */
		mb_space_epilog->identify.status = MB_SPACE;
		mb_space_prolog->mb_size += mb_use_prolog->mb_size;
		mb_space_epilog->mb_size = mb_space_prolog->mb_size;
	}
	else {
		/* 前方ブロックは使用中なので対象ブロックを空きブロック先頭にする */
		/* signatureとmb_sizeは変わらないので書き換えない */
		mb_space_prolog = (MBSpaceProlog*)mb_use_prolog;
		mb_space_epilog = (MBSpaceEpilog*)(PTRVAR(mb_space_prolog) + mb_space_prolog->mb_size) - 1;
		mb_space_prolog->identify.status = mb_space_epilog->identify.status = MB_SPACE;
		mb_space_epilog->mb_size = mb_space_prolog->mb_size;
		/* 空きリンクリストに追加 */
		link_add_last(&mb_space_link, &(mb_space_prolog->link));
	}

	/* 対象ブロックの後方ブロックチェック */
	if ( mb_back_id->status == MB_SPACE ) {
		/* 後方ブロックが空きブロックならブロックを連結する */
		MBSpaceProlog* mb_back_prolog = (MBSpaceProlog*)mb_back_id;
		MBSpaceEpilog* mb_back_epilog = (MBSpaceEpilog*)(PTRVAR(mb_back_prolog) + mb_back_prolog->mb_size) - 1;
		mb_space_prolog->mb_size += mb_back_prolog->mb_size;
		mb_back_epilog->mb_size = mb_space_prolog->mb_size;
		/* 後方ブロックは対象ブロックに連結するので空きリンクリストから外す */
		link_remove(&(mb_back_prolog->link));
	}
	else {
		/* 後方ブロックが使用中なら処理は無し */
	}

err_ret:
	_kernel_mutex_unlock(&malloc_mutex);

	return;
}
