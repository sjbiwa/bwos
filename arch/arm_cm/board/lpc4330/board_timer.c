/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * timer.c
 *
 *  Created on: 2015/07/01
 *      Author: biwa
 */
#include "kernel.h"
#include "chip.h"

#define	NO_TMOUT_COUNT		(0xFFFFFFFFFFFFFFFFLL)
#define	NORMAL_TMOUT_COUNT	(10000)

/* Tick処理 */
static TimeSpec		tick_count;		/* Tick Counter */
static TimeSpec		tmout_count;	/* Timeout Counter value */
/* DUMMY */
void timer_handler(void)
{
}

static void update_timer(TimeSpec tm_diff)
{
	if ( NORMAL_TMOUT_COUNT < tm_diff ) {
		tm_diff = NORMAL_TMOUT_COUNT;
	}
	LPC_TIMER0->MR[0] = tm_diff;
	LPC_TIMER0->TC = 0;
}

void _timer0_entry(void)
{
	uint32_t irq_state = irq_save();
	LPC_TIMER0->IR = 0xff;
	/* TICK値更新 */
	tick_count += LPC_TIMER0->TC;
	if ( tmout_count <= tick_count ) {
		TimeSpec tm_diff = tick_count - tmout_count;
		update_timer(tm_diff);

		tmout_count = NO_TMOUT_COUNT;
		irq_restore(irq_state);
		/* OSのタイムアウト処理呼び出し */
		task_tick();
	}
	else {
		TimeSpec tm_diff = tmout_count - tick_count;
		update_timer(tm_diff);

		irq_restore(irq_state);
	}
}

/* TICKカウンタの取得 */
TimeSpec get_tick_count(void)
{
	TimeSpec ret;
	uint32_t irq_state = irq_save();
	ret = tick_count + LPC_TIMER0->TC;
	irq_restore(irq_state);
	return ret;
}

/* 最短のタイムアウト時間通知 (tickless対応用) */
void update_first_timeout(TimeSpec tmout)
{
	TimeSpec ret;
	uint32_t irq_state = irq_save();
	tmout_count = tmout;
	TimeSpec curr_count = tick_count + LPC_TIMER0->TC;
	if ( tmout_count <= curr_count ) {
		/* すでに過ぎているのでTIMER割り込みONする */
		NVIC->ISPR[TIMER0_IRQn/32] = 0x1u << (TIMER0_IRQn % 32);
	}
	else {
		TimeSpec tm_diff = tmout_count - tick_count;
		update_timer(tm_diff);
	}
	irq_restore(irq_state);
}

void
arch_timer_init(uint32_t cpuid)
{
	__irq_set_enable(TIMER0_IRQn, IRQ_DISABLE, 0);

	/* PRESCALE設定 (1ms毎にカウントアップ) */
	LPC_TIMER0->PR = CPU_CLOCK/1000;

	LPC_TIMER0->IR = 0x01;
	LPC_TIMER0->MCR = 0x01;
	LPC_TIMER0->TCR = 0x03;

	/* 標準タイムアウト値をMATCHレジスタに設定 */
	update_timer(NO_TMOUT_COUNT);

	tmout_count = NO_TMOUT_COUNT;

	LPC_TIMER0->TCR = 0x01;
	__irq_set_enable(TIMER0_IRQn, IRQ_ENABLE, 0);
}
