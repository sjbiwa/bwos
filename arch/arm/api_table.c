/*
 * api_table.c
 *
 *  Created on: 2015/01/25
 *      Author: biwa
 */
#include "common.h"
#include "api_stub.h"
#include "api_no.h"
#include "api_caller_callee.h"

/* API calleeスタブ */
extern int __flag_twait_stub(APIFlagTwaitParam* param);
/********************/

void* svc_entry_table[] = {
		[TASK_CREATE_APINO] =                 (void*)__task_create,
		[TASK_ACTIVE_APINO] =                 (void*)__task_active,
		[TASK_SLEEP_APINO] =                  (void*)__task_sleep,
		[TASK_WAKEUP_APINO] =                 (void*)__task_wakeup,
		[TASK_TSLEEP_APINO] =                 (void*)__task_tsleep,
		[TASK_GET_TLS_APINO] =                (void*)__task_get_tls,
		[FLAG_CREATE_APINO] =                 (void*)__flag_create,
		[FLAG_SET_APINO] =                    (void*)__flag_set,
		[FLAG_WAIT_APINO] =                   (void*)__flag_wait,
		[FLAG_TWAIT_APINO] =                  (void*)__flag_twait_stub,
		[FLAG_CLEAR_APINO] =                  (void*)__flag_clear,
		[MUTEX_CREATE_APINO] =                (void*)__mutex_create,
		[MUTEX_UNLOCK_APINO] =                (void*)__mutex_unlock,
		[MUTEX_LOCK_APINO] =                  (void*)__mutex_lock,
		[MUTEX_TLOCK_APINO] =                 (void*)__mutex_tlock,
		[SEM_CREATE_APINO] =                  (void*)__sem_create,
		[SEM_REQUEST_APINO] =                 (void*)__sem_request,
		[SEM_REQUEST_APINO] =                 (void*)__sem_trequest,
		[SEM_RELEASE_APINO] =                 (void*)__sem_release,
		[MSGQ_CREATE_APINO] =                 (void*)__msgq_create,
		[MSGQ_SEND_APINO] =                   (void*)__msgq_send,
		[MSGQ_TSEND_APINO] =                  (void*)__msgq_tsend,
		[MSGQ_RECV_APINO] =                   (void*)__msgq_recv,
		[MSGQ_TRECV_APINO] =                  (void*)__msgq_trecv,
		[FIXMB_CREATE_APINO] =                (void*)__fixmb_create,
		[FIXMB_REQUEST_APINO] =               (void*)__fixmb_request,
		[FIXMB_TREQUEST_APINO] =              (void*)__fixmb_trequest,
		[FIXMB_RELEASE_APINO] =               (void*)__fixmb_release,
		[SYS_MALLOC_APINO] =                  (void*)__sys_malloc,
		[SYS_FREE_APINO] =                    (void*)__sys_free,
		[SYS_MALLOC_ALIGN_APINO] =            (void*)__sys_malloc_align,
};
