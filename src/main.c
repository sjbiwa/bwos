/*
 * main.c
 *
 *  Created on: 2012/12/01
 *      Author: biwa
 */
#include "common.h"

extern TaskStruct	task_info[];
extern uint32_t		task_info_num;

int main(void)
{
	int ix;
	tprintf("main start\n");
	task_init();
	for (ix=0; ix<task_info_num; ix++) {
		task_create(&task_info[ix]);
	}
	irq_init();
	timer_init();
	schedule();
}
