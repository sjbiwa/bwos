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
	push {lr}
	svc #TASK_CREATE_APINO
	pop {lr}
	bx	lr

/* OSAPI int task_active(int id); */
	.global task_active
	.type   task_active, %function
task_active:
	push {lr}
	svc #TASK_ACTIVE_APINO
	pop {lr}
	bx	lr

/* OSAPI int task_sleep(void); */
	.global task_sleep
	.type   task_sleep, %function
task_sleep:
	push {lr}
	svc #TASK_SLEEP_APINO
	pop {lr}
	bx	lr

/* OSAPI int task_wakeup(int id); */
	.global task_wakeup
	.type   task_wakeup, %function
task_wakeup:
	push {lr}
	svc #TASK_WAKEUP_APINO
	pop {lr}
	bx	lr

/* OSAPI int task_tsleep(TimeOut tm); */
	.global task_tsleep
	.type   task_tsleep, %function
task_tsleep:
	push {lr}
	svc #TASK_TSLEEP_APINO
	pop {lr}
	bx	lr

/* OSAPI int task_dormant(void); */
	.global task_dormant
	.type   task_dormant, %function
task_dormant:
	push {lr}
	svc #TASK_DORMANT_APINO
	pop {lr}
	bx	lr

/* OSAPI void* task_get_tls(int id); */
	.global task_get_tls
	.type   task_get_tls, %function
task_get_tls:
	push {lr}
	svc #TASK_GET_TLS_APINO
	pop {lr}
	bx	lr


/* フラグ関連API */
/* OSAPI int flag_create(void); */
	.global flag_create
	.type   flag_create, %function
flag_create:
	push {lr}
	svc #FLAG_CREATE_APINO
	pop {lr}
	bx	lr

/* OSAPI int flag_set(int id, uint32_t pattern); */
	.global flag_set
	.type   flag_set, %function
flag_set:
	push {lr}
	svc #FLAG_SET_APINO
	pop {lr}
	bx	lr

/* OSAPI int flag_wait(int id, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern); */
	.global flag_wait
	.type   flag_wait, %function
flag_wait:
	push {lr}
	svc #FLAG_WAIT_APINO
	pop {lr}
	bx	lr

/* OSAPI int flag_twait(int id, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern, TimeOut tmout); */
	.global flag_twait_stub
	.type   flag_twait_stub, %function
flag_twait_stub:
	push {lr}
	svc #FLAG_TWAIT_APINO
	pop {lr}
	bx	lr

/* OSAPI int flag_clear(int id, uint32_t pattern); */
	.global flag_clear
	.type   flag_clear, %function
flag_clear:
	push {lr}
	svc #FLAG_CLEAR_APINO
	pop {lr}
	bx	lr


/* ミューテックス関連API */
/* OSAPI int mutex_create(void); */
	.global mutex_create
	.type   mutex_create, %function
mutex_create:
	push {lr}
	svc #MUTEX_CREATE_APINO
	pop {lr}
	bx	lr

/* OSAPI int mutex_unlock(int id); */
	.global mutex_unlock
	.type   mutex_unlock, %function
mutex_unlock:
	push {lr}
	svc #MUTEX_UNLOCK_APINO
	pop {lr}
	bx	lr

/* OSAPI int mutex_lock(int id); */
	.global mutex_lock
	.type   mutex_lock, %function
mutex_lock:
	push {lr}
	svc #MUTEX_LOCK_APINO
	pop {lr}
	bx	lr

/* OSAPI int mutex_tlock(int id, TimeOut tmout); */
	.global mutex_tlock
	.type   mutex_tlock, %function
mutex_tlock:
	push {lr}
	svc #MUTEX_TLOCK_APINO
	pop {lr}
	bx	lr


/* セマフォ関連API */
/* OSAPI int sem_create(uint32_t max); */
	.global sem_create
	.type   sem_create, %function
sem_create:
	push {lr}
	svc #SEM_CREATE_APINO
	pop {lr}
	bx	lr

/* OSAPI int sem_request(int id, uint32_t num); */
	.global sem_request
	.type   sem_request, %function
sem_request:
	push {lr}
	svc #SEM_REQUEST_APINO
	pop {lr}
	bx	lr

/* OSAPI int sem_trequest(int id, uint32_t num, TimeOut tmout); */
	.global sem_trequest
	.type   sem_trequest, %function
sem_trequest:
	push {lr}
	svc #SEM_TREQUEST_APINO
	pop {lr}
	bx	lr

/* OSAPI int sem_release(int id, uint32_t num); */
	.global sem_release
	.type   sem_release, %function
sem_release:
	push {lr}
	svc #SEM_RELEASE_APINO
	pop {lr}
	bx	lr


/* メッセージキュー関連API */
/* OSAPI int msgq_create(uint32_t length); */
	.global msgq_create
	.type   msgq_create, %function
msgq_create:
	push {lr}
	svc #MSGQ_CREATE_APINO
	pop {lr}
	bx	lr

/* OSAPI int msgq_send(int id, void* ptr); */
	.global msgq_send
	.type   msgq_send, %function
msgq_send:
	push {lr}
	svc #MSGQ_SEND_APINO
	pop {lr}
	bx	lr

/* OSAPI int msgq_tsend(int id, void* ptr, TimeOut tmout); */
	.global msgq_tsend
	.type   msgq_tsend, %function
msgq_tsend:
	push {lr}
	svc #MSGQ_TSEND_APINO
	pop {lr}
	bx	lr

/* OSAPI int msgq_recv(int id, void** ptr); */
	.global msgq_recv
	.type   msgq_recv, %function
msgq_recv:
	push {lr}
	svc #MSGQ_RECV_APINO
	pop {lr}
	bx	lr

/* OSAPI int msgq_trecv(int id, void** ptr, TimeOut tmout); */
	.global msgq_trecv
	.type   msgq_trecv, %function
msgq_trecv:
	push {lr}
	svc #MSGQ_TRECV_APINO
	pop {lr}
	bx	lr


/* 固定長メモリブロック関連API */
/* OSAPI int fixmb_create(uint32_t mb_size, uint32_t length); */
	.global fixmb_create
	.type   fixmb_create, %function
fixmb_create:
	push {lr}
	svc #FIXMB_CREATE_APINO
	pop {lr}
	bx	lr

/* OSAPI int fixmb_request(int id, void** ptr); */
	.global fixmb_request
	.type   fixmb_request, %function
fixmb_request:
	push {lr}
	svc #FIXMB_REQUEST_APINO
	pop {lr}
	bx	lr

/* OSAPI int fixmb_trequest(int id, void** ptr, TimeOut tmout); */
	.global fixmb_trequest
	.type   fixmb_trequest, %function
fixmb_trequest:
	push {lr}
	svc #FIXMB_TREQUEST_APINO
	pop {lr}
	bx	lr

/* OSAPI int fixmb_release(int id, void* ptr); */
	.global fixmb_release
	.type   fixmb_release, %function
fixmb_release:
	push {lr}
	svc #FIXMB_RELEASE_APINO
	pop {lr}
	bx	lr


/* ヒープメモリ関連API */
/* OSAPI void* sys_malloc(uint32_t size); */
	.global sys_malloc
	.type   sys_malloc, %function
sys_malloc:
	push {lr}
	svc #SYS_MALLOC_APINO
	pop {lr}
	bx	lr

/* OSAPI void sys_free(void* ptr); */
	.global sys_free
	.type   sys_free, %function
sys_free:
	push {lr}
	svc #SYS_FREE_APINO
	pop {lr}
	bx	lr

/* OSAPI void* sys_malloc_align(uint32_t size, uint32_t align); */
	.global sys_malloc_align
	.type   sys_malloc_align, %function
sys_malloc_align:
	push {lr}
	svc #SYS_MALLOC_ALIGN_APINO
	pop {lr}
	bx	lr

/* OSAPI void irq_add_handler(uint32_t irqno, IRQ_HANDLER func, void* info); */
	.global irq_add_handler
	.type   irq_add_handler, %function
irq_add_handler:
	push {lr}
	svc #IRQ_ADD_HANDLER_APINO
	pop {lr}
	bx	lr

/* OSAPI void irq_set_enable(uint32_t irqno, int setting); */
	.global irq_set_enable
	.type   irq_set_enable, %function
irq_set_enable:
	push {lr}
	svc #IRQ_SET_ENABLE_APINO
	pop {lr}
	bx	lr

/* OSAPI int irq_get_enable(info->irq); */
	.global irq_get_enable
	.type   irq_get_enable, %function
irq_get_enable:
	push {lr}
	svc #IRQ_GET_ENABLE_APINO
	pop {lr}
	bx	lr
