/*
 * timer.c
 *
 *  Created on: 2012/12/07
 *      Author: biwa
 */
#include "common.h"
#include "mpcore.h"
#include "task.h"
#include "timer.h"

uint32_t		tick_count = 0L;

static void
timer_handler(uint32_t irqno, void* info)
{
	iowrite32(PTM_INTSTATUS, 0x00000001);
	tick_count++;
	//tprintf("tick=%d\n", tick_count);
	task_tick();
}

void
timer_init(void)
{
	iowrite32(PTM_LOAD, 1500000);
	iowrite32(PTM_COUNTER, 1500000);
	iowrite32(PTM_INTSTATUS, 0x00000001);
	iowrite32(PTM_CONTROL, 0x00000007);
	irq_add_handler(29, timer_handler, NULL);
	irq_set_enable(29);
}
