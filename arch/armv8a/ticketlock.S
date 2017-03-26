/*
 * Copyright (C) 2017 sjbiwa All rights reserved.
 */

/*
 * ticketlock.S
 *
 *  Created on: 2017/03/09
 *      Author: biwa
 */

#include "armv8.h"
#include "armv8reg.h"

	.text

/****************************************/
/*  void spin_init(SpinLockObj* ptr)    */
/****************************************/
	.type   spin_init, %function
	.globl	spin_init
spin_init:
	MOV		W1, #0
	STR		W1, [X0]
	DSB		SY
	RET

/****************************************/
/*  void spin_lock(SpinLockObj* ptr)    */
/****************************************/
	.type   spin_lock, %function
	.globl	spin_lock
spin_lock:
1:
	LDXR		W1, [X0]
	ADD			W2, W1, #0x00010000
	STXR		W3, W2, [X0]
	CMP			W3, #0
	BNE			1b
	LSR			W1, W1, #16
2:
	LDRH		W2, [X0]
	CMP			W1, W2
	BEQ			3f
	WFE
	B			2b
3:

	DMB			SY
	RET


/****************************************/
/*  void spin_unlock(SpinLockObj* ptr)  */
/****************************************/
	.type   spin_lock, %function
	.globl	spin_unlock
spin_unlock:
	DMB			SY
	LDRH		W1, [X0]
	ADD			W1, W1, #1
	STRH		W1, [X0]
	DSB			SY
	SEV
	RET

/****************************************/
/*  int spin_trylock(SpinLockObj* ptr)  */
/*    return 1:success 0:fail			*/
/****************************************/
	.type   spin_trylock, %function
	.globl	spin_trylock
spin_trylock:
	LDXR		W1, [X0]
	MOV			W3, #0
	EOR			W3, W3, W3, ROR #16
	CMP			W3, #0  /* W3:high16 == W3:low16 ? */
	BNE			1f
	ADD			W1, W1, #0x00010000
	STXR		W3, W1, [X0]
	CMP			W3, #0
	BNE			2f
	DMB			SY
	MOV			X0, #1
	RET
1:
	CLREX
2:
	DMB			SY
	MOV			X0, #0
	RET
