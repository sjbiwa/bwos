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

	.text
	.balign 4

/* タスク関連API */
/*OSAPI int task_create(TaskCreateInfo* info);*/
	.global task_create
	.type   task_create, %function
task_create:
	svc #TASK_CREATE_APINO
	bx	lr

/* OSAPI int task_active(int id); */
	.global task_active
	.type   task_active, %function
task_active:
	svc #TASK_ACTIVE_APINO
	bx	lr

/* OSAPI int task_sleep(void); */
	.global task_sleep
	.type   task_sleep, %function
task_sleep:
	svc #TASK_SLEEP_APINO
	bx	lr

/* OSAPI int task_wakeup(int id); */
	.global task_wakeup
	.type   task_wakeup, %function
task_wakeup:
	svc #TASK_WAKEUP_APINO
	bx	lr

/* OSAPI int task_tsleep(TimeOut tm); */
	.global task_tsleep
	.type   task_tsleep, %function
task_tsleep:
	svc #TASK_TSLEEP_APINO
	bx	lr

/* OSAPI int task_dormant(void); */
	.global task_dormant
	.type   task_dormant, %function
task_dormant:
	svc #TASK_DORMANT_APINO
	bx	lr

/* OSAPI void* task_get_tls(int id); */
	.global task_get_tls
	.type   task_get_tls, %function
task_get_tls:
	svc #TASK_GET_TLS_APINO
	bx	lr


/* フラグ関連API */
/* OSAPI int flag_create(void); */
	.global flag_create
	.type   flag_create, %function
flag_create:
	svc #FLAG_CREATE_APINO
	bx	lr

/* OSAPI int flag_set(int id, uint32_t pattern); */
	.global flag_set
	.type   flag_set, %function
flag_set:
	svc #FLAG_SET_APINO
	bx	lr

/* OSAPI int flag_wait(int id, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern); */
	.global flag_wait
	.type   flag_wait, %function
flag_wait:
	svc #FLAG_WAIT_APINO
	bx	lr

/* OSAPI int flag_twait(int id, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern, TimeOut tmout); */
	.global flag_twait_stub
	.type   flag_twait_stub, %function
flag_twait_stub:
	svc #FLAG_TWAIT_APINO
	bx	lr

/* OSAPI int flag_clear(int id, uint32_t pattern); */
	.global flag_clear
	.type   flag_clear, %function
flag_clear:
	svc #FLAG_CLEAR_APINO
	bx	lr


/* ミューテックス関連API */
/* OSAPI int mutex_create(void); */
	.global mutex_create
	.type   mutex_create, %function
mutex_create:
	svc #MUTEX_CREATE_APINO
	bx	lr

/* OSAPI int mutex_unlock(int id); */
	.global mutex_unlock
	.type   mutex_unlock, %function
mutex_unlock:
	svc #MUTEX_UNLOCK_APINO
	bx	lr

/* OSAPI int mutex_lock(int id); */
	.global mutex_lock
	.type   mutex_lock, %function
mutex_lock:
	svc #MUTEX_LOCK_APINO
	bx	lr

/* OSAPI int mutex_tlock(int id, TimeOut tmout); */
	.global mutex_tlock
	.type   mutex_tlock, %function
mutex_tlock:
	svc #MUTEX_TLOCK_APINO
	bx	lr


/* セマフォ関連API */
/* OSAPI int sem_create(uint32_t max); */
	.global sem_create
	.type   sem_create, %function
sem_create:
	svc #SEM_CREATE_APINO
	bx	lr

/* OSAPI int sem_request(int id, uint32_t num); */
	.global sem_request
	.type   sem_request, %function
sem_request:
	svc #SEM_REQUEST_APINO
	bx	lr

/* OSAPI int sem_trequest(int id, uint32_t num, TimeOut tmout); */
	.global sem_trequest
	.type   sem_trequest, %function
sem_trequest:
	svc #SEM_TREQUEST_APINO
	bx	lr

/* OSAPI int sem_release(int id, uint32_t num); */
	.global sem_release
	.type   sem_release, %function
sem_release:
	svc #SEM_RELEASE_APINO
	bx	lr


/* メッセージキュー関連API */
/* OSAPI int msgq_create(uint32_t length); */
	.global msgq_create
	.type   msgq_create, %function
msgq_create:
	svc #MSGQ_CREATE_APINO
	bx	lr

/* OSAPI int msgq_send(int id, void* ptr); */
	.global msgq_send
	.type   msgq_send, %function
msgq_send:
	svc #MSGQ_SEND_APINO
	bx	lr

/* OSAPI int msgq_tsend(int id, void* ptr, TimeOut tmout); */
	.global msgq_tsend
	.type   msgq_tsend, %function
msgq_tsend:
	svc #MSGQ_TSEND_APINO
	bx	lr

/* OSAPI int msgq_recv(int id, void** ptr); */
	.global msgq_recv
	.type   msgq_recv, %function
msgq_recv:
	svc #MSGQ_RECV_APINO
	bx	lr

/* OSAPI int msgq_trecv(int id, void** ptr, TimeOut tmout); */
	.global msgq_trecv
	.type   msgq_trecv, %function
msgq_trecv:
	svc #MSGQ_TRECV_APINO
	bx	lr


/* 固定長メモリブロック関連API */
/* OSAPI int fixmb_create(uint32_t mb_size, uint32_t length); */
	.global fixmb_create
	.type   fixmb_create, %function
fixmb_create:
	svc #FIXMB_CREATE_APINO
	bx	lr

/* OSAPI int fixmb_request(int id, void** ptr); */
	.global fixmb_request
	.type   fixmb_request, %function
fixmb_request:
	svc #FIXMB_REQUEST_APINO
	bx	lr

/* OSAPI int fixmb_trequest(int id, void** ptr, TimeOut tmout); */
	.global fixmb_trequest
	.type   fixmb_trequest, %function
fixmb_trequest:
	svc #FIXMB_TREQUEST_APINO
	bx	lr

/* OSAPI int fixmb_release(int id, void* ptr); */
	.global fixmb_release
	.type   fixmb_release, %function
fixmb_release:
	svc #FIXMB_RELEASE_APINO
	bx	lr


/* ヒープメモリ関連API */
/* OSAPI void* sys_malloc(uint32_t size); */
	.global sys_malloc
	.type   sys_malloc, %function
sys_malloc:
	svc #SYS_MALLOC_APINO
	bx	lr

/* OSAPI void sys_free(void* ptr); */
	.global sys_free
	.type   sys_free, %function
sys_free:
	svc #SYS_FREE_APINO
	bx	lr

/* OSAPI void* sys_malloc_align(uint32_t size, uint32_t align); */
	.global sys_malloc_align
	.type   sys_malloc_align, %function
sys_malloc_align:
	svc #SYS_MALLOC_ALIGN_APINO
	bx	lr

