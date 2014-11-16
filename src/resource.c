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

static uint8_t		stack[15][1024] __attribute__((aligned(16)));

TaskStruct		task_info[] = {
		{{0,0}, task1, stack[0], 1024, "TASK1", 1},
		{{0,0}, task2, stack[1], 1024, "TASK2", 1},
		{{0,0}, task3, stack[2], 1024, "TASK3", 1},
		{{0,0}, task4, stack[3], 1024, "TASK4", 1},
		{{0,0}, task5, stack[4], 1024, "TASK5", 1},
		{{0,0}, task1, stack[5], 1024, "TASK6", 1},
		{{0,0}, task2, stack[6], 1024, "TASK7", 1},
		{{0,0}, task3, stack[7], 1024, "TASK8", 1},
		{{0,0}, task4, stack[8], 1024, "TASK9", 1},
		{{0,0}, task5, stack[9], 1024, "TASK10", 1},
		{{0,0}, task1, stack[10], 1024, "TASK11", 1},
		{{0,0}, task2, stack[11], 1024, "TASK12", 1},
		{{0,0}, task3, stack[12], 1024, "TASK13", 1},
		{{0,0}, task4, stack[13], 1024, "TASK14", 1},
		{{0,0}, task5, stack[14], 1024, "TASK15", 1},
};
const uint32_t task_info_num = arrayof(task_info);
