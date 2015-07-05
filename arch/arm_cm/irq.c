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
	__dsb(); /* 割り込み禁止/許可設定の同期化 */
	         /* ※次の処理実行中に割り込みが発生するのを防ぐため */
			 /*   割り込み禁止にする場合だけで良い？             */
}

/*
 * 	irq_get_enable
 */
OSAPISTUB int __irq_get_enable(uint32_t irqno)
{
	int ret;
	return ret;
}

/*
 * 	arch_irq_init
 */
void
arch_irq_init(uint32_t cpuid)
{
}
