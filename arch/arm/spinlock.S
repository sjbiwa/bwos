/*
 * spinlock.S
 *
 *  Created on: 2015/05/26
 *      Author: biwa
 */

	.syntax unified
	.cpu cortex-a7
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
	MOV		R1, #0
	STR		R1, [R0]
	DSB
	BX		LR

/****************************************/
/*  void spin_lock(SpinLockObj* ptr)    */
/****************************************/
	.type   spin_lock, %function
	.globl	spin_lock
spin_lock:
	MOV		R1, #1
Loop:
	LDREX	R2, [R0]
	CMP		R2, #0
	WFENE
	STREXEQ	R2, R1, [R0]
	CMPEQ	R2, #0
	BNE		Loop
	DMB
	BX		LR


/****************************************/
/*  void spin_unlock(SpinLockObj* ptr)  */
/****************************************/
	.type   spin_lock, %function
	.globl	spin_unlock
spin_unlock:
	MOV R1, #0
	DMB
	STR R1, [R0]
	DSB
	SEV
	BX		LR

/****************************************/
/*  int spin_trylock(SpinLockObj* ptr)  */
/*    return 1:success 0:fail			*/
/****************************************/
	.type   spin_trylock, %function
	.globl	spin_trylock
spin_trylock:
	MOV		R1, #1
	LDREX	R2, [R0]
	CMP		R2, #0
	STREXEQ	R2, R1, [R0]
	CMPEQ	R2, #0
	MOVEQ	R0, #1
	MOVNE	R0, #0
	DMB
	BX		LR
