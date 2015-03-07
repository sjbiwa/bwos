/*
 * idle.c
 *
 *  Created on: 2012/12/09
 *      Author: biwa
 */
#include "kernel.h"
#include "arm.h"
#include "cp15reg.h"

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
		uint64_t t1 = CNTPCT_get();
		uint64_t t2 = CNTP_CVAL_get();
		irq_enable();
	}
}
