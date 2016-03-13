#include <string.h>
#include "bwos.h"
#include "cache.h"

extern "C" {
extern	void lprintf(...);
}

#if USE_SMP != 1
#define	task_set_affinity(...)
#endif

/* configuration task */
static int		task_struct[100];

void task1(void* arg0, void* arg1)
{
	for (;;) {
		lprintf("task\n");
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
