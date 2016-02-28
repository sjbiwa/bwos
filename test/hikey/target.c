#include "bwos.h"

#if USE_SMP != 1
#define	task_set_affinity(...)
#endif
#define	task_set_affinity(...)

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
static volatile int msgq;


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

void task_dummy(uint32_t arg0, uint32_t arg1)
{
	for (uint32_t ix = CPUID_get()+1;;ix++ ) {
		lprintf("CORE=%d:task2:%d\n", CPUID_get(), ix);
		task_set_affinity(ix%CPU_NUM);
		task_tsleep(MSEC(100+CPUID_get()+arg0));
	}
}

void task_msgq_1(void)
{
	int ix;
	void* ptr;
	MsgCmd	cmd;
	for ( ix=1;; ix++ ) {
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
		{"TASK01", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_1, 5024, 0, 5, (void*)0},
		{"TASK02", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_2, 5024, 0, 6, (void*)1},
		{"TASK13", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_2, 5024, 0, 7, (void*)2},
		{"TASK14", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task_msgq_3, 5024, 0, 8, (void*)3},

		{"TASK01", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task_dummy, 5024, 0, 5, (void*)10},
		{"TASK01", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task_dummy, 5024, 0, 5, (void*)20},
		{"TASK01", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task_dummy, 5024, 0, 5, (void*)30},
		{"TASK01", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task_dummy, 5024, 0, 5, (void*)40},
		{"TASK01", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task_dummy, 5024, 0, 5, (void*)50},
		{"TASK01", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task_dummy, 5024, 0, 5, (void*)60},
		{"TASK01", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task_dummy, 5024, 0, 5, (void*)70},
		{"TASK01", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task_dummy, 5024, 0, 5, (void*)80},
};

void main_task(void)
{
	int ix;
	mutex = mutex_create();
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
