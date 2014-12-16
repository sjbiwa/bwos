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
extern void task1(void);
extern void task2(void);
extern void task3(void);
extern void task4(void);
extern void task5(void);

TaskStruct		task_info[] = {
		{TASK_RESERVE, "TASK1", task1, 0, 1024, 1},
		{TASK_RESERVE, "TASK2", task2, 0, 1024, 1},
		{TASK_RESERVE, "TASK3", task3, 0, 1024, 1},
		{TASK_RESERVE, "TASK4", task4, 0, 1024, 1},
		{TASK_RESERVE, "TASK5", task5, 0, 1024, 2},
};

const uint32_t task_info_num = arrayof(task_info);
