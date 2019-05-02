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
#include "common.h"
#include "cmsis.h"

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
	NRF_TIMER2->CC[0] = tm_diff;
	NRF_TIMER2->TASKS_CLEAR = 1;
}

void _timer2_entry(void)
{
	uint32_t irq_state = irq_save();
	NRF_TIMER2->EVENTS_COMPARE[0] = 0;  /* 割り込み要求ビットクリア */
	/* TICK値更新 */
	NRF_TIMER2->TASKS_CAPTURE[1] = 1;
	tick_count += NRF_TIMER2->CC[1];
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
	NRF_TIMER2->TASKS_CAPTURE[1] = 1;
	ret = tick_count + NRF_TIMER2->CC[1];
	irq_restore(irq_state);
	return ret;
}

/* 最短のタイムアウト時間通知 (tickless対応用) */
void update_first_timeout(TimeSpec tmout)
{
	TimeSpec ret;
	uint32_t irq_state = irq_save();
	tmout_count = tmout;
	TimeSpec curr_count = tick_count/* + LPC_TIMER0->TC*/;
	if ( tmout_count <= curr_count ) {
		/* すでに過ぎているのでTIMER割り込みONする */
		NVIC->ISPR[TIMER2_IRQn/32] = 0x1u << (TIMER2_IRQn % 32);
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
	__irq_set_enable(TIMER2_IRQn, IRQ_DISABLE, 0);

	/* LFCLK start */
	//NRF_CLOCK->TASKS_LFCLKSTART = 1;

	/* PRESCALE設定 1MHz */
	NRF_TIMER2->PRESCALER = 4;
	NRF_TIMER2->MODE = 0;
	NRF_TIMER2->BITMODE = 3;
	NRF_TIMER2->INTENSET = 0x1u<<16;
	NRF_TIMER2->TASKS_START = 1;

	/* 標準タイムアウト値をMATCHレジスタに設定 */
	update_timer(NO_TMOUT_COUNT);

	tmout_count = NO_TMOUT_COUNT;

	__irq_set_enable(TIMER2_IRQn, IRQ_ENABLE, 0);
}
