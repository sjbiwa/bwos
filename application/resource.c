/*
 * resource.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "common.h"
#include "task.h"
#include "resource.h"

/* configuration task */
extern void task0(void);
extern void task1(void);
extern void task2(void);
extern void task3(void);
extern void task4(void);
extern void task5(void);

static uint8_t		stack[15][1024] __attribute__((aligned(16)));

TaskStruct		task_info[] = {
		{TASK_RESERVE, "TASK1", task1, stack[0], 1024, 1},
		{TASK_RESERVE, "TASK0", task0, stack[1], 1024, 1},
//		{TASK_RESERVE, "TASK2", task2, stack[1], 1024, 1},
//		{TASK_RESERVE, "TASK3", task3, stack[2], 1024, 1},
//		{TASK_RESERVE, "TASK4", task4, stack[3], 1024, 1},
//		{TASK_RESERVE, "TASK5", task5, stack[4], 1024, 2},
};

const uint32_t task_info_num = arrayof(task_info);
