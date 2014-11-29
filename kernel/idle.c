/*
 * idle.c
 *
 *  Created on: 2012/12/09
 *      Author: biwa
 */
#include "common.h"
#include "arm.h"
#include "task.h"

/*
 * 	idle_task
 * 	  call in DISABLE_IRQ
 */
void
idle_task(void)
{
	for (;;) {
		irq_disable();
		__wfi();
		irq_enable();
	}
}
