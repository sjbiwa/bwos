/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "api.h"

static FlagStruct*	flag1;
static FlagStruct*	flag2;

#define	WF_BIT0		(0x0001<<0)
#define	WF_BIT1		(0x0001<<1)
#define	WF_BIT2		(0x0001<<2)
#define	WF_BIT3		(0x0001<<3)

void task1(void)
{
	task_tsleep(SEC(10));
	lprintf("TASK1:send1\n");
	flag_set(flag2, WF_BIT0);

	task_tsleep(SEC(20));

	lprintf("TASK1:send2\n");
	flag_set(flag2, WF_BIT1);

	task_sleep();
}

void task2(void)
{
	task_tsleep(SEC(5));
	lprintf("TASK2:send1\n");
	flag_set(flag2, WF_BIT1);

	task_tsleep(SEC(25));

	lprintf("TASK2:send2\n");
	flag_set(flag2, WF_BIT0);

	task_sleep();
}

void task3(void)
{
	uint32_t ret_pattern;
	int ret;

	lprintf("TASK3:WAIT flag\n");
	ret = flag_wait(flag2, WF_BIT0|WF_BIT1, FLAG_AND|FLAG_CLR, &ret_pattern);
	lprintf("TASK3:COMPLETE:%d:%08X\n", ret, ret_pattern);

	task_tsleep(SEC(15));

	ret = flag_wait(flag2, WF_BIT0|WF_BIT1, FLAG_OR, &ret_pattern);
	lprintf("TASK3:COMPLETE:%d:%08X\n", ret, ret_pattern);

	task_tsleep(SEC(15));

	lprintf("TASK3:WAIT flag\n");
	ret = flag_wait(flag2, WF_BIT0|WF_BIT1, FLAG_OR|FLAG_CLR, &ret_pattern);
	lprintf("TASK3:COMPLETE:%d:%08X\n", ret, ret_pattern);

	ret = flag_twait(flag2, WF_BIT0|WF_BIT1, FLAG_OR|FLAG_CLR, &ret_pattern, SEC(10));
	lprintf("TASK3:COMPLETE:%d:%08X\n", ret, ret_pattern);

	ret = flag_twait(flag2, WF_BIT0|WF_BIT1, FLAG_OR|FLAG_CLR, &ret_pattern, TMO_POLL);
	lprintf("TASK3:COMPLETE:%d:%08X\n", ret, ret_pattern);

	flag_set(flag2, WF_BIT0);
	flag_set(flag2, WF_BIT1);
	flag_set(flag2, WF_BIT2);
	flag_set(flag2, WF_BIT3);
	ret = flag_twait(flag2, WF_BIT0|WF_BIT1, FLAG_OR|FLAG_CLR, &ret_pattern, TMO_POLL);
	lprintf("TASK3:COMPLETE:%d:%08X\n", ret, ret_pattern);

	ret = flag_twait(flag2, WF_BIT0|WF_BIT1|WF_BIT2|WF_BIT3, FLAG_OR, &ret_pattern, TMO_POLL);
	lprintf("TASK3:COMPLETE:%d:%08X\n", ret, ret_pattern);

	flag_set(flag2, WF_BIT0);
	flag_set(flag2, WF_BIT1);
	flag_set(flag2, WF_BIT2);
	flag_set(flag2, WF_BIT3);
	ret = flag_twait(flag2, WF_BIT0|WF_BIT1, FLAG_OR|FLAG_BITCLR, &ret_pattern, TMO_POLL);
	lprintf("TASK3:COMPLETE:%d:%08X\n", ret, ret_pattern);

	ret = flag_twait(flag2, WF_BIT2|WF_BIT3, FLAG_OR|FLAG_BITCLR, &ret_pattern, TMO_POLL);
	lprintf("TASK3:COMPLETE:%d:%08X\n", ret, ret_pattern);

	ret = flag_twait(flag2, WF_BIT0|WF_BIT1|WF_BIT2|WF_BIT3, FLAG_OR, &ret_pattern, TMO_POLL);
	lprintf("TASK3:COMPLETE:%d:%08X\n", ret, ret_pattern);

	flag_set(flag2, WF_BIT0);
	flag_set(flag2, WF_BIT1);
	flag_set(flag2, WF_BIT2);
	flag_set(flag2, WF_BIT3);
	flag_clear(flag2, WF_BIT0|WF_BIT2);

	ret = flag_twait(flag2, WF_BIT0|WF_BIT1|WF_BIT2|WF_BIT3, FLAG_OR, &ret_pattern, TMO_POLL);
	lprintf("TASK3:COMPLETE:%d:%08X\n", ret, ret_pattern);

	task_sleep();
}

/* configuration task */
TaskStruct*		task_struct[16];

TaskCreateInfo	task_info[] = {
		{"TASK1", TASK_ACT|TASK_FPU, task1, 0, 1024, 1024, 7},
		{"TASK2", TASK_ACT|TASK_FPU, task2, 0, 1024, 1024, 7},
		{"TASK3", TASK_ACT|TASK_FPU, task3, 0, 1024, 1024, 6},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_create(&task_struct[ix], &task_info[ix]);
	}

	/* 共有リソース初期化 */
	flag_create(&flag1);
	flag_create(&flag2);

	task_sleep();
}
