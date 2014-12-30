/*
 * resource.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "api.h"
#include "resource.h"

/* configuration task */
extern void task1(void);
extern void task2(void);
extern void task3(void);
extern void task4(void);
extern void task5(void);

TaskStruct		task_info[] = {
		{TASK_RESERVE, "TASK1", TASK_FPU, task1, 0, 1024, 4, 0, 1024},
		{TASK_RESERVE, "TASK2", TASK_FPU, task2, 0, 1024, 4, 0, 1024},
		{TASK_RESERVE, "TASK3", TASK_FPU, task3, 0, 1024, 4, 0, 1024},
		{TASK_RESERVE, "TASK4", 0, task4, 0, 1024, 5, 0, 1024},
		{TASK_RESERVE, "TASK5", 0, task5, 0, 1024, 7, 0, 1024},
};

const uint32_t task_info_num = arrayof(task_info);
