/*
 * startup.c
 *
 *  Created on: 2012/12/01
 *      Author: biwa
 */
#include "kernel.h"
#include "version.h"
#include "kernel/smp.h"

static SpinLockObj	spin_lock_obj;

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

	/* ハードウェア初期化 */
	arch_system_preinit();
	tprintf("Booting BWOS Ver " OS_VERSION  "\n");

	/* 起動時メモリマネージャにメモリを登録 */
	arch_register_st_memory();

	irq_init();
	task_init();
	flag_init();
	mutex_init();
	sem_init();
	fixmb_init();
	msgq_init();
	timer_init();

	arch_timer_init();
	arch_system_postinit();

	/* 標準メモリマネージャ初期化 */
	sys_malloc_init();
	/* 起動時メモリマネージャの残りメモリを標準メモリマネージャに移行 */
	st_malloc_normalize();
	/* その他のメモリを標準メモリマネージャに登録 */
	arch_register_normal_memory();

	/* 初期タスク生成 */
	task_init_task_create();


	spin_init(&spin_lock_obj);

#if defined(SMP_ENABLE)
	smp_boot_slave_cpu();
#endif

	for (;;) {
		spin_lock(&spin_lock_obj);
		tprintf("Booting master cpu:%d\n", cpuid);
		spin_unlock(&spin_lock_obj);
		wait_loop();
	}
	schedule();
}

#if defined(SMP_ENABLE)
static void startup_slave(uint32_t cpuid)
{
	arch_system_preinit();
	mmgr_init_slave();
	for (;;) {
		spin_lock(&spin_lock_obj);
		tprintf("Booting slave cpu:%d\n", cpuid);
		spin_unlock(&spin_lock_obj);
		wait_loop();
	}
	schedule();
}

#endif

void startup(void)
{
#if defined(SMP_ENABLE)
	uint32_t cpuid = CPUID_get();
	if ( cpuid == 0 ) {
		startup_master(cpuid);
	}
	else {
		startup_slave(cpuid);
	}
#else
	startup_master(0);
#endif
}
