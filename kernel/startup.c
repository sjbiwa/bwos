/*
 * startup.c
 *
 *  Created on: 2012/12/01
 *      Author: biwa
 */
#include "kernel.h"
#include "version.h"

void startup(void)
{
	int ix;
	/* ハードウェア初期化 */
	arch_system_preinit();
	tprintf("Booting BWOS Ver " OS_VERSION  "\n");

	/* 起動時メモリマネージャにメモリを登録 */
	arch_register_st_memory();

	irq_init();
	timer_init();
	task_init();
	flag_init();
	mutex_init();
	sem_init();
	fixmb_init();
	msgq_init();

	arch_system_postinit();

	/* 標準メモリマネージャ初期化 */
	sys_malloc_init();
	/* 起動時メモリマネージャの残りメモリを標準メモリマネージャに移行 */
	st_malloc_normalize();
	/* その他のメモリを標準メモリマネージャに登録 */
	arch_register_normal_memory();

	/* 初期タスク生成 */
	task_init_task_create();

	schedule();
}
