/*
 * startup.c
 *
 *  Created on: 2012/12/01
 *      Author: biwa
 */
#include "kernel.h"
#include "version.h"
#include "kernel/smp.h"

static volatile uint32_t value;
static void wait_loop()
{
	for (int ix=0; ix < 100000; ix++) {
		value = ioread32(0x01C25C00+0x1a4);
	}
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
//	fixmb_init();
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

	smp_boot_slave_cpu();

	schedule(&cpu_struct[CPUID_get()]);
	_dispatch();
}

static void startup_slave(uint32_t cpuid)
{
	arch_system_preinit(cpuid);
	arch_timer_init(cpuid);
	arch_system_postinit(cpuid);

	CpuStruct* cpu = &cpu_struct[CPUID_get()];
	schedule(cpu);
	_dispatch();
	for (;;);
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
