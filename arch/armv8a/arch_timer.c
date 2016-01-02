/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

#include "kernel.h"
#include "armv8reg.h"
#include "gicv3reg.h"

/* TickLess専用 */
#define	hword(v)		((uint32_t)((uint64_t)(v)>>32))
#define	lword(v)		((uint32_t)(v))

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
}

/* TICKカウンタの取得 */
TimeSpec get_tick_count(void)
{
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
		__irq_add_handler(30, timer_handler, NULL);
	}
	__irq_set_enable(30, IRQ_ENABLE, CPU_SELF);
}
