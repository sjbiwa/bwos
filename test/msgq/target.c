/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "bwos.h"

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

void task1(void)
{
	int ix;
	void* ptr;
	MsgCmd	cmd;
	for ( ix=1; ix < 100000; ix++ ) {
		cmd.cmd = ix;
		cmd.param1 = ix*10;
		cmd.param2 = ix*100;
		msgq_send(msgq1, &cmd, sizeof(cmd));
		task_tsleep(MSEC(200));
//		lprintf("TASK1:MSGQ1:send:%d\n", ix);
	}
	task_sleep();
}

void task2(void)
{
	int ix;
	MsgCmd cmd;
	MsgCmd2 cmd2;
	for ( ix=0;; ix++ ) {
		msgq_recv(msgq1, &cmd, sizeof(cmd));
//		lprintf("TASK2:MSG1:recv:%d\n", ix);
		cmd2.cmd = cmd.cmd;
		cmd2.param5 = cmd.param1;
		cmd2.param6 = cmd.param2;
		lprintf("TASK2:CMD2:%d %d %d\n", cmd2.cmd, cmd2.param5, cmd2.param6);
	}
}

void task3(void)
{
	int ix;
	MsgCmd cmd;
	for ( ix=0;; ix++ ) {
		msgq_recv(msgq1, &cmd, sizeof(cmd));
//		lprintf("TASK3:MSG2:recv:%d\n", ix);
		lprintf("TASK3:CMD2:%d %d %d\n", cmd.cmd, cmd.param1, cmd.param2);
	}
}

/* configuration task */
int		task_struct[16];

TaskCreateInfo	task_info[] = {
		{"TASK1", TASK_ACT|TASK_FPU, task1, 1024, 1024, 7},
		{"TASK2", TASK_ACT|TASK_FPU, task2, 1024, 1024, 6},
		{"TASK3", TASK_ACT|TASK_FPU, task3, 1024, 1024, 5},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	/* 共有リソース初期化 */
	msgq1 = msgq_create(128);
	msgq2 = msgq_create(1024);

	task_sleep();
}
