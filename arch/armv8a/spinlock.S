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

/****************************************/
/*  void spin_init(SpinLockObj* ptr)    */
/****************************************/
	.type   spin_init, %function
	.globl	spin_init
spin_init:
	MOV		w1, #0
	STR		w1, [X0]
	DSB
	RET

/****************************************/
/*  void spin_lock(SpinLockObj* ptr)    */
/****************************************/
	.type   spin_lock, %function
	.globl	spin_lock
spin_lock:
	MOV		w1, #1
Loop:
	LDREX	w2, [x0]
	CMP		w2, #0
	WFENE
	STREXEQ	w2, w1, [x0]
	CMPEQ	w2, #0
	BNE		Loop
	DMB
	RET


/****************************************/
/*  void spin_unlock(SpinLockObj* ptr)  */
/****************************************/
	.type   spin_lock, %function
	.globl	spin_unlock
spin_unlock:
	MOV w1, #0
	DMB
	STR w1, [x0]
	DSB
	SEV
	RET

/****************************************/
/*  int spin_trylock(SpinLockObj* ptr)  */
/*    return 1:success 0:fail			*/
/****************************************/
	.type   spin_trylock, %function
	.globl	spin_trylock
spin_trylock:
	MOV		w1, #1
	LDREX	w2, [x0]
	CMP		w2, #0
	STREXEQ	w2, w1, [x0]
	CMPEQ	w2, #0
	MOVEQ	w0, #1
	MOVNE	w0, #0
	DMB
	RET
