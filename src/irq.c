/*
 * irq.c
 *
 *  Created on: 2012/12/07
 *      Author: biwa
 */
#include "common.h"
#include "mpcore.h"
#include "task.h"

typedef	void (*IRQ_HANDLER)(uint32_t irqno, void* info);

static struct {
	IRQ_HANDLER		handler;
	void*			info;
} irq_action[IRQ_NUM];;

/*
 * 	c_handler
 */
void
c_handler(uint32_t* sp, uint32_t pc, uint32_t sr)
{
	uint		ack;
	uint		irqno;
	/* Get Acknowledge */
	ack = ioread32(GICC_IAR);
	irqno = ack & 0x3ff;
	//tprintf("c_handler:SP=%08X PC=%08X SR=%08X ACK=%08X\n", sp, pc, sr, ack);

	if ( (irqno < IRQ_NUM) && irq_action[irqno].handler ) {
		(*irq_action[irqno].handler)(irqno, irq_action[irqno].info);
	}
	/* EOI */
	iowrite32(GICC_EOIR, ack);
}

/*
 * 	irq_add_handler
 */
void
irq_add_handler(uint irqno, IRQ_HANDLER func, void* info)
{
	if ( irqno < IRQ_NUM ) {
		irq_action[irqno].handler = func;
		irq_action[irqno].info = info;
	}
}

/*
 * 	irq_set_enable
 */
void
irq_set_enable(uint irqno)
{
	uint		off;
	uint		bit;

	off = irqno / 32;
	bit = irqno % 32;
	iowrite32(GICD_ISENABLER+off, 0x01<<bit);
}

/*
 * 	irq_set_disable
 */
 void
irq_set_disable(uint irqno)
{
	uint		off;
	uint		bit;

	off = irqno / 32;
	bit = irqno % 32;
	iowrite32(GICD_ICENABLER+off, 0x01<<bit);
}

/*
 * 	irq_init
 */
void
irq_init(void)
{
	int			ix;
	uint		irq_num;

	/* DIC initialize */
	irq_num = (ioread32(GICD_TYPER) & 0x1f) << 5;
	tprintf("IRQ NUM=%d\n", irq_num);

	/* Distributor register Initialize */
	iowrite32(GICD_CTLR, 0x00000000); // disable

	/* Clear-Enable Register */
	for (ix=0; ix<(irq_num/32); ix++) {
		iowrite32(GICD_ICENABLER+ix*4, 0xffffffff);
	}

	/* Clear-Pending Register */
	for (ix=0; ix<(irq_num/32); ix++) {
		iowrite32(GICD_ICPENDR+ix*4, 0xffffffff);
	}

#if 0
	/* Clear-Active Register */
	for (ix=0; ix<(irq_num/32); ix++) {
		iowrite32(GICD_ICACTIVER+ix*4, 0xffffffff);
	}
#endif

	/* Set Priority Register (0x00:Highest) */
	for (ix=0; ix<(irq_num/4); ix++) {
		iowrite32(GICD_IPRIORITYR+ix*4, 0x00000000);
	}

	/* Set Target Register (CORE0) */
	for (ix=2; ix<(irq_num/4); ix++) {
		iowrite32(GICD_ITARGETSR+ix*4, 0x01010101);
	}

	/* Interrupt Configuration Register (SGI+PPI) */
	for (ix=0; ix<2; ix++) {
		iowrite32(GICD_ICFGR+ix*4, 0x00000000);
	}
	/* Interrupt Configuration Register (SharedPeriheralInterrupt) */
	for (ix=2; ix<(irq_num/16); ix++) {
		iowrite32(GICD_ICFGR+ix*4, 0x55555555);
	}
	iowrite32(GICD_CTLR, 0x00000001); // enable


	/* CPU Interface Register Initialize */
	iowrite32(GICC_CTLR, 0x00000000); // disable
	/* Priority Register */
	iowrite32(GICC_PMR, 0xff);
	/* BinaryPoint Register */
	iowrite32(GICC_BPR, 0x03);

	iowrite32(GICC_CTLR, 0x00000001); // enable
}
