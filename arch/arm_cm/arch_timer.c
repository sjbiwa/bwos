/*
 * timer.c
 *
 *  Created on: 2015/07/01
 *      Author: biwa
 */
#include "kernel.h"

/* Tick処理 */
static TimeSpec		tick_count;	/* Tick Counter */

static void led_flash(void)
{
static uint32_t delay = 0;
static bool		up = false;
	if ( delay == 0) {
		if ( up ) {
			Board_LED_Set(0, 0);
			up = false;
		}
		else {
			Board_LED_Set(0, 1);
			up = true;
		}
		delay = 1000;
	}
	else {
		delay--;
	}
}

void timer_handler(uint32_t irqno, void* info)
{
	/* Tickタイマありの場合はタイマ割り込み設定更新とTickCount更新 */
	uint32_t irq_state;
	irq_state = irq_save();
	tick_count++;
	irq_restore(irq_state);

	/* OSのタイムアウト処理呼び出し */
	task_tick();

	led_flash();
}

/* TICKカウンタの取得 */
TimeSpec get_tick_count(void)
{
	TimeSpec ret;
	uint32_t irq_state;
	irq_state = irq_save();
	ret = tick_count;
	irq_restore(irq_state);
	return ret;
}

/* 最短のタイムアウト時間通知 (tickless対応用) */
void update_first_timeout(TimeSpec tmout)
{
}

void
arch_timer_init(uint32_t cpuid)
{
	SysTick->LOAD = MAX_CLOCK_FREQ / 1000;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}
