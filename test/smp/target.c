#include "bwos.h"

#if USE_SMP != 1
#define	task_set_affinity(...)
#endif

void wait(uint32_t t)
{
	volatile uint32_t count = t;
	while ( 0 < count ) {
		count--;
	}
}

void timer_handler(void* param)
{
//	task_wakeup(task_struct[0]);
}

void timer_start(uint32_t arg0, uint32_t arg1)
{
	TimerInfo info;
	int timer = timer_create();
	info.cyclic = SEC(10);
	info.tmout = SEC(1);
	info.kind = TIMER_CYCLIC;
	info.param = 0;
	info.handler = timer_handler;

	timer_set(timer, &info);
	timer_enable(timer, true);
}

typedef	struct {
	uint32_t	buff1[16];
	uint8_t		buff2[8];
} Message;

/* configuration task */
static int		task_struct[100];

static volatile int mutex;
static volatile int fixmb;
static volatile int sem;
static volatile int flag;

volatile uint32_t test_array[128]  __attribute__((aligned(64)));

void task1(uint32_t arg0, uint32_t arg1)
{
	for (uint32_t ix = CPUID_get()+1;;ix++ ) {
		lprintf("CORE=%d:task%d:%d\n", CPUID_get(), arg0, ix);
		task_set_affinity(ix%CPU_NUM);
		test_array[0] = 0;
		cache_clean_invalid_sync(test_array, sizeof(test_array));
		test_array[0] = 0xa5a5a5a5;
		cache_invalid_sync(test_array, sizeof(test_array));
		if ( test_array[0] != 0xa5a5a5a5 ) {
			lprintf("0:[0] = %08X\n", test_array[0]);
		}
		test_array[0] = 0x34343434;
		cache_invalid_sync(test_array, sizeof(test_array));
		if ( test_array[0] != 0x34343434 ) {
			lprintf("1:[0] = %08X\n", test_array[0]);
		}
		task_tsleep(SEC(1));
	}
}

void task2(uint32_t arg0, uint32_t arg1)
{
	for (uint32_t ix = CPUID_get()+1;;ix++ ) {
		lprintf("CORE=%d:task2:%d\n", CPUID_get(), ix);
		task_set_affinity(ix%CPU_NUM);
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
		int ret = fixmb_trequest(fixmb, (void**)(&ptr), MSEC(500));
		if ( ret == RT_OK ) {
			lprintf("CORE=%d:get %08X\n", CPUID_get(), ptr);
		}
		else {
			lprintf("CORE=%d:%d:get xxxxx error\n", CPUID_get(), ix);
		}
		task_set_affinity(ix%CPU_NUM);
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
		task_set_affinity(ix%CPU_NUM);
	}
	mutex_unlock(mutex);

	task_tsleep(SEC(70));
	for (ix--; 0 <= ix; ix--) {
		fixmb_release(fixmb, ptr[ix]);
		task_tsleep(SEC(50));
		task_set_affinity(ix%CPU_NUM);
	}
}

void task5(uint32_t arg0, uint32_t arg1)
{
	lprintf("CORE=%d:task5:%d\n", CPUID_get(), sem);
	for (int ix=0;;ix++ ) {
		int ret = sem_trequest(sem, 10, MSEC(5));
		lprintf("CORE=%d:task5:request ret=%d\n", CPUID_get(), ret);
		task_set_affinity(ix%CPU_NUM);
	}
}

void task6(uint32_t arg0, uint32_t arg1)
{
	lprintf("CORE=%d:task6\n", CPUID_get());
	task_tsleep(MSEC(5));
	for (int ix=0;;ix++ ) {
		int ret = sem_trequest(sem, 5, MSEC(3));
		lprintf("CORE=%d:task6:request ret=%d\n", CPUID_get(), ret);
		ret = sem_release(sem, 2);
		task_tsleep(MSEC(3));
		task_set_affinity(ix%CPU_NUM);
	}
}

void task7(uint32_t arg0, uint32_t arg1)
{
	lprintf("CORE=%d:task7\n", CPUID_get());
	for (int ix=0;;ix++ ) {
		uint32_t pattern;
		flag_wait(flag, 0x0001, FLAG_OR|FLAG_CLR, &pattern);
		lprintf("CORE=%d:flag_wait done\n", CPUID_get());
		task_set_affinity(ix%CPU_NUM);
	}
}

void task8(uint32_t arg0, uint32_t arg1)
{
	lprintf("CORE=%d:task8\n", CPUID_get());
	for (int ix=0;;ix++ ) {
		task_tsleep(MSEC(500));
		flag_set(flag, 0x0001);
		lprintf("CORE=%d:flag_set done\n", CPUID_get());
		task_set_affinity(ix%CPU_NUM);
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
		lprintf("CORE=%d:TASK1:MSGQ1:send:%d\n", CPUID_get(), ix);
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
		lprintf("CORE=%d:TASK2:MSG1:recv:%d\n", CPUID_get(), ix);
		cmd2.cmd = cmd.cmd;
		cmd2.param5 = cmd.param1;
		cmd2.param6 = cmd.param2;
		lprintf("CORE=%d:TASK2:CMD2:%d %d %d\n", CPUID_get(), cmd2.cmd, cmd2.param5, cmd2.param6);
		task_tsleep(MSEC(5));
	}
}

void task_msgq_3(void)
{
	int ix;
	MsgCmd cmd;
	for ( ix=0;; ix++ ) {
		msgq_recv(msgq1, &cmd, sizeof(cmd));
		lprintf("CORE=%d:TASK3:MSG2:recv:%d\n", CPUID_get(), ix);
		lprintf("CORE=%d:TASK3:CMD2:%d %d %d\n", CPUID_get(), cmd.cmd, cmd.param1, cmd.param2);
	}
}


TaskCreateInfo	task_info[] = {
		{"TASK01", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 1024, 0, 5, (void*)0},
		{"TASK02", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task2, 1024, 0, 6, (void*)0},
		{"TASK11", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 1024, 0, 5, (void*)1},
		{"TASK12", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task2, 1024, 0, 6, (void*)1},
		{"TASK02", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task2, 1024, 0, 6, (void*)0},
		{"TASK11", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 1024, 0, 5, (void*)1},
		{"TASK12", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task2, 1024, 0, 6, (void*)1},
		{"TASK13", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task3, 1024, 0, 7, (void*)0},
		{"TASK14", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task4, 1024, 0, 8, (void*)1},
		{"TASK14", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task6, 1024, 0, 8, (void*)0},
		{"TASK13", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task5, 1024, 0, 7, (void*)0},
		{"TASK14", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task6, 1024, 0, 8, (void*)0},
		{"TASK13", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task5, 1024, 0, 7, (void*)0},
		{"TASK14", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task6, 1024, 0, 8, (void*)0},
		{"TASK13", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task5, 1024, 0, 7, (void*)0},
		{"TASK15", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task7, 1024, 0, 8, (void*)0},
		{"TASK16", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task8, 1024, 0, 7, (void*)0},
		{"TASKm1", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_1, 1024, 0, 8, (void*)0},
		{"TASKm1", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_1, 1024, 0, 8, (void*)0},
		{"TASKm2", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_2, 1024, 0, 7, (void*)0},
		{"TASKm3", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_3, 1024, 0, 7, (void*)0},
		{"TASKm1", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_1, 1024, 0, 8, (void*)0},
		{"TASKm1", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_1, 1024, 0, 8, (void*)0},
		{"TASKm2", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_2, 1024, 0, 7, (void*)0},
		{"TASKm3", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_3, 1024, 0, 7, (void*)0},
		{"TASKm1", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_1, 1024, 0, 8, (void*)0},
		{"TASKm1", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_1, 1024, 0, 8, (void*)0},
		{"TASKm2", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_2, 1024, 0, 7, (void*)0},
		{"TASKm3", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_3, 1024, 0, 7, (void*)0},
		{"TASK01", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)0},
		{"TASK01", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)1},
		{"TASK01", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)2},
		{"TASK01", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task1, 1024, 0, 5, (void*)3},
		{"TASK01", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 1024, 0, 5, (void*)4},
		{"TASK01", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task1, 1024, 0, 5, (void*)5},
		{"TASK01", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task1, 4096, 0, 5, (void*)6},
		{"TASK01", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task1, 1024, 0, 5, (void*)7},
		{"TASK02", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task2, 1024, 0, 6, (void*)0},
		{"TASK11", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task1, 1024, 0, 5, (void*)1},
		{"TASK12", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task2, 1024, 0, 6, (void*)1},
		{"TASK02", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task2, 1024, 0, 6, (void*)0},
		{"TASK11", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task1, 1024, 0, 5, (void*)1},
		{"TASK12", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task2, 1024, 0, 6, (void*)1},
		{"TASK13", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task3, 1024, 0, 7, (void*)0},
		{"TASK14", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task4, 1024, 0, 8, (void*)1},
		{"TASK14", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task6, 1024, 0, 8, (void*)0},
		{"TASK13", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task5, 1024, 0, 7, (void*)0},
		{"TASK14", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task6, 1024, 0, 8, (void*)0},
		{"TASK13", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task5, 1024, 0, 7, (void*)0},
		{"TASK14", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task6, 1024, 0, 8, (void*)0},
		{"TASK13", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task5, 1024, 0, 7, (void*)0},
		{"TASK15", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task7, 1024, 0, 8, (void*)0},
		{"TASK16", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task8, 1024, 0, 7, (void*)0},
		{"TASKm1", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_1, 1024, 0, 8, (void*)0},
		{"TASKm1", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_1, 1024, 0, 8, (void*)0},
		{"TASKm2", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_2, 1024, 0, 7, (void*)0},
		{"TASKm3", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_3, 1024, 0, 7, (void*)0},
		{"TASKm1", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_1, 1024, 0, 8, (void*)0},
		{"TASKm1", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_1, 1024, 0, 8, (void*)0},
		{"TASKm2", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_2, 1024, 0, 7, (void*)0},
		{"TASKm3", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_3, 1024, 0, 7, (void*)0},
		{"TASKm1", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_1, 1024, 0, 8, (void*)0},
		{"TASKm1", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_1, 1024, 0, 8, (void*)0},
		{"TASKm2", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_2, 1024, 0, 7, (void*)0},
		{"TASKm3", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_3, 1024, 0, 7, (void*)0},
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

	task_sleep();
}
