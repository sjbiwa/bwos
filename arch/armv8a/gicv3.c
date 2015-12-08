/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

#include "common.h"
#include "armv8reg.h"
#include "gicv3reg.h"

#define	TVAL_VALUE		(0x700000u)
extern void wait(uint32_t t);

void c_irq_handler()
{
	uint32_t intid = ICC_IAR1_EL1_get() & 0x00FFFFFF;
	ICC_EOIR1_EL1_set(intid);
	if ( (0 <= intid) && (intid <= 15) ) {
		tprintf("SGI:%d\n", intid);
	}
	else if ( intid == 30 ) {
		tprintf("TIMER\n");
		CNTP_TVAL_EL0_set(TVAL_VALUE);
	}
}

static void gicd_wait_rwp(void)
{
	while (ioread32(GICD_CTLR) & (0x01<<31));
}

static void gicr_wait_rwp(void)
{
	while (ioread32(GICD_CTLR) & (0x01<<3));
}

void gic_init(void)
{
	int ix;
	uint32_t intr_lines = ioread32(GICD_TYPER);
	tprintf("GICD_TYPER=%08X\n", intr_lines);
	intr_lines = 32 * ((intr_lines & 0x1f) + 1);
	if ( 1021 < intr_lines ) {
		intr_lines = 1021;
	}
	tprintf("INTR_LINES=%d\n", intr_lines);

	tprintf("GICR_TYPER=%08X\n", ioread32(GICR_TYPER));
	/************************************************/
	/*	GIC distributor setting	 (Main Core only)	*/
	/************************************************/
	/* GICD control disable */
	iowrite32(GICD_CTLR, 0x00000030);
	gicd_wait_rwp();
	/* SPI GROUP設定 (secure-group1) */
	for ( ix = 1; ix < ((intr_lines+31)/32); ix++ ) {
		iowrite32n(GICD_IGROUPR, ix, 0x00000000);
		iowrite32n(GICD_IGRPMODR, ix, 0xffffffff);
	}
	/* SPI Clear-Enable */
	for ( ix = 1; ix < ((intr_lines+31)/32); ix++ ) {
		iowrite32n(GICD_ICENABLER, ix, 0xffffffff);
	}
	gicd_wait_rwp();
	/* SPI Clear-Pending */
	for ( ix = 1; ix < ((intr_lines+31)/32); ix++ ) {
		iowrite32n(GICD_ICPENDR, ix, 0xffffffff);
	}
	/* SPI Clear-Active */
	for ( ix = 1; ix < ((intr_lines+31)/32); ix++ ) {
		iowrite32n(GICD_ICACTIVER, ix, 0xffffffff);
	}
	/* SPI Priority */
	for ( ix = 8; ix < ((intr_lines+3)/4); ix++ ) {
		iowrite32n(GICD_IPRIORITYR, ix, 0x00000000);
	}
	/* SPI Configuration (Trigger-Mode) */
	for ( ix = 2; ix < ((intr_lines+15)/16); ix++ ) {
		iowrite32n(GICD_ICFGR, ix, 0x00000000);
	}
	/* SPI NonSecure Access Control */
	for ( ix = 2; ix < ((intr_lines+15)/16); ix++ ) {
		iowrite32n(GICD_NSACR, ix, 0x00000000);
	}
	/* SPI Routing (Aff=CORE0) */
	for ( ix = 32; ix < intr_lines; ix++ ) {
		iowrite32n(GICD_IROUTER, ix, 0x00000000);
	}
	/* GICD control enable */
	iowrite32(GICD_CTLR, 0x00000037);
	gicd_wait_rwp();


	/************************************************/
	/*	GIC Redistributor setting (All Core)		*/
	/************************************************/
	iowrite32(GICR_STATUSR, 0xf);

	iowrite32(GICR_WAKER, ioread32(GICR_WAKER) & ~(0x01<<1));
	while (ioread32(GICR_WAKER) & (0x01<<2));
	tprintf("GICR WAKEUP\n");

	iowrite32(GICR_CTLR, 0x00000000);
	gicr_wait_rwp();
	iowrite32(GICR_IGROUPR0, 0x00000000);
	iowrite32(GICR_IGRPMODR0, 0xffffffff);

	iowrite32(GICR_ICENABLER0, 0xffffffff);
	gicr_wait_rwp();
	iowrite32(GICR_ICPENDR0, 0xffffffff);
	iowrite32(GICR_ICACTIVER0, 0xffffffff);
	for ( ix = 0; ix < 8; ix++ ) {
		iowrite32n(GICR_IPRIORITYR, ix, 0x00000000);
	}
	iowrite32(GICR_ICFGR0, 0x00000000);
	iowrite32(GICR_ICFGR1, 0x00000000);
	iowrite32(GICR_NSACR, 0x00000000);


	/************************************************/
	/*	GIC CPU interface setting (All Core)		*/
	/************************************************/
	ICC_CTLR_EL1_set(0x00000000);
	ICC_BPR0_EL1_set(0);
	ICC_BPR1_EL1_set(0);
	ICC_PMR_EL1_set(0xff);
	ICC_IGRPEN1_EL1_set(0x00000001);

	tprintf("GIC init complete:%08X\n", (uint32_t)ICC_SRE_EL1_get());
}

void timer_test(void)
{
	/* SGI enable */
	iowrite32(GICR_ISENABLER0, 0x4000ffff);

	/* timer init */
	CNTP_CTL_EL0_set(0x00000000);
	CNTP_TVAL_EL0_set(TVAL_VALUE);
	CNTP_CTL_EL0_set(0x00000001);

	DAIFClr_seti(0xf);
	for (uint32_t ix=0;; ix++) {
		ICC_SGI1R_EL1_set(0x000000000000FFFFL | ((ix&0xf) << 24));
		wait(5000000);
	}
}
