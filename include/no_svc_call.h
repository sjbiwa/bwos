/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * no_svc_call.h
 *
 *  Created on: 2015/06/28
 *      Author: biwa
 */

#ifndef NO_SVC_CALL_H_
#define NO_SVC_CALL_H_

/* タスク関連API */
#define	task_create			__task_create
#define	task_active			__task_active
#define	task_sleep			__task_sleep
#define	task_wakeup			__task_wakeup
#define	task_tsleep			__task_tsleep
#define	task_dormant		__task_dormant
#define	task_get_tls		__task_get_tls

/* フラグ関連API */
#define	flag_create			__flag_create
#define	flag_set			__flag_set
#define	flag_wait			__flag_wait
#define	flag_twait			__flag_twait
#define	flag_clear			__flag_clear

/* ミューテックス関連API */
#define	mutex_create		__mutex_create
#define	mutex_unlock		__mutex_unlock
#define	mutex_lock			__mutex_lock
#define	mutex_tlock			__mutex_tlock

/* セマフォ関連API */
#define	sem_create			__sem_create
#define	sem_request			__sem_request
#define	sem_trequest		__sem_trequest
#define	sem_release			__sem_release

/* メッセージキュー関連API */
#define	msgq_create			__msgq_create
#define	msgq_send			__msgq_send
#define	msgq_tsend			__msgq_tsend
#define	msgq_recv			__msgq_recv
#define	msgq_trecv			__msgq_trecv

/* 固定長メモリブロック関連API */
#define	fixmb_create		__fixmb_create
#define	fixmb_request		__fixmb_request
#define	fixmb_trequest		__fixmb_trequest
#define	fixmb_release		__fixmb_release

/* ヒープメモリ関連API */
#define	sys_malloc			__sys_malloc
#define	sys_free			__sys_free
#define	sys_malloc_align	__sys_malloc_align

/* 割り込みハンドラ関連API */
#define	irq_add_handler		__irq_add_handler
#define	irq_set_enable		__irq_set_enable
#define	irq_get_enable		__irq_get_enable

/* タイマハンドラ関連API */
#define	timer_create		__timer_create
#define	timer_set			__timer_set
#define	timer_enable		__timer_enable

#endif /* NO_SVC_CALL_H_ */
