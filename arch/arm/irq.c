/*
 * irq.c
 *
 *  Created on: 2012/12/07
 *      Author: biwa
 */
#include "kernel.h"
#include "mpcore.h"

uint32_t _irq_level[CPU_NUM]; /* 多重割り込みレベル */

static struct {
	IRQ_HANDLER		handler;
	void*			info;
} irq_action[IRQ_NUM];;

/*
 * 	c_handler
 */
void c_handler(uint32_t* sp, uint32_t pc, uint32_t sr)
{
	uint32_t		ack;
	uint32_t		irqno;
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
OSAPISTUB void __irq_add_handler(uint32_t irqno, uint32_t irq_attr, IRQ_HANDLER func, void* info)
{
	if ( irqno < IRQ_NUM ) {
		irq_action[irqno].handler = func;
		irq_action[irqno].info = info;
		if ( 32 <= irqno ) {
			/* Set Target Register */
			uint32_t cpuid = CPU_GET(irq_attr);
			if ( cpuid == CPU_SELF ) {
				cpuid = CPUID_get();
			}
			iowrite8(GICD_ITARGETSR+irqno, 0x01u<<cpuid);
		}
		sync_barrier();
	}
}

/*
 * 	irq_set_enable
 */
OSAPISTUB void __irq_set_enable(uint32_t irqno, int setting)
{
	uint32_t		off;
	uint32_t		bit;

	off = irqno / 32;
	bit = irqno % 32;
	if ( setting == IRQ_ENABLE ) {
		iowrite32(GICD_ISENABLER+off*4, 0x01<<bit);
	}
	else {
		iowrite32(GICD_ICENABLER+off*4, 0x01<<bit);
	}
}

/*
 * 	irq_get_enable
 */
OSAPISTUB int __irq_get_enable(uint32_t irqno)
{
	uint32_t		off;
	uint32_t		bit;

	off = irqno / 32;
	bit = irqno % 32;
	int ret = (ioread32(GICD_ICENABLER+off*4) & (0x01<<bit)) ? IRQ_ENABLE : IRQ_DISABLE;
	return ret;
}

/*
 * 	arch_irq_init
 */
void
arch_irq_init(uint32_t cpuid)
{
	int			ix;
	uint32_t		irq_num;

	if ( cpuid == MASTER_CPU_ID ) {
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

		/* Interrupt Configuration Register (SGI+PPI) */
		for (ix=0; ix<8; ix++) {
			iowrite32(GICD_ICFGR+ix*4, 0x00000000);
		}

		/* Set Target Register (CORE0) */
		for (ix=8; ix<(irq_num/4); ix++) {
			iowrite32(GICD_ITARGETSR+ix*4, 0x01010101);
		}

		/* Interrupt Configuration Register (SharedPeriheralInterrupt) */
		for (ix=2; ix<(irq_num/16); ix++) {
			iowrite32(GICD_ICFGR+ix*4, 0x55555555);
		}
		iowrite32(GICD_CTLR, 0x00000001); // enable
	}
	else {
		/* Clear-Enable Register */
		iowrite32(GICD_ICENABLER+0, 0xffffffff);

		/* Clear-Pending Register */
		iowrite32(GICD_ICPENDR+0, 0xffffffff);

		/* Set Priority Register (0x00:Highest) (ID0-ID31) */
		for (ix=0; ix<8; ix++) {
			iowrite32(GICD_IPRIORITYR+ix*4, 0x00000000);
		}

		/* Interrupt Configuration Register (SGI+PPI) */
		for (ix=0; ix<8; ix++) {
			iowrite32(GICD_ICFGR+ix*4, 0x00000000);
		}
	}

	/* CPU Interface Register Initialize */
	iowrite32(GICC_CTLR, 0x00000000); // disable
	/* Priority Register */
	iowrite32(GICC_PMR, 0xff);
	/* BinaryPoint Register */
	iowrite32(GICC_BPR, 0x03);

	iowrite32(GICC_CTLR, 0x00000001); // enable
}
