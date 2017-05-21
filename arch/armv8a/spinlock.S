/*
 * Copyright (C) 2016 sjbiwa All rights reserved.
 */

/*
 * spinlock.S
 *
 *  Created on: 2016/01/10
 *      Author: biwa
 */

#include "armv8.h"
#include "armv8reg.h"

	.text
#define	USE_WFE_SEV

/****************************************/
/*  void spin_init(SpinLockObj* ptr)    */
/****************************************/
	.type   spin_init, %function
	.globl	spin_init
spin_init:
	MOV		w1, #0
	STR		w1, [X0]
	DSB		SY
	RET

/****************************************/
/*  void spin_lock(SpinLockObj* ptr)    */
/****************************************/
	.type   spin_lock, %function
	.globl	spin_lock
spin_lock:
	DMB		SY
	MOV		w1, #1
Loop:
	LDXR	w2, [x0]
	CMP		w2, #0
	BNE		wait_event
	STXR	w2, w1, [x0]
	CMP		w2, #0
	BNE		Loop
	DMB		SY
	RET
wait_event:
#if defined(USE_WFE_SEV)
	WFE
#endif
	B		Loop

/****************************************/
/*  void spin_unlock(SpinLockObj* ptr)  */
/****************************************/
	.type   spin_lock, %function
	.globl	spin_unlock
spin_unlock:
	DMB		SY
	LDR		w1, [x0]
	CMP		w1, #1
	BNE		err_unlock
	MOV		w1, #0
	DMB		SY
	STR		w1, [x0]
	DSB		SY
#if defined(USE_WFE_SEV)
	SEV
#endif
	RET
err_unlock:
	ldr		x8, =0x0000000000000001
	ldr		x8, [x8]
	b		err_unlock

/****************************************/
/*  int spin_trylock(SpinLockObj* ptr)  */
/*    return 1:success 0:fail			*/
/****************************************/
	.type   spin_trylock, %function
	.globl	spin_trylock
spin_trylock:
	DMB		SY
	MOV		w1, #1
	LDXR	w2, [x0]
	CMP		w2, #0
	BNE		fail_ret
	STXR	w2, w1, [x0]
	CMP		w2, #0
	BNE		fail_ret
	MOV		x0, #1
	B		spin_trylock_ret
fail_ret:
	MOV		x0, #0
spin_trylock_ret:
	DMB		SY
	RET
