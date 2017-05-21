/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * init_task.c
 *
 *  Created on: 2015/02/07
 *      Author: biwa
 */
#include "bwos.h"

void init_task(void* cre_param, void* sta_param)
{
	lprintf_init();

	/* 初期タスクでのarch依存処理 */
	arch_init_task_depend();

#if USE_SMP == 1
	/* CPU affinity拡張機能 */
	cpu_affinity_ext_init();
#endif
	
	/* ユーザ側エントリ呼び出し */
	main_task();

	/* init_taskのオブジェクトはすべてstaticメモリを使うので */
	/* 終了してはいけない */
	for (;;) {
		task_sleep();
	}
}
