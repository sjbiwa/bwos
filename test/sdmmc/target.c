#include "bwos.h"

/* configuration task */
static int		task_struct[100];

void task1(uint32_t arg0, uint32_t arg1)
{
	lprintf("CORE=%d:task%d\n", CPUID_get(), arg0);
	for (;;) {
		sdmmc_check();
		task_tsleep(SEC(1));
	}
}


TaskCreateInfo	task_info[] = {
		{"TASK01", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 1024, 0, 5, (void*)0},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}
