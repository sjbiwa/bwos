/*
 * Copyright (C) 2016 sjbiwa All rights reserved.
 */

/*
 * ticketlock.S
 *
 *  Created on: 2016/01/21
 *      Author: biwa
 */

	.syntax unified
	.thumb
	.thumb_func

#include "arm.h"
#include "cp15reg.h"

/****************************************/
/*  void spin_init(SpinLockObj* ptr)    */
/****************************************/
	.type   spin_init, %function
	.globl	spin_init
spin_init:
	MOV			R1, #0
	STR			R1, [R0]
	DSB
	BX			LR

/****************************************/
/*  void spin_lock(SpinLockObj* ptr)    */
/****************************************/
	.type   spin_lock, %function
	.globl	spin_lock
spin_lock:
1:
	LDREX		R1, [R0]
	ADD			R2, R1, #0x00010000
	STREX		R3, R2, [R0]
	CMP			R3, #0
	BNE			1b
	MOV			R1, R1, LSR #16
2:
	LDRH		R2, [R0]
	CMP			R1, R2
	WFENE
	BNE			2b

	DMB
	BX			LR


/****************************************/
/*  void spin_unlock(SpinLockObj* ptr)  */
/****************************************/
	.type   spin_lock, %function
	.globl	spin_unlock
spin_unlock:
	DMB
	LDRH		R1, [R0]
	ADD			R1, R1, #1
	STRH		R1, [R0]
	DSB
	SEV
	BX			LR

/****************************************/
/*  int spin_trylock(SpinLockObj* ptr)  */
/*    return 1:success 0:fail			*/
/****************************************/
	.type   spin_trylock, %function
	.globl	spin_trylock
spin_trylock:
1:
	LDREX		R1, [R0]
	MOV			R3, #0
	SUBS		R2, R1, R1, ror #16
	ADDEQ		R1, R1, #0x00010000
	STREXEQ		R3, R1, [R0]

	CMP			R3, #0
	BNE			1b

	CMP			R2, #0
	DMBEQ
	MOVEQ		R0, #1
	MOVNE		R0, #0
	BX			LR
