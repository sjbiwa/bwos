/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * api_no.h
 *
 *  Created on: 2015/01/25
 *      Author: biwa
 */

#ifndef API_NO_H_
#define API_NO_H_

/* API番号定義 */
#define	TASK_CREATE_APINO					1
#define	TASK_ACTIVE_APINO					2
#define	TASK_SLEEP_APINO					3
#define	TASK_WAKEUP_APINO					4
#define	TASK_TSLEEP_APINO					5
#define	TASK_DORMANT_APINO					6
#define	TASK_GET_TLS_APINO					7
#define	FLAG_CREATE_APINO					8
#define	FLAG_SET_APINO						9
#define	FLAG_WAIT_APINO						10
#define	FLAG_TWAIT_APINO					11
#define	FLAG_CLEAR_APINO					12
#define	MUTEX_CREATE_APINO					13
#define	MUTEX_UNLOCK_APINO					14
#define	MUTEX_LOCK_APINO					15
#define	MUTEX_TLOCK_APINO					16
#define	SEM_CREATE_APINO					17
#define	SEM_REQUEST_APINO					18
#define	SEM_TREQUEST_APINO					19
#define	SEM_RELEASE_APINO					20
#define	MSGQ_CREATE_APINO					21
#define	MSGQ_SEND_APINO						22
#define	MSGQ_TSEND_APINO					23
#define	MSGQ_RECV_APINO						24
#define	MSGQ_TRECV_APINO					25
#define	FIXMB_CREATE_APINO					26
#define	FIXMB_REQUEST_APINO					27
#define	FIXMB_TREQUEST_APINO				28
#define	FIXMB_RELEASE_APINO					29
#define	SYS_MALLOC_APINO					30
#define	SYS_FREE_APINO						31
#define	SYS_MALLOC_ALIGN_APINO				32
#define	IRQ_ADD_HANDLER_APINO				33
#define	IRQ_SET_ENABLE_APINO				34
#define	IRQ_GET_ENABLE_APINO				35
#define	TIMER_CREATE_APINO					36
#define	TIMER_SET_APINO						37
#define	TIMER_ENABLE_APINO					38
#if USE_SMP == 1
#define	TASK_SET_AFFINITY_APINO				39
#endif

#endif /* API_NO_H_ */
