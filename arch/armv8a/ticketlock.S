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
/* This is ticketlock */
/* +0                            +2                           +4            +8            */
/* -------------------------------------------------------------------------------------- */
/* |        処理中のID          |       要求待ちID          |     MPIDR     | LR(W30)   | */
/* -------------------------------------------------------------------------------------- */
/* |  処理が完了したら1だけ足す | 要求時に1足す(exclusive)  |                             */
/* |  足すのは処理中のCPUのみ   |                           |                             */
/* |  読み出しは要求待ちCPU全て |                           |                             */
/**/
/* typedef	struct {								*/
/*    uint16_t	owner;		 獲得中のID				*/
/*    uint16_t	req;		 リクエスタID			*/
/*    uint32_t	cpuid;		 獲得中のCPUID			*/
/*    uint32_t	lr;			 獲得中処理のLR			*/
/* } SpinLockObj;									*/

/****************************************/
/*  void spin_init(SpinLockObj* ptr)    */
/****************************************/
	.type   spin_init, %function
	.globl	spin_init
spin_init:
	MOV		W1, #0
	STR		W1, [X0]			/* req / owner	*/
	STR		W1, [X0, #4]		/* cpuid		*/
	STR		W1, [X0, #8]		/* LR			*/
	DSB		SY
	RET

/****************************************/
/*  void spin_lock(SpinLockObj* ptr)    */
/****************************************/
	.type   spin_lock, %function
	.globl	spin_lock
spin_lock:
1:
	LDXR		W1, [X0]				/* req:owner load(execlusive) */
	ADD			W2, W1, #0x00010000		/* req increment */
	STXR		W3, W2, [X0]			/* req:owner store(exclusive) */
	CBNZ		W3, 1b
	
	LSR			W1, W1, #16				/* prev-req => W1 */
2:
	LDRH		W2, [X0]				/* owner load */
	CMP			W1, W2					/* prev-req == owner ? */
	BEQ			3f
	WFE
	B			2b

3:
	DMB			SY

#if 1
/* save cpuid */
	MPIDR_EL1_get(x1)
	STR			w1, [x0, #4]			/* CPUID */
	STR			w30, [x0, #8]			/* LRの下位32bit */
#endif	

	RET


/****************************************/
/*  void spin_unlock(SpinLockObj* ptr)  */
/****************************************/
	.type   spin_lock, %function
	.globl	spin_unlock
spin_unlock:
	DMB			SY

#if 1
/* LOCKした時のCPUIDとunlockするCPUIDとの比較 */
	MPIDR_EL1_get(x1)
	LDR			w2, [x0, #4]
	CMP			w1, w2
	BNE			err_end
#endif

	LDRH		W1, [X0]				/* owner load */
	ADD			W1, W1, #1				/* owner increment */
	STRH		W1, [X0]				/* owner store */
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
	LDXR		W1, [X0]				/* req:owner load(exclusive) */
	EOR			W3, W1, W1, ROR #16		/* W1[31:16][15:0] EOR W1[15:0][31:16] => W3 (上位HW == 下位HW?) */
	CBNZ		W3, 1f
	ADD			W1, W1, #0x00010000		/* increment req */
	STXR		W3, W1, [X0]			/* req:owner store(exclusive) */
	CBNZ		W3, 2f
	DMB			SY
#if 1
/* save cpuid */
	MPIDR_EL1_get(x1)
	STR			w1, [x0, #4]			/* CPUID */
	STR			w30, [x0, #8]			/* LRの下位32bit */
#endif	
	
	MOV			X0, #1
	RET
1:
	CLREX
2:
	DMB			SY
	MOV			X0, #0
	RET

	
err_end:
	B			err_end
