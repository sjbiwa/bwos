/*
**
*/
	.syntax unified
	.cpu cortex-a7
	.thumb
	.thumb_func

#include "arm.h"
#include "cp15reg.h"
#include "api_no.h"

/* OSAPI定義用マクロ */
.macro DEF_OSAPI api_name, api_id
	.global \api_name
	.type   \api_name, %function
\api_name:
	push {lr}
	svc #\api_id
	pop {lr}
	bx	lr
.endm
/*********************/

	.text
	.balign 4

/* タスク関連API */
/*OSAPI int task_create(TaskCreateInfo* info);*/
DEF_OSAPI task_create, TASK_CREATE_APINO
/* OSAPI int task_active(int id); */
DEF_OSAPI task_active, TASK_ACTIVE_APINO
/* OSAPI int task_sleep(void); */
DEF_OSAPI task_sleep, TASK_SLEEP_APINO
/* OSAPI int task_wakeup(int id); */
DEF_OSAPI task_wakeup, TASK_WAKEUP_APINO
/* OSAPI int task_tsleep(TimeOut tm); */
DEF_OSAPI task_tsleep, TASK_TSLEEP_APINO
/* OSAPI int task_dormant(void); */
DEF_OSAPI task_dormant, TASK_DORMANT_APINO
/* OSAPI void* task_get_tls(int id); */
DEF_OSAPI task_get_tls, TASK_GET_TLS_APINO


/* フラグ関連API */
/* OSAPI int flag_create(void); */
DEF_OSAPI flag_create, FLAG_CREATE_APINO
/* OSAPI int flag_set(int id, uint32_t pattern); */
DEF_OSAPI flag_set, FLAG_SET_APINO
/* OSAPI int flag_wait(int id, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern); */
DEF_OSAPI flag_wait, FLAG_WAIT_APINO
/* OSAPI int flag_twait(int id, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern, TimeOut tmout); */
DEF_OSAPI flag_twait_stub, FLAG_TWAIT_APINO
/* OSAPI int flag_clear(int id, uint32_t pattern); */
DEF_OSAPI flag_clear, FLAG_CLEAR_APINO


/* ミューテックス関連API */
/* OSAPI int mutex_create(void); */
DEF_OSAPI mutex_create, MUTEX_CREATE_APINO
/* OSAPI int mutex_unlock(int id); */
DEF_OSAPI mutex_unlock, MUTEX_UNLOCK_APINO
/* OSAPI int mutex_lock(int id); */
DEF_OSAPI mutex_lock, MUTEX_LOCK_APINO
/* OSAPI int mutex_tlock(int id, TimeOut tmout); */
DEF_OSAPI mutex_tlock, MUTEX_TLOCK_APINO


/* セマフォ関連API */
/* OSAPI int sem_create(uint32_t max); */
DEF_OSAPI sem_create, SEM_CREATE_APINO
/* OSAPI int sem_request(int id, uint32_t num); */
DEF_OSAPI sem_request, SEM_REQUEST_APINO
/* OSAPI int sem_trequest(int id, uint32_t num, TimeOut tmout); */
DEF_OSAPI sem_trequest, SEM_TREQUEST_APINO
/* OSAPI int sem_release(int id, uint32_t num); */
DEF_OSAPI sem_release, SEM_RELEASE_APINO


/* メッセージキュー関連API */
/* OSAPI int msgq_create(uint32_t length); */
DEF_OSAPI msgq_create, MSGQ_CREATE_APINO
/* OSAPI int msgq_send(int id, void* ptr); */
DEF_OSAPI msgq_send, MSGQ_SEND_APINO
/* OSAPI int msgq_tsend(int id, void* ptr, TimeOut tmout); */
DEF_OSAPI msgq_tsend, MSGQ_TSEND_APINO
/* OSAPI int msgq_recv(int id, void** ptr); */
DEF_OSAPI msgq_recv, MSGQ_RECV_APINO
/* OSAPI int msgq_trecv(int id, void** ptr, TimeOut tmout); */
DEF_OSAPI msgq_trecv, MSGQ_TRECV_APINO


/* 固定長メモリブロック関連API */
/* OSAPI int fixmb_create(uint32_t mb_size, uint32_t length); */
DEF_OSAPI fixmb_create, FIXMB_CREATE_APINO
/* OSAPI int fixmb_request(int id, void** ptr); */
DEF_OSAPI fixmb_request, FIXMB_REQUEST_APINO
/* OSAPI int fixmb_trequest(int id, void** ptr, TimeOut tmout); */
DEF_OSAPI fixmb_trequest, FIXMB_TREQUEST_APINO
/* OSAPI int fixmb_release(int id, void* ptr); */
DEF_OSAPI fixmb_release, FIXMB_RELEASE_APINO


/* ヒープメモリ関連API */
/* OSAPI void* sys_malloc(uint32_t size); */
DEF_OSAPI sys_malloc, SYS_MALLOC_APINO
/* OSAPI void sys_free(void* ptr); */
DEF_OSAPI sys_free, SYS_FREE_APINO
/* OSAPI void* sys_malloc_align(uint32_t size, uint32_t align); */
DEF_OSAPI sys_malloc_align, SYS_MALLOC_ALIGN_APINO

/* OSAPI void irq_add_handler(uint32_t irqno, IRQ_HANDLER func, void* info); */
DEF_OSAPI irq_add_handler, IRQ_ADD_HANDLER_APINO
/* OSAPI void irq_set_enable(uint32_t irqno, int setting); */
DEF_OSAPI irq_set_enable, IRQ_SET_ENABLE_APINO
/* OSAPI int irq_get_enable(info->irq); */
DEF_OSAPI irq_get_enable, IRQ_GET_ENABLE_APINO
