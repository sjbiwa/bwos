#include <string.h>
#include "bwos.h"
#include "cache.h"
#include "armv8reg.h"

extern "C" {
extern	void lprintf(...);

extern void benchmark(uint32_t cpuid, int arsize);

typedef struct {
    uint32_t      tv_sec;     /* 秒 */
    uint32_t     tv_usec;    /* マイクロ秒 */
} xtimeval;

void xgettimeofday(xtimeval* tm)
{
	uint64_t tval = (CNTPCT_EL0_get() * 100) / (CNTFRQ_VALUE/10000);
	tm->tv_sec = tval / 1000000;
	tm->tv_usec = tval % 1000000;
}

}


/* configuration task */
static int		task_struct[100];

void task_benchmark(void* arg0, void* arg1)
{
	for (;;) {
		benchmark(CPUID_get(), 200);
		task_tsleep(SEC(2));
	}
}


TaskCreateInfo	task_info[] = {
		{"TASK1", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task_benchmark, 65536, 0, 5, (void*)1},
#if defined(USE_SMP)
		{"TASK2", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task_benchmark, 65536, 0, 5, (void*)2},
		{"TASK3", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task_benchmark, 65536, 0, 5, (void*)3},
		{"TASK4", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task_benchmark, 65536, 0, 5, (void*)4},
		{"TASK5", CPU_CORE4|TASK_ACT|TASK_FPU|TASK_SYS, task_benchmark, 65536, 0, 5, (void*)5},
		{"TASK6", CPU_CORE5|TASK_ACT|TASK_FPU|TASK_SYS, task_benchmark, 65536, 0, 5, (void*)6},
		{"TASK7", CPU_CORE6|TASK_ACT|TASK_FPU|TASK_SYS, task_benchmark, 65536, 0, 5, (void*)7},
		{"TASK8", CPU_CORE7|TASK_ACT|TASK_FPU|TASK_SYS, task_benchmark, 65536, 0, 5, (void*)8},
#endif
};

void main_task(void)
{
	for ( int ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}

