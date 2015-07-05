	.syntax unified
	.thumb
	.thumb_func
//
// dispatch.S
//

#include "arm.h"

	.text
	.balign 4

/**
** _dispatch
**  割り込み禁止状態で呼び出すこと
**  スレッド状態であること
*/
	.global _dispatch
	.type   _dispatch, %function

_dispatch:
	/* SVCコールするために割り込み許可する
	**  自タスク(ctask)は休止状態になっている
	**  ※ 割り込み許可->SVC までの間に外部割込みが発生した場合にどうなるか?
	*/
	mov		r0, #0
	msr		PRIMASK, r0
	svc		#0
	bx		lr


/* _dispatch_handler
**    ハンドラモード / 割り込み禁止状態
**    LR: EXC_RETURN
**				----------------------
** 		PSP		| R0				|+0
**				| R1				|+4
**				| R2				|+8
**				| R3				|+12
**				| R12				|+16
**				| LR				|+20
**				| PC(return address)|+24
**				| xPSR              |+28
**				---------------------
**				| (padding)			|+32
**				---------------------
**   beforePSP	|					|+32 (+4)
**
**			↓↓↓ dispatch実行後
**
**				----------------------
** 		PSP		| R4				|+0
**				| R5				|+4
**				| R6				|+8
**				| R7				|+12
**				| R8				|+16
**				| R9				|+20
**				| R10				|+24
**				| R11				|+28
**				---------------------
**				| EXC_RETURN		|+32
**				----------------------
**				| R0				|+36
**				| R1				|+40
**				| R2				|+44
**				| R3				|+48
**				| R12				|+52
**				| LR				|+56
**				| PC(return address)|+60
**				| xPSR              |+64
**				---------------------
**				| (padding)			|+68
**				---------------------
**   beforePSP	|					|+68 (+4)
**
*/
	.type   _dispatch_handler, %function
_dispatch_handler:
	/* ctask != ntask の確認 */
	ldr		r2, =cpu_struct
	ldr		r0, [r2, #CPUSTRUCT_CTASK] /* r0 <- ctask */
	ldr		r1, [r2, #CPUSTRUCT_NTASK] /* r1 <- ntask */
	cmp		r0, #0
	beq		_restore_context
	/* ctask==ntask (and ctask != 0) の時はなにもせず終了(現タスクを継続) */
	cmp		r0, r1
	beq		_dispatch_fin

_save_context:
	mrs		r3, PSP
	stmdb	r3!, {r4-r11,lr}
	str		r3, [r0, #SAVE_SP]

_restore_context:
	/* copy _ntask -> _ctask */
	str		r1, [r2, #CPUSTRUCT_CTASK]

	/* check _ntask != 0 (IDLE?) */
	cmp		r1, #0
	beq		_idle_loop

	/* R1: _ntask 他のレジスタは空き */
	ldr		r3, [r1, #SAVE_SP]
	ldmia	r3!, {r4-r11,lr}
	msr		PSP, r3

_dispatch_fin:
	bx		lr

/*
** _svc_entry
*/
/************************************************************************************/
/* 再度割り込み禁止にしてdispatch処理実行											*/
/* --------------------------------------------------------------------------------	*/
/*  ※割り込み許可->SVC->割り込み禁止 までの間に割り込みが発生した場合の懸案事項	*/
/*    大前提として、ここに処理が来るのは、ctask==自タスク であるはず            	*/
/*    懸案は自タスクがWAIT状態ではない場合                                     		*/
/*    1.割り込みハンドラにて 自タスク(ctask)がREADY状態になった場合           		*/
/*      ctask!=ntaskであれば　preemptでdispatchを継続で問題なし                		*/
/*      ctask==ntaskとなる場合が有り得る。このときはdispatchせずにそのまま復帰		*/
/*	対策として、割り込み禁止後に再度ctask!=ntaskであることを確認すればよい			*/
/*   																				*/
/************************************************************************************/
	.global _svc_entry
	.type   _svc_entry, %function
_svc_entry:
	mov		r0, #1
	msr		PRIMASK, r0
	b		_dispatch_handler
/*
**  _pendsvc_entry
**   PendSVC割り込みハンドラ
**    処理はSVCハンドラと同じ
*/
	.global _pendsvc_entry
	.type   _pendsvc_entry, %function
_pendsvc_entry:
	b		_svc_entry


/*
**	_idle_loop
**  ここにはハンドラモードで呼ばれるので、EXC_RETURNでアイドルタスクに遷移する
**				----------------------
**				| R0				|+36
**				| R1				|+40
**				| R2				|+44
**				| R3				|+48
**				| R12				|+52
**				| LR				|+56
**				| PC(return address)|+60
**				| xPSR              |+64
**				---------------------
*/
	.type   _idle_loop, %function
_idle_loop:
	ldr		lr, =stack_idle
	bic		lr, lr, #0x7 /* 8byte align */
	ldr		r6, =idle_task
	ldr		r7, =(0x01<<24) /* T_BIT */
	stmdb	lr!, {r0-r7}
	msr		PSP, lr
	ldr		lr, =0xFFFFFFFD; /* Threadモード/Procesスタック に遷移 */
	bx		lr
