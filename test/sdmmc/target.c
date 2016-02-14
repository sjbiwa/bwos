#include "bwos.h"

/* configuration task */
static int		task_struct[100];

void task1(uint32_t arg0, uint32_t arg1)
{
	lprintf("CORE=%d:task%d\n", CPUID_get(), arg0);
	for (;;) {
		sdmmc_reset(0);
		sdmmc_check();
		task_tsleep(SEC(1));
	}
}

extern uint32_t read_counter;

void task2(uint32_t arg0, uint32_t arg1)
{
	for (;;) {
		lprintf("TASK2:%d\n", read_counter);
		sdmmc_check();
		task_tsleep(SEC(10));
	}
}

TaskCreateInfo	task_info[] = {
		{"TASK01", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 1024, 0, 5, (void*)0},
//		{"TASK02", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task2, 1024, 0, 1, (void*)0},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}
