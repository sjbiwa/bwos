/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * idle.c
 *
 *  Created on: 2015/12/26
 *      Author: biwa
 */
#include "kernel.h"
#include "armv8.h"
#include "armv8reg.h"
#include "gicv3reg.h"

/*
 * 	idle_task
 * 	  call in DISABLE_IRQ
 */
void idle_task(void)
{
	for (;;) {
		irq_disable();
		//__wfi();
		irq_enable();
	}
}

void fault_handler(uint32_t id, uint32_t* sp)
{
	for (;;);
}
