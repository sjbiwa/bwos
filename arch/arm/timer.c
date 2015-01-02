/*
 * timer.c
 *
 *  Created on: 2012/12/07
 *      Author: biwa
 */
#include "common.h"
#include "mpcore.h"
#include "cp15reg.h"
#include "task.h"


/* Tick処理 */
#if !defined(USE_TICKLESS)
static TimeSpec		tick_count;	/* Tick Counter */
#if defined(HAVE_CP15_TIMER)
static uint64_t	compare_reg_value = 0;
#endif
#endif

static inline uint64_t conv_time_to_reg(TimeSpec time)
{
	return (uint64_t)(time*TIME_UNIT_VALUE);
}

static inline TimeSpec conv_reg_to_time(uint64_t reg)
{
	return (TimeSpec)(reg/TIME_UNIT_VALUE);
}

static void
timer_handler(uint32_t irqno, void* info)
{
#if defined(USE_TICKLESS)
	/* 64bit最大値を設定してタイムアウト割り込みが発生しないようにする */
	CNTP_CVAL_set(0xffffffffffffffffLL);
#else
	/* Tickタイマありの場合はタイマ割り込み設定更新とTickCount更新 */
	uint32_t cpsr;
	irq_save(cpsr);
	tick_count++;
	irq_restore(cpsr);
#if defined(HAVE_SCU_LOCAL_TIMER)
	iowrite32(PTM_INTSTATUS, 0x00000001);
#elif defined(HAVE_CP15_TIMER)
	compare_reg_value += (CNTFRQ_VALUE/1000) * TICK_CYCLE;
	CNTP_CVAL_set(compare_reg_value);
#endif
#endif
	/* OSのタイムアウト処理呼び出し */
	task_tick();
}

/* TICKカウンタの取得 */
TimeSpec get_tick_count(void)
{
#if defined(USE_TICKLESS)
	return conv_reg_to_time(CNTPCT_get());
#else
	TimeSpec ret;
	uint32_t cpsr;
	irq_save(cpsr);
	ret = tick_count;
	irq_restore(cpsr);
	return ret;
#endif
}

/* 最短のタイムアウト時間通知 (tickless対応用) */
void update_first_timeout(TimeSpec tmout)
{
#if defined(USE_TICKLESS)
	/* tmoutをそのまま設定してみる */
	CNTP_CVAL_set(conv_time_to_reg(tmout));
#endif
}

void
timer_init(void)
{
#if defined(USE_TICKLESS)
	/* 64bit最大値を設定してタイムアウト割り込みが発生しないようにする */
	CNTP_CVAL_set(0xffffffffffffffffLL);
	CNTP_CTL_set(0x00000001);
#else
#if defined(HAVE_SCU_LOCAL_TIMER)
	iowrite32(PTM_LOAD, 1500000);
	iowrite32(PTM_COUNTER, 1500000);
	iowrite32(PTM_INTSTATUS, 0x00000001);
	iowrite32(PTM_CONTROL, 0x00000007);
#elif defined(HAVE_CP15_TIMER)
	CNTP_CVAL_set(compare_reg_value);
	CNTP_CTL_set(0x00000001);
#endif
#endif
	irq_add_handler(29, timer_handler, NULL);
	irq_set_enable(29);
}
