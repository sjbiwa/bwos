/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * startup.c
 *
 *  Created on: 2012/12/01
 *      Author: biwa
 */
#include "kernel.h"
#include "version.h"

static volatile uint8_t cpu_boot_sync_flag[CPU_NUM];

static void boot_each_core(uint32_t cpuid)
{
	schedule(&cpu_struct[cpuid]);
	if ( USE_SMP == 1 ) {
		cpu_boot_sync_flag[cpuid] = 1;
		sync_barrier();
		for (;;) {
			int ix;
			for ( ix=0; ix<CPU_NUM; ix++ ) {
				if ( cpu_boot_sync_flag[ix] == 0 ) {
					break;
				}
			}
			if ( ix == CPU_NUM ) {
				break;
			}
		}
	}
	_dispatch();
	for (;;);
}

static void startup_master(uint32_t cpuid)
{
	int ix;

	/* 起動時メモリマネージャにメモリを登録 */
	arch_register_st_memory();

	/* ハードウェア初期化 */
	arch_system_preinit(cpuid);

	tprintf("Booting BWOS Ver " OS_VERSION  "\n");

	task_init();
	flag_init();
	mutex_init();
	sem_init();
	fixmb_init();
	msgq_init();
	timer_init();

	arch_timer_init(cpuid);
	arch_system_postinit(cpuid);

	/* 標準メモリマネージャ初期化 */
	__sys_malloc_init();
	/* 起動時メモリマネージャの残りメモリを標準メモリマネージャに移行 */
	__st_malloc_normalize();
	/* その他のメモリを標準メモリマネージャに登録 */
	arch_register_normal_memory();

	/* 初期タスク生成 */
	task_init_task_create();

	if ( USE_SMP == 1 ) {
		/* コア起動完了同期フラグ */
		for ( ix=0; ix < CPU_NUM; ix++ ) {
			cpu_boot_sync_flag[ix] = 0;
		}
		/* 初期起動コアに関連するスレーブコアを起動 */
		smp_boot_relayed_cpu(cpuid);
	}

	boot_each_core(cpuid);
}

static void startup_slave(uint32_t cpuid)
{
	if ( USE_SMP == 1 ) {
		arch_system_preinit(cpuid);
		arch_timer_init(cpuid);
		arch_system_postinit(cpuid);

		/* スレーブコアに関連するスレーブコアを起動 */
		smp_boot_relayed_cpu(cpuid);

		boot_each_core(cpuid);
	}
}

void startup(void)
{
	uint32_t cpuid = CPUID_get();
	if ( (USE_SMP == 0) || (cpuid == MASTER_CPU_ID) ) {
		startup_master(cpuid);
	}
	else {
		startup_slave(cpuid);
	}
}
