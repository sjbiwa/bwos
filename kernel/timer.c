/*
 * timer.c
 *
 *  Created on: 2015/05/13
 *      Author: biwa
 */
#include <stdint.h>
#include "kernel.h"
#include "link.h"

#if !defined(TIMER_MAX_NUM)
#define	TIMER_MAX_NUM		(100)
#endif

typedef	struct tagTimerStruct {
	Link		link;
	TimeSpec	timeout;
	TimerInfo	info;
} TimerStruct;

/* オブジェクト<->インデックス変換用 */
static void timer_sub_init(void) {}
OBJECT_INDEX_FUNC(timer,TimerStruct,TIMER_MAX_NUM);


static int _kernel_timer_create(TimerStruct* timer)
{
	link_clear(&timer->link);
	return RT_OK;
}

static int _kernel_timer_set(TimerStruct* timer, TimerInfo* info)
{
	uint32_t irq_state = irq_save();
	if ( !link_is_empty(&timer->link) ) {
		/* 再設定する場合はいったん外す */
		link_remove(&timer->link);
	}
	irq_restore(irq_state);
	timer->info = *info;
	return RT_OK;
}

static void _timer_add(TimerStruct* timer)
{
	Link* timer_list = &(cpu_struct[CPUID_get()].timer_list);
	Link* link = timer_list->next;
	while ( link != timer_list ) {
		TimerStruct* q_timer = containerof(link, TimerStruct, link);
		/* 検索中タイマのexpire時間が登録タイマよりも早ければ */
		/* その直前に登録する */
		if ( timer->timeout <= q_timer->timeout ) {
			/* linkの前が登録位置 */
			break;
		}
		link = link->next;
	}
	/* link_add_lastはlinkの直前に入れるので期待通り */
	link_add_last(link, &(timer->link));
}

static int _kernel_timer_enable(TimerStruct* timer, bool enable)
{
	uint32_t irq_state = irq_save();
	if ( enable ) {
		/* 有効化 */
		timer->timeout = get_tick_count() + timer->info.tmout;
		_timer_add(timer);
	}
	else {
		/* 無効化 */
		link_remove(&timer->link);
	}

	/* タイムアウト更新 */
	_kernel_timer_update();

	irq_restore(irq_state);
	return RT_OK;
}


OSAPISTUB int __timer_create(void)
{
	int ret = RT_ERR;
	int timer_id = alloc_timer_id();
	if ( 0 <= timer_id ) {
		TimerStruct* timer = timerid2object(timer_id);
		ret = _kernel_timer_create(timer);
		if ( ret == RT_OK ) {
			ret = timer_id;
		}
		else {
			free_timer_struct(timer_id);
		}
	}
	return ret;
}

OSAPISTUB int __timer_set(int id, TimerInfo* info)
{
	TimerStruct* timer = timerid2object(id);
	return _kernel_timer_set(timer, info);
}

OSAPISTUB int __timer_enable(int id, bool enable)
{
	TimerStruct* timer = timerid2object(id);
	return _kernel_timer_enable(timer, enable);
}

/* タイマモジュールにタイマexpireを通知 */
void _timer_notify_tick(TimeSpec tick_count)
{
	Link* timer_list = &(cpu_struct[CPUID_get()].timer_list);
	Link* link;
	while ( (link = timer_list->next) != timer_list ) {
		TimerStruct* q_timer = containerof(link, TimerStruct, link);
		/* expire時間のほうが大きいところで検索終了 */
		if ( tick_count < q_timer->timeout ) {
			break;
		}
		/* いったんリストから削除する */
		link_remove(&q_timer->link);
		q_timer->info.handler(q_timer->info.param);
		/* cyclicの場合は再度登録 */
		if ( q_timer->info.kind == TIMER_CYCLIC ) {
			q_timer->timeout = tick_count + q_timer->info.cyclic;
			_timer_add(q_timer);
		}
	}
}

TimeSpec _timer_get_next_timeout(void)
{
	Link* timer_list = &(cpu_struct[CPUID_get()].timer_list);
	TimeSpec ret = (TimeSpec)0;
	if ( !link_is_empty(timer_list) ) {
		TimerStruct* timer = containerof(timer_list->next, TimerStruct, link);
		ret = timer->timeout;
	}
	return ret;
}
