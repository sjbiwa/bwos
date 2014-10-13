/*
 * idle.c
 *
 *  Created on: 2012/12/09
 *      Author: biwa
 */
#include "common.h"
#include "arm.h"
#include "task.h"

extern Link			task_time_out_list;

/*
 * 	idle_task
 * 	  call in DISABLE_IRQ
 */
void
idle_task(void)
{
	Link*			link;
	TaskStruct*		task;
	for (;;) {
		irq_enable();
		irq_disable();
		__wfi();
	}
}
