#include <string.h>
#include "bwos.h"
#include "cache.h"
#include "gicv2reg.h"
#include "kernel.h"

#if USE_SMP != 1
#define	task_set_affinity(...)
#endif

//#undef CPU_CORE0
//#undef CPU_CORE1
//#undef CPU_CORE2
//#undef CPU_CORE3
//#undef CPU_CORE4
//#undef CPU_CORE5
#undef CPU_CORE6
#undef CPU_CORE7

//#define	CPU_CORE0	CPU_CORE4
//#define	CPU_CORE1	CPU_CORE5
//#define	CPU_CORE2	CPU_CORE4
//#define	CPU_CORE3	CPU_CORE5
//#define	CPU_CORE4	CPU_CORE0
//#define	CPU_CORE5	CPU_CORE1
#define	CPU_CORE6	CPU_CORE4
#define	CPU_CORE7	CPU_CORE5
//#define	CPU_CORE6	CPU_CORE4
//#define	CPU_CORE7	CPU_CORE5

//#undef	TASK_FPU
//#define	TASK_FPU	0

//#define	task_set_affinity(...)

#define	TASK_SET_AFFI(c)	task_set_affinity(c)
//#define	TASK_SET_AFFI(c)	task_set_affinity((c % 4))
//#define	TASK_SET_AFFI(c)	task_set_affinity((c % 2) + 4)
//#define	TASK_SET_AFFI(c)

/* configuration task */
static int		task_struct[1000];

void task1(void* arg0, void* arg1)
{
	for (uint32_t ix = CPUID_get()+1;;ix++ ) {
		lprintf("CORE=%d:task1:%d : MIDR=%08X\n", CPUID_get(), ix, (uint32_t)MIDR_EL1_get());
		TASK_SET_AFFI(ix%CPU_NUM);
		task_tsleep(MSEC(10));
	}
}

void task2(void* arg0, void* arg1)
{
	for (uint32_t ix = CPUID_get()+1;;ix++ ) {
		lprintf("CORE=%d:task2:%d : MIDR=%08X\n", CPUID_get(), ix, (uint32_t)MIDR_EL1_get());
		TASK_SET_AFFI(ix%CPU_NUM);
		task_tsleep(MSEC(1));
	}
}

#define	TASK_ACT		(0)

TaskCreateInfo	task_info[] = {
		{"TASK11", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 5, (void*)1},
		{"TASK12", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)1},
		{"TASK02", CPU_CORE4|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)0},
		{"TASK11", CPU_CORE5|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 5, (void*)1},
		{"TASK12", CPU_CORE6|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)1},
		{"TASK13", CPU_CORE7|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 7, (void*)0},
		{"TASK01", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)0},
		{"TASK02", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)0},
		{"TASK11", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)1},
		{"TASK12", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)1},
		{"TASK02", CPU_CORE4|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)0},
		{"TASK11", CPU_CORE5|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)1},
		{"TASK12", CPU_CORE6|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)1},
		{"TASK01", CPU_CORE5|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)0},
		{"TASK01", CPU_CORE6|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)1},
		{"TASK01", CPU_CORE7|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)2},
		{"TASK01", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)3},
		{"TASK01", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)4},
		{"TASK01", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)5},
		{"TASK01", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)6},
		{"TASK01", CPU_CORE4|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)7},
		{"TASK02", CPU_CORE5|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)0},
		{"TASK11", CPU_CORE6|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)1},
		{"TASK12", CPU_CORE7|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)1},
		{"TASK02", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)0},
		{"TASK11", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)1},
		{"TASK12", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)1},
		{"TASK01", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 5, (void*)0},
#if 1
		{"TASK02", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)0},
		{"TASK11", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 5, (void*)1},
		{"TASK12", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)1},
		{"TASK02", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)0},
		{"TASK11", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 5, (void*)1},
		{"TASK12", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)1},
		{"TASK13", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 7, (void*)0},
		{"TASK01", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)0},
		{"TASK02", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)0},
		{"TASK11", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)1},
		{"TASK12", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)1},
		{"TASK02", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)0},
		{"TASK11", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)1},
		{"TASK12", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)1},
		{"TASK01", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)0},
		{"TASK01", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)1},
		{"TASK01", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)2},
		{"TASK01", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)3},
		{"TASK01", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)4},
		{"TASK01", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)5},
		{"TASK01", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)6},
		{"TASK01", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)7},
		{"TASK02", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)0},
		{"TASK11", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)1},
		{"TASK12", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)1},
		{"TASK02", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)0},
		{"TASK11", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)1},
		{"TASK12", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task2, 4096, 0, 6, (void*)1},
#endif
};

void main_task(void)
{
	lprintf("main_task\n");
	int ix;
	
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
		lprintf("%d:%d\n", ix, task_struct[ix]);
	}
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_active(task_struct[ix], (void*)ix);
	}

	task_sleep();
}
