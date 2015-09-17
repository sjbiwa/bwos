/*
 * Copyright (C) 2015 syujibiwa All rights reserved.
 */

/*
 * idle.c
 *
 *  Created on: 2015/07/01
 *      Author: biwa
 */
#include "kernel.h"
#include "arm.h"

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

void fault_handler(uint32_t id, uint32_t* sp)
{
	for (;;);
}

void _nmi_entry(void)
{
	fault_handler(0, 0);
}
void _hard_fault_entry(void)
{
	fault_handler(0, 0);
}
void _mem_manage_entry(void)
{
	fault_handler(0, 0);
}
void _bus_fault_entry(void)
{
	fault_handler(0, 0);
}
void _usage_fault_entry(void)
{
	fault_handler(0, 0);
}

void _debug_entry(void)
{
	fault_handler(0, 0);
}

void _systick_entry(void)
{
	timer_handler(0,0);
}
