/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * fixmb.h
 *
 *  Created on: 2015/01/16
 *      Author: biwa
 */

#ifndef FIXMB_H_
#define FIXMB_H_

#include "common.h"
#include "link.h"
#include "smp.h"

typedef	struct tagFixmbList {
	uint32_t	index;
} FixmbList;

typedef	struct tagFixmbListMng {
	FixmbList	top;
	FixmbList	last;
} FixmbListMng;

typedef struct tagFixmbStruct {
	Link			link;			/* 待ちタスク連結用 */
#if USE_SMP==1
	SpinLockObj		spin_lock;		/* spin lock */
#endif
	bool			id_initialized;	/* 初期完了フラグ */
	void*			mb_area;		/* メモリブロック領域 */
	uint32_t		mb_size;		/* メモリブロックサイズ */
	uint32_t		mb_length;		/* メモリブロック総数 */
	uint32_t		use_count;		/* 使用中メモリブロック数 */
	bool			is_list_mode;	/* 空きリストを使用するモード */
	uint32_t		mb_array_index;	/* メモリブロック割り当て位置(配列モード時) */
	FixmbListMng	list;			/* 空きリスト(単方向リンク) */
	uint32_t*		use_bitmap;		/* 使用中/空き識別ビットマップ */
} FixmbStruct;

/* fixmb待ちオブジェクト */
typedef	struct {
	FixmbStruct*	fixmb;			/* MBオブジェクト */
	void**			ptr;			/* 確保したMBアドレス格納エリアへのポインタ */
} FixmbInfoStruct;

#endif /* FIXMB_H_ */
