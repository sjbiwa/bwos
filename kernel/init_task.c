/*
 * init_task.c
 *
 *  Created on: 2015/02/07
 *      Author: biwa
 */
#include "common.h"
#include "api.h"

extern void main_task(void);

static TaskCreateInfo	main_task_info = {
	"MAIN_TASK", TASK_ACT|TASK_FPU, main_task, 0, 1024, 1024, 1,
};

void init_task(void)
{
	lprintf_init();
	task_create(&main_task_info);

	/* init_taskのオブジェクトはすべてstaticメモリを使うので */
	/* 終了してはいけない */
	for (;;) {
		task_sleep();
	}
}
