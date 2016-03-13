/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

#include "kernel.h"
#include "armv8reg.h"
#include "gicv3reg.h"

/* TickLess専用 */

static inline uint64_t conv_time_to_reg(TimeSpec time)
{
	return (uint64_t)(time*TIME_UNIT_VALUE);
}

static inline TimeSpec conv_reg_to_time(uint64_t reg)
{
	return (TimeSpec)(reg/TIME_UNIT_VALUE);
}

static void timer_handler(uint32_t irqno, void* info)
{
	/* 64bit最大値を設定してタイムアウト割り込みが発生しないようにする */
	CNTP_CVAL_EL0_set(0xffffffffffffffffLL);

	/* OSのタイムアウト処理呼び出し */
	task_tick();

	uint64_t t = CNTPCT_EL0_get();
	if ( t == 0xffffffffffffffffuLL ) {
		debug_print("COUNTER is over\n");
		for (;;) {
			__wfe();
		}
	}
}

/* TICKカウンタの取得 */
TimeSpec get_tick_count(void)
{
	__isb();
	return conv_reg_to_time(CNTPCT_EL0_get());
}

/* 最短のタイムアウト時間通知 */
void update_first_timeout(TimeSpec tmout)
{
	CNTP_CVAL_EL0_set(conv_time_to_reg(tmout));
}

void arch_timer_init(uint32_t cpuid)
{
	/* timer init */
	CNTP_CTL_EL0_set(0x00000000);
	/* 64bit最大値を設定してタイムアウト割り込みが発生しないようにする */
	CNTP_CVAL_EL0_set(0xffffffffffffffffLL);
	CNTP_CTL_EL0_set(0x00000001);

	if ( cpuid == MASTER_CPU_ID ) {
		__irq_add_handler(GENERIC_TIMER_INTR, timer_handler, NULL);
	}
	__irq_set_enable(GENERIC_TIMER_INTR, IRQ_ENABLE, CPU_SELF);
}
