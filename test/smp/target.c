/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "bwos.h"
#include "cp15reg.h"

typedef	struct {
	uint32_t	buff1[16];
	uint8_t		buff2[8];
} Message;

/* configuration task */
static int		task_struct[32];

static volatile int mutex;
static volatile int fixmb;
static volatile int sem;
static volatile int flag;

void task1(uint32_t arg0, uint32_t arg1)
{
	for (;;) {
		lprintf("CORE=%d:task1\n", CPUID_get());
		task_tsleep(MSEC(100));
	}
}

void task2(uint32_t arg0, uint32_t arg1)
{
	for (;;) {
		lprintf("CORE=%d:task2\n", CPUID_get());
		task_tsleep(MSEC(200));
	}
}

void task3(uint32_t arg0, uint32_t arg1)
{
	/* fixmb full待ち */
	mutex_lock(mutex);

	Message* ptr;
	for (int ix=0;;ix++ ) {
		lprintf("CORE=%d:task3\n", CPUID_get());
		int ret = fixmb_trequest(fixmb, (void**)(&ptr), SEC(30));
		lprintf("ret=%d\n", ret);
		if ( ret == RT_OK ) {
			lprintf("CORE=%d:get %08X\n", CPUID_get(), ptr);
		}
		else {
			lprintf("CORE=%d:%d:get xxxxx error\n", CPUID_get(), ix);
		}
	}
}

void task4(uint32_t arg0, uint32_t arg1)
{
	void*  ptr[64];
	int ix;
	for ( ix=0;;ix++ ) {
		if ( fixmb_trequest(fixmb, (void**)(&ptr[ix]), TMO_POLL) == RT_OK ) {
			lprintf("CORE=%d:get %08X\n", CPUID_get(), ptr[ix]);
			memset(ptr[ix], 0x45, sizeof(Message));
		}
		else {
			lprintf("CORE=%d:%d:get yyyyy error\n", CPUID_get(), ix);
			break;
		}
	}
	mutex_unlock(mutex);

	task_tsleep(SEC(70));
	for (ix--; 0 <= ix; ix--) {
		fixmb_release(fixmb, ptr[ix]);
		task_tsleep(SEC(50));
	}
}

void task5(uint32_t arg0, uint32_t arg1)
{
	lprintf("task5:%d\n", sem);
	for (;;) {
		int ret = sem_trequest(sem, 10, MSEC(5));
		lprintf("task5:request ret=%d\n", ret);
	}
}

void task6(uint32_t arg0, uint32_t arg1)
{
	lprintf("task6\n");
	task_tsleep(MSEC(5));
	for (;;) {
		int ret = sem_trequest(sem, 5, MSEC(3));
		lprintf("task6:request ret=%d\n", ret);
		ret = sem_release(sem, 2);
		task_tsleep(MSEC(3));
	}
}

void task7(uint32_t arg0, uint32_t arg1)
{
	lprintf("task7\n");
	for (;;) {
		uint32_t pattern;
		flag_wait(flag, 0x0001, FLAG_OR|FLAG_CLR, &pattern);
		lprintf("flag_wait done\n");
	}
}

void task8(uint32_t arg0, uint32_t arg1)
{
	lprintf("task8\n");
	for (;;) {
		task_tsleep(MSEC(500));
		flag_set(flag, 0x0001);
		lprintf("flag_set done\n");
	}
}


static int	msgq1;
static int	msgq2;

typedef	struct {
	uint32_t	cmd;
	uint32_t	param1;
	uint32_t	param2;
} MsgCmd;

typedef	struct {
	uint32_t	cmd;
	uint32_t	param1;
	uint32_t	param2;
	uint32_t	param3;
	uint32_t	param4;
	uint32_t	param5;
	uint32_t	param6;
} MsgCmd2;

void task_msgq_1(void)
{
	int ix;
	void* ptr;
	MsgCmd	cmd;
	for ( ix=1; ix < 100000; ix++ ) {
		cmd.cmd = ix;
		cmd.param1 = ix*10;
		cmd.param2 = ix*100;
		msgq_tsend(msgq1, &cmd, sizeof(cmd), SEC(1));
		lprintf("%d:TASK1:MSGQ1:send:%d\n", CPUID_get(), ix);
		task_tsleep(MSEC(100));
	}
	task_sleep();
}

void task_msgq_2(void)
{
	int ix;
	MsgCmd cmd;
	MsgCmd2 cmd2;
	for ( ix=0;; ix++ ) {
		msgq_recv(msgq1, &cmd, sizeof(cmd));
		lprintf("%d:TASK2:MSG1:recv:%d\n", CPUID_get(), ix);
		cmd2.cmd = cmd.cmd;
		cmd2.param5 = cmd.param1;
		cmd2.param6 = cmd.param2;
		lprintf("%d:TASK2:CMD2:%d %d %d\n", CPUID_get(), cmd2.cmd, cmd2.param5, cmd2.param6);
		task_tsleep(MSEC(5));
	}
}

void task_msgq_3(void)
{
	int ix;
	MsgCmd cmd;
	for ( ix=0;; ix++ ) {
		msgq_recv(msgq1, &cmd, sizeof(cmd));
		lprintf("%d:TASK3:MSG2:recv:%d\n", CPUID_get(), ix);
		lprintf("%d:TASK3:CMD2:%d %d %d\n", CPUID_get(), cmd.cmd, cmd.param1, cmd.param2);
	}
}


TaskCreateInfo	task_info[] = {
#if 1
		{"TASK01", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task1, 0, 4096, 4096, 5, (void*)0},
		{"TASK02", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task2, 0, 4096, 4096, 6, (void*)0},
		{"TASK11", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 0, 4096, 4096, 5, (void*)1},
		{"TASK12", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task2, 0, 4096, 4096, 6, (void*)1},
		{"TASK01", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task1, 0, 4096, 4096, 5, (void*)0},
		{"TASK02", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task2, 0, 4096, 4096, 6, (void*)0},
		{"TASK11", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 0, 4096, 4096, 5, (void*)1},
		{"TASK12", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task2, 0, 4096, 4096, 6, (void*)1},
		{"TASK13", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task3, 0, 4096, 4096, 7, (void*)0},
		{"TASK14", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task4, 0, 4096, 4096, 8, (void*)1},
		{"TASK14", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task6, 0, 4096, 4096, 8, (void*)0},
		{"TASK13", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task5, 0, 4096, 4096, 7, (void*)0},
		{"TASK14", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task6, 0, 4096, 4096, 8, (void*)0},
		{"TASK13", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task5, 0, 4096, 4096, 7, (void*)0},
		{"TASK14", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task6, 0, 4096, 4096, 8, (void*)0},
		{"TASK13", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task5, 0, 4096, 4096, 7, (void*)0},
		{"TASK15", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task7, 0, 4096, 4096, 8, (void*)0},
		{"TASK16", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task8, 0, 4096, 4096, 7, (void*)0},
#endif
		{"TASKm1", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_1, 0, 4096, 4096, 8, (void*)0},
		{"TASKm2", CPU_CORE1|TASK_FPU|TASK_SYS, task_msgq_2, 0, 4096, 4096, 7, (void*)0},
		{"TASKm3", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_3, 0, 4096, 4096, 7, (void*)0},
};

void main_task(void)
{
	int ix;
	mutex = mutex_create();
	mutex_lock(mutex);
	fixmb = fixmb_create(sizeof(Message), 16);
	sem = sem_create(16);
	flag = flag_create();
	/* 共有リソース初期化 */
	msgq1 = msgq_create(128);
	msgq2 = msgq_create(1024);
	__dsb();

	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_active(task_struct[1], 0);
	task_sleep();
}
