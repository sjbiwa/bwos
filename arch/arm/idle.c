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

void fault_handler(uint32_t id, uint32_t* sp)
{
	/****************/
	/* SP SPSR		*/
	/*    SP_usr	*/
	/*	  SP_svc	*/
	/*	  SP_irq	*/
	/*    R0		*/
	/*	  ...		*/
	/*	  R12		*/
	/*	  LR(ret-PC)*/
	/****************/
	switch (id) {
	case 0:
		tprintf("\nundef\n");
		break;

	case 1:
		tprintf("\nprefetch abort\n");
		break;

	case 2:
		tprintf("\ndata abort\n");
		break;

	default:
		tprintf("\nunknown fault\n");
		break;
	}

	tprintf(" R0:%08X  R1:%08X  R2:%08X  R3:%08X\n", sp[4], sp[5], sp[6], sp[7]);
	tprintf(" R4:%08X  R5:%08X  R6:%08X  R7:%08X\n", sp[8], sp[9], sp[10], sp[11]);
	tprintf(" R8:%08X  R9:%08X R10:%08X R11:%08X\n", sp[12], sp[13], sp[14], sp[15]);
	tprintf("R12:%08X  LR:%08X CPSR:%08X\n", sp[16], sp[17], sp[0]);
	tprintf("SP_usr:%08X SP_svc:%08X SP_irq:%08X\n", sp[1], sp[2], sp[3]);

	tprintf("DFSR:%08X DFAR:%08X IFSR:%08X IFAR:%08X\n", DFSR_get(), DFAR_get(), IFSR_get(), IFAR_get());
	for (;;);
}
