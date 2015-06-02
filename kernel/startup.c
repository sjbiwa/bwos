/*
 * startup.c
 *
 *  Created on: 2012/12/01
 *      Author: biwa
 */
#include "kernel.h"
#include "version.h"
#include "kernel/smp.h"

static volatile uint8_t cpu_boot_sync_flag[CPU_NUM];

static volatile uint32_t value;
static void wait_loop()
{
	for (int ix=0; ix < 100000; ix++) {
		value = ioread32(0x01C25C00+0x1a4);
	}
}

static void boot_each_core(uint32_t cpuid)
{
	schedule(&cpu_struct[cpuid]);
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
//	flag_init();
	mutex_init();
//	sem_init();
	fixmb_init();
//	msgq_init();
	timer_init();

	arch_timer_init(cpuid);
	arch_system_postinit(cpuid);

	/* 標準メモリマネージャ初期化 */
	sys_malloc_init();
	/* 起動時メモリマネージャの残りメモリを標準メモリマネージャに移行 */
	st_malloc_normalize();
	/* その他のメモリを標準メモリマネージャに登録 */
	arch_register_normal_memory();

	/* 初期タスク生成 */
	task_init_task_create();

	/* コア起動完了同期フラグ */
	for ( ix=0; ix < CPU_NUM; ix++ ) {
		cpu_boot_sync_flag[ix] = 0;
	}

	smp_boot_slave_cpu();

	boot_each_core(cpuid);
}

static void startup_slave(uint32_t cpuid)
{
	arch_system_preinit(cpuid);
	arch_timer_init(cpuid);
	arch_system_postinit(cpuid);

	boot_each_core(cpuid);
}

void startup(void)
{
	uint32_t cpuid = CPUID_get();
	if ( cpuid == MASTER_CPU_ID ) {
		startup_master(cpuid);
	}
	else {
		startup_slave(cpuid);
	}
}
