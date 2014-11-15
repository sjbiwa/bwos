/*
 * main.c
 *
 *  Created on: 2012/12/01
 *      Author: biwa
 */
#include "common.h"
#include "arm.h"
#include "cp15reg.h"

extern TaskStruct	task_info[];
extern uint32_t		task_info_num;

int main(void)
{
	tprintf("main start\n");
	task_init(task_info, task_info_num);
	irq_init();
	timer_init();
	schedule();
}
