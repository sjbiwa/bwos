/*
 * main.c
 *
 *  Created on: 2012/12/01
 *      Author: biwa
 */
#include "common.h"
#include "task.h"
#include "malloc.h"
#include "version.h"

extern TaskStruct	task_info[];
extern uint32_t		task_info_num;

int main(void)
{
	int ix;
	/* ハードウェア初期化 */
	arch_system_preinit();
	tprintf("Booting BWOS Ver " OS_VERSION  "\n");
	irq_init();
	timer_init();
	lprintf_init();
	sys_malloc_init();
	arch_system_postinit();

	/* OSリソース初期化 */
	task_init();
	for (ix=0; ix<task_info_num; ix++) {
		task_create(&task_info[ix]);
	}

	schedule();
}
