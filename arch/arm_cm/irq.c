/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * irq.c
 *
 *  Created on: 2015/07/01
 *      Author: biwa
 */
#include "kernel.h"

/*
 * 	c_handler
 */
void c_handler(uint32_t* sp, uint32_t pc, uint32_t sr)
{
}

/*
 * 	irq_add_handler
 */
OSAPISTUB void __irq_add_handler(uint32_t irqno, IRQ_HANDLER func, void* info)
{
}

/*
 * 	irq_set_enable
 */
OSAPISTUB void __irq_set_enable(uint32_t irqno, int setting, uint32_t irq_attr)
{
	if ( irqno < IRQ_NUM ) {
		uint32_t off = irqno / 32;
		uint32_t bit = irqno % 32;
		if ( setting == IRQ_ENABLE ) {
			/* 割り込み有効 */
			order_barrier(); /* 割り込み許可前後のメモリオーダー確定 */
#if __ARM_ARCH == 8
			NVIC->IPR[irqno] = 0xff;
#elif __ARM_ARCH == 7
			NVIC->IP[irqno] = 0xff;
#else
			uint32_t pri_off = irqno / 4;
			uint32_t pri_pos = (irqno % 4) * 8;
			NVIC->IP[pri_off] = (NVIC->IP[pri_off] & ~(0xff << pri_pos)) | (0x40 << pri_pos);
#endif
			NVIC->ISER[off] = (0x1u << bit);
		}
		else {
			/* 割り込み禁止 */
			NVIC->ICER[off] = (0x1u << bit);
		}
		__dsb(); /* 割り込み禁止/許可設定の同期化 */
				 /* ※次の処理実行中に割り込みが発生するのを防ぐため */
				 /*   割り込み禁止にする場合だけで良い？             */
	}
}

/*
 * 	irq_get_enable
 */
OSAPISTUB int __irq_get_enable(uint32_t irqno)
{
	int ret = IRQ_DISABLE;
	if ( irqno < IRQ_NUM ) {
		uint32_t off = irqno / 32;
		uint32_t bit = irqno % 32;
		if ( NVIC->ISER[off] & (0x1u << bit) ) {
			int ret = IRQ_ENABLE;
		}
	}
	return ret;
}

/*
 * 	arch_irq_init
 */
void
arch_irq_init(uint32_t cpuid)
{
}
