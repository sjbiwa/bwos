	.syntax unified
	.thumb
	.thumb_func
//
// dispatch.S
//

#include "arm.h"

#if defined(USE_VFP)
#define	EXC_RET_VFP			(0x01<<4)
#endif


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
	cpsie		i
	svc			#0
	cpsid		i
	bx			lr


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
**
**
**   EXC_RETURN.bit4==0(VFP有効かつVFPが使われている)場合
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
**				| S16				|+36
**				|  ～				|
**				| S31				|+96
**				----------------------
**				| R0				|+100
**				| R1				|+104
**				| R2				|+108
**				| R3				|+112
**				| R12				|+116
**				| LR				|+120
**				| PC(return address)|+124
**				| xPSR              |+128
**				---------------------
**				| S0				|+132
**				|  ～				|
**				| S15				|+192
**				| FPSCR				|+196
**				----------------------
**				| (padding)			|+200
**				---------------------
**   beforePSP	|					|+200 (+4)
**
*/
	.type   _dispatch_handler, %function
_dispatch_handler:

	/* 割り込み禁止 */
	cpsid		i
	/* ctask != ntask の確認 (_dispatch_handlerまでは割り込み許可なので */
	/* その間にctask==ntaskになる場合のチェック)						*/
	ldr			r2, =cpu_struct
	ldr			r0, [r2, #CPUSTRUCT_CTASK] /* r0 <- ctask */
	ldr			r1, [r2, #CPUSTRUCT_NTASK] /* r1 <- ntask */
	str			r1, [r2, #CPUSTRUCT_CTASK] /* copy ntask -> ctask */
	/* 割り込み許可 */
	cpsie		i
	/* 以降は割り込みは受け付ける。										 */
	/* ハンドラモードなのでPendSV割り込みは発生しないのでdispatchしない  */
	/* ctaskはこの処理でRUNするタスク。それと違うタスクが再スケジュール  */
	/* された時だけ PendSV要求があればよい。                             */

	cmp			r0, #0
	beq			_restore_context
	/* ctask==ntask (and ctask != 0) の時はなにもせず終了(現タスクを継続) */
	cmp			r0, r1
	beq			_dispatch_fin

_save_context:
	mrs			r3, PSP
#if __ARM_ARCH == 7
#if USE_VFP
	tst			lr, #EXC_RET_VFP
	vstmdbeq	r3!, {s16-s31}
#endif
	stmdb		r3!, {r4-r11,lr}
#else
	subs		r3, #(4*9)
	stm			r3!, {r4-r7}
	mov			r4, r8
	mov			r5, r9
	mov			r6, r10
	mov			r7, r11
	stm			r3!, {r4-r7}
	mov			r4, lr
	stm			r3!, {r4}
	subs		r3, #(4*9)
#endif
	str			r3, [r0, #SAVE_SP]

_restore_context:
	/* check _ntask != 0 (IDLE?) */
	cmp			r1, #0
	beq			_idle_loop

	/* R1: _ntask 他のレジスタは空き */
	ldr			r3, [r1, #SAVE_SP]
#if __ARM_ARCH == 7
	ldmia		r3!, {r4-r11,lr}
#if USE_VFP
	tst			lr, #EXC_RET_VFP
	vldmiaeq	r3!, {s16-s31}
#endif
#else
	adds		r3, #(4*4)
	ldm			r3!, {r0,r4-r7}
	mov			r8, r0
	mov			r9, r4
	mov			r10, r5
	mov			r11, r6
	mov			lr, r7
	subs		r3, #(4*9)
	ldm			r3!, {r4-r7}
	adds		r3, #(4*5)
#endif
	msr			PSP, r3

_dispatch_fin:

	bx			lr

/*
** _svc_entry
**  _pendsvc_entry
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
	.global _pendsvc_entry
	.type   _pendsvc_entry, %function
_svc_entry:
_pendsvc_entry:
	b			_dispatch_handler


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
	ldr			r0, =stack_idle
#if __ARM_ARCH == 7
	bic			r0, r0, #0x7 /* 8byte align */
#else
	movs		r1, #0x7
	bics		r0, r1		/* 8byte align */
#endif
	subs		r0, #(8*4)
	ldr			r6, =idle_task  /* PC */
	ldr			r7, =(0x01<<24) /* xPSR(T_BIT) */
#if __ARM_ARCH == 7
	stmia		r0, {r0-r7}
#else
	stmia		r0!, {r0-r7}
	subs		r0, #(4*8)
#endif
	msr			PSP, r0
	ldr			r0, =0xFFFFFFFD; /* Threadモード/Procesスタック に遷移 */
	bx			r0
