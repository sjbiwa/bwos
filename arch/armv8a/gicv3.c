/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

#include "kernel.h"
#include "armv8reg.h"
#include "gicv3reg.h"

static struct {
	IRQ_HANDLER		handler;
	void*			info;
} irq_action[IRQ_NUM];;

static void gicd_wait_rwp(void)
{
	while (ioread32(GICD_CTLR) & (0x01<<31));
}

static void gicr_wait_rwp(uint8_t* rd_base)
{
	while (ioread32(rd_base+GICR_CTLR) & (0x01<<3));
}

void c_handler(uint32_t* sp, uint32_t pc, uint32_t sr)
{
	uint32_t intid = ICC_IAR1_EL1_get() & 0x00FFFFFF;
	//tprintf("c_handler:%d: IRQ=%d\n", CPUID_get(), intid);
	if ( (intid < IRQ_NUM) && irq_action[intid].handler ) {
		(*irq_action[intid].handler)(intid, irq_action[intid].info);
	}
	ICC_EOIR1_EL1_set(intid);
}

/*
 * 	irq_add_handler
 */
OSAPISTUB void __irq_add_handler(uint32_t irqno, IRQ_HANDLER func, void* info)
{
	if ( irqno < IRQ_NUM ) {
		irq_action[irqno].handler = func;
		irq_action[irqno].info = info;
		__dsb(); /* 念のため入れておく (この後に割り込みenableするはずなので不要かも？) */
	}
}

/*
 * 	irq_set_enable
 */
OSAPISTUB void __irq_set_enable(uint32_t irqno, int setting, uint32_t irq_attr)
{
	uint32_t		off;
	uint32_t		bit;

	off = irqno / 32;
	bit = irqno % 32;
	if ( 32 <= irqno ) {
		if ( setting == IRQ_ENABLE ) {
			order_barrier(); /* 割り込み許可前後のメモリオーダー確定 */
			/* Set Target Register */
			uint32_t cpuid = CPU_GET(irq_attr);
			if ( cpuid == CPU_GET(CPU_SELF) ) {
				cpuid = CPUID_get();
			}
			iowrite32n(GICD_IROUTER, irqno, 0x01u<<cpuid);
			iowrite32(GICD_ISENABLER+off*4, 0x01<<bit);
		}
		else {
			iowrite32(GICD_ICENABLER+off*4, 0x01<<bit);
			gicd_wait_rwp();
		}
	}
	else {
		uint32_t cpuid = CPUID_get();
		if ( setting == IRQ_ENABLE ) {
			order_barrier(); /* 割り込み許可前後のメモリオーダー確定 */
			iowrite32(GIC_SGI_BASE(cpuid)+GICR_ISENABLER0, 0x01u << bit);
		}
		else {
			iowrite32(GIC_SGI_BASE(cpuid)+GICR_ICENABLER0, 0x01u << bit);
			gicr_wait_rwp(GIC_RD_BASE(cpuid));
		}
	}
	__dsb(); /* 割り込み禁止/許可設定の同期化 */
	         /* ※次の処理実行中に割り込みが発生するのを防ぐため */
			 /*   割り込み禁止にする場合だけで良い？             */
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
	int ret = 0;
	if ( 32 <= irqno ) {
		ret = (ioread32(GICD_ICENABLER+off*4) & (0x01<<bit)) ? IRQ_ENABLE : IRQ_DISABLE;
	}
	else {
		uint32_t cpuid = CPUID_get();
		ret = (ioread32(GIC_SGI_BASE(cpuid)+GICR_ICENABLER0+off*4) & (0x01<<bit)) ? IRQ_ENABLE : IRQ_DISABLE;
	}
	return ret;
}

/*
 * 	arch_irq_init
 */
void arch_irq_init(uint32_t cpuid)
{
	int ix;
	if ( cpuid == MASTER_CPU_ID ) {
		uint32_t intr_lines = ioread32(GICD_TYPER);
		tprintf("GICD_TYPER=%08X\n", intr_lines);
		intr_lines = 32 * ((intr_lines & 0x1f) + 1);
		if ( 1021 < intr_lines ) {
			intr_lines = 1021;
		}
		tprintf("INTR_LINES=%d\n", intr_lines);

		/************************************************/
		/*	GIC distributor setting	 (Main Core only)	*/
		/************************************************/
		/* GICD control disable */
		iowrite32(GICD_CTLR, 0x000000030);
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
		/* GICD control enable */
		iowrite32(GICD_CTLR, 0x00000037);
		gicd_wait_rwp();
		/* SPI Routing (Aff=CORE0) */
		for ( ix = 32; ix < intr_lines; ix++ ) {
			iowrite32n(GICD_IROUTER, ix, 0x00000000);
		}
	}

	/************************************************/
	/*	GIC Redistributor setting (All Core)		*/
	/************************************************/
	uint8_t* rd_base = GIC_RD_BASE(cpuid);
	uint8_t* sgi_base = GIC_SGI_BASE(cpuid);

	iowrite32(rd_base+GICR_STATUSR, 0xf);
	iowrite32(rd_base+GICR_WAKER, ioread32(rd_base+GICR_WAKER) & ~(0x01<<1));
	while (ioread32(rd_base+GICR_WAKER) & (0x01<<2));
	tprintf("GICR WAKEUP\n");

	iowrite32(rd_base+GICR_CTLR, 0x00000000);
	gicr_wait_rwp(rd_base);
	iowrite32(sgi_base+GICR_IGROUPR0, 0x00000000);
	iowrite32(sgi_base+GICR_IGRPMODR0, 0xffffffff);
	iowrite32(sgi_base+GICR_ICENABLER0, 0xffffffff);
	iowrite32(sgi_base+GICR_ISENABLER0, 0x0000ffff);
	iowrite32(sgi_base+GICR_ICPENDR0, 0xffffffff);
	iowrite32(sgi_base+GICR_ICACTIVER0, 0xffffffff);
	for ( ix = 0; ix < 8; ix++ ) {
		iowrite32n(sgi_base+GICR_IPRIORITYR, ix, 0x00000000);
	}
	iowrite32(sgi_base+GICR_ICFGR0, 0x00000000);
	iowrite32(sgi_base+GICR_ICFGR1, 0x00000000);
	iowrite32(sgi_base+GICR_NSACR, 0x00000000);
	gicr_wait_rwp(rd_base);

	/************************************************/
	/*	GIC CPU interface setting (All Core)		*/
	/************************************************/
	ICC_SRE_EL1_set(0x00000007);
	__isb(); /* XXX: ISBを入れないと次のICC_CTLR_EL1へのアクセスがアボートする */
	ICC_CTLR_EL1_set(0x00000000);
	ICC_PMR_EL1_set(0xff);
	ICC_BPR0_EL1_set(0);
	ICC_BPR1_EL1_set(0);
	ICC_IGRPEN1_EL1_set(0x00000001);
}

void ipi_request_dispatch(uint32_t other_cpu_list)
{
	__dsb();
	ICC_SGI1R_EL1_set(other_cpu_list);
	__isb();
	__dsb();
}

void ipi_request_dispatch_one(CpuStruct* cpu)
{
	uint32_t cpuid = cpu->cpuid;
	__dsb();
	ICC_SGI1R_EL1_set(0x01u<<cpuid);
	__isb();
	__dsb();
}
