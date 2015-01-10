/*
 * startup.c
 *
 *  Created on: 2012/12/01
 *      Author: biwa
 */
#include "common.h"
#include "task.h"
#include "malloc.h"
#include "version.h"

void startup(void)
{
	int ix;
	/* ハードウェア初期化 */
	arch_system_preinit();
	tprintf("Booting BWOS Ver " OS_VERSION  "\n");
	irq_init();
	timer_init();
	lprintf_init();
	sys_malloc_init();
	task_init();
	arch_system_postinit();

	schedule();
}
