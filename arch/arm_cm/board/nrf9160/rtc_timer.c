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
#include "nrfx_clock.h"

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
	NRF_RTC0->CC[0] = tm_diff;
	NRF_RTC0->TASKS_CLEAR = 1;
}

void _rtc0_entry(void)
{
	uint32_t irq_state = irq_save();
	NRF_RTC0->EVENTS_COMPARE[0] = 0;  /* 割り込み要求ビットクリア */
	/* TICK値更新 */
	tick_count += NRF_RTC0->COUNTER;
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
	ret = tick_count + NRF_RTC0->COUNTER;
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
		NVIC->ISPR[RTC0_IRQn/32] = 0x1u << (RTC0_IRQn % 32);
	}
	else {
		TimeSpec tm_diff = tmout_count - tick_count;
		update_timer(tm_diff);
	}
	irq_restore(irq_state);
}

static void nrfx_clock_event_handler(nrfx_clock_evt_type_t event)
{
}

void arch_timer_init(uint32_t cpuid)
{
	nrfx_clock_init(nrfx_clock_event_handler);
	nrfx_clock_enable();
	nrfx_clock_hfclk_start();
	nrfx_clock_lfclk_start();
	
	__irq_set_enable(RTC0_IRQn, IRQ_DISABLE, 0);


	/* PRESCALE設定 (32.768KHz / 8 = 4.096KHz) */
	NRF_RTC0->PRESCALER = 7;

	NRF_RTC0->INTENSET = 0x1u<<16;
	NRF_RTC0->TASKS_START = 1;

	/* 標準タイムアウト値をMATCHレジスタに設定 */
	update_timer(NO_TMOUT_COUNT);

	tmout_count = NO_TMOUT_COUNT;

	//LPC_TIMER0->TCR = 0x01;
	__irq_set_enable(RTC0_IRQn, IRQ_ENABLE, 0);
}
