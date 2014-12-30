/*
 * timer.c
 *
 *  Created on: 2012/12/07
 *      Author: biwa
 */
#include "common.h"
#include "mpcore.h"
#include "cp15reg.h"
#include "task.h"
#include "timer.h"

#if defined(HAVE_CP15_TIMER)
static uint64_t	compare_reg_value = 0;
#endif

static void
timer_handler(uint32_t irqno, void* info)
{
#if defined(HAVE_SCU_LOCAL_TIMER)
	iowrite32(PTM_INTSTATUS, 0x00000001);
#elif defined(HAVE_CP15_TIMER)
	compare_reg_value += (CNTFRQ_VALUE/1000)*TICK_CYCLE;
	CNTP_CVAL_set(compare_reg_value);
#endif
	task_tick();
}

void
timer_init(void)
{
#if defined(HAVE_SCU_LOCAL_TIMER)
	iowrite32(PTM_LOAD, 1500000);
	iowrite32(PTM_COUNTER, 1500000);
	iowrite32(PTM_INTSTATUS, 0x00000001);
	iowrite32(PTM_CONTROL, 0x00000007);
#elif defined(HAVE_CP15_TIMER)
	CNTP_CVAL_set(compare_reg_value);
	CNTP_CTL_set(0x00000001);
#endif
	irq_add_handler(29, timer_handler, NULL);
	irq_set_enable(29);
}
