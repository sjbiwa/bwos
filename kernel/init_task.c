/*
 * init_task.c
 *
 *  Created on: 2015/02/07
 *      Author: biwa
 */
#include "bwos.h"

extern void main_task(void);
extern void init_task_arch_depend();

static TaskCreateInfo	main_task_info = {
	"MAIN_TASK", TASK_ACT|TASK_SYS|TASK_FPU, main_task, 0, 1024, 1024, 1,
};

void init_task(void)
{
	lprintf_init();

	/* 初期タスクでのarch依存処理 */
	init_task_arch_depend();

	task_create(&main_task_info);
	/* init_taskのオブジェクトはすべてstaticメモリを使うので */
	/* 終了してはいけない */
	for (;;) {
		task_sleep();
	}
}
