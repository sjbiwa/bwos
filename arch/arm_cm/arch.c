/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * arch.c
 *
 *  Created on: 2015/07/01
 *      Author: biwa
 */

#include "kernel.h"
#include "bwos.h"
#include "arm.h"
#include "my_board.h"

#define	SHP_NO(n)			((n)+12)

extern char __heap_start;

extern void board_register_normal_memory(void);
extern void board_init_task_depend(void);

extern void	_entry_stub(void);

static inline void arch_init_task(TaskStruct* task, void* cre_param)
{
	uint32_t*		ptr;
	uint32_t*		usr_stack;
	/* USR/SYS stack */
	usr_stack = (uint32_t*)((PtrInt_t)(task->usr_init_sp) + task->usr_stack_size);
	usr_stack = PRE_ALIGN_BY(usr_stack, STACK_ALIGN);
	ptr = (uint32_t*)((PtrInt_t)usr_stack - TASK_FRAME_SIZE);
	task->save_sp = ptr;

	/* setup task-context */
	ptr[TASK_FRAME_EXC_RETURN] = 0xFFFFFFFD; /* Thread/process/Basic */
	/* Task entry arg */
	ptr[TASK_FRAME_R0] = (uint32_t)cre_param;
	ptr[TASK_FRAME_R1] = 0; /* task_active()で設定可 */
	ptr[TASK_FRAME_R2] = 0;
	ptr[TASK_FRAME_R3] = 0;
	ptr[TASK_FRAME_R12] = 0;
	ptr[TASK_FRAME_LR] = (PtrInt_t)task_dormant;
	ptr[TASK_FRAME_PC] = (PtrInt_t)task->entry;
	ptr[TASK_FRAME_PSR] = FLAG_T;
}

void arch_init_task_create(TaskStruct* task)
{
	task->usr_init_sp = sys_malloc_align_body(task->usr_stack_size, STACK_ALIGN);
	/* ::init_spまたはusr_init_spがnullの時はシステムエラー */

	arch_init_task(task, NULL);
}

int arch_task_create(TaskStruct* task, void* cre_param)
{
	int ret = RT_OK;

	/* USR stack */
	if ( task->usr_init_sp == 0 ) {
		/* stackをヒープから確保 */
		task->usr_init_sp = __sys_malloc_align(task->usr_stack_size, STACK_ALIGN);
		if ( !(task->usr_init_sp) ) {
			goto ERR_RET;
		}
	}
	arch_init_task(task, cre_param);
	return ret;

ERR_RET:
	return RT_ERR;
}

void arch_task_active(TaskStruct* task, void* act_param)
{
	uint32_t*		ptr = task->save_sp;
	ptr[TASK_FRAME_R1] = (uint32_t)act_param;
}

void arch_system_preinit(uint32_t cpuid)
{
	debug_print_init();
	board_system_preinit();

	/* 割り込みコントローラ初期化 */
	arch_irq_init(cpuid);
}

void arch_register_st_memory()
{
	/* 起動時メモリ登録 */
	st_malloc_init(&__heap_start, PTRVAR(END_MEM_ADDR+1) - PTRVAR(&__heap_start));
}

void arch_register_normal_memory(void)
{
	board_register_normal_memory();
}

void arch_system_postinit(uint32_t cpuid)
{
}

bool arch_can_dispatch(void)
{
	bool ret = true;
	if ( xpsr_get() & 0x1ff ) {
		ret = false;
	}
	return ret;
}

void _delayed_dispatch(void)
{
	/* PendSVC設定 */
	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

/* SVCコールするために割り込み許可する
**  自タスク(ctask)は休止状態になっている
**  ※ 割り込み許可->SVC までの間に外部割込みが発生した場合にどうなるか?
*/
void _dispatch(void)
{
#if defined(USE_SVC_DISPATCH)
	/* API呼び出しでのdispatchにSVCを使う */
	irq_enable();
	__svc();
	irq_disable();
#else
	/* API呼び出しでのdispatchもPendSVを使う */
	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
	irq_enable();
	__dsb();
	while (SCB->ICSR & SCB_ICSR_PENDSVSET_Msk);
	irq_disable();
#endif
}


void ipi_request_dispatch(uint32_t other_cpu_list)
{
}

void ipi_request_dispatch_one(CpuStruct* cpu)
{
}

void arch_init_task_depend(void)
{
extern void board_init_task_depend(void);
	board_init_task_depend();
}

static inline void setup_sys_int(uint32_t id, uint32_t pri)
{
#if __ARM_ARCH == 7
	SCB->SHP[id] = pri;
#else
	id -= 4;
	uint32_t pri_off = id / 4;
	uint32_t pri_pos = (id % 4) * 8;
	SCB->SHP[pri_off] = (SCB->SHP[pri_off] & ~(0xff << pri_pos)) | (pri << pri_pos);
#endif
}
void system_init(void)
{
	extern uint32_t	handler_entry;
#if defined(HAVE_VTOR)
	SCB->VTOR = (uint32_t)(&handler_entry);
#endif

	SCB->CCR |= SCB_CCR_STKALIGN_Msk | SCB_CCR_UNALIGN_TRP_Msk;

	SCB->SHCSR = 0;
#if defined(MPU_FAULT_PRIORITY)
	setup_sys_int(SHP_NO(MemoryManagement_IRQn), MPU_FAULT_PRIORITY);
	SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
#endif
#if defined(BUS_FAULT_PRIORITY)
	setup_sys_int(SHP_NO(BusFault_IRQn), BUS_FAULT_PRIORITY);
	SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA_Msk;
#endif
#if defined(USAGE_FAULT_PRIORITY)
	setup_sys_int(SHP_NO(UsageFault_IRQn), USAGE_FAULT_PRIORITY);
	SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;
#endif
#if defined(SVCALL_PRIORITY)
	setup_sys_int(SHP_NO(SVCall_IRQn), SVCALL_PRIORITY);
#endif
#if defined(DEBUG_MONITOR_PRIORITY)
	setup_sys_int(SHP_NO(DebugMonitor_IRQn), DEBUG_MONITOR_PRIORITY);
#endif
#if defined(PENDSV_PRIORITY)
	setup_sys_int(SHP_NO(PendSV_IRQn), PENDSV_PRIORITY);
#endif
#if defined(SYSTICK_PRIORITY)
	setup_sys_int(SHP_NO(SysTick_IRQn), SYSTICK_PRIORITY);
#endif
#if defined(USE_VFP)
	SCB->CPACR = 0x00F00000; /* enable VFP */
	FPU->FPCCR = FPU_FPCCR_ASPEN_Msk | FPU_FPCCR_LSPEN_Msk;
	FPU->FPDSCR = 0x00000000;
#endif
	__dsb();
}

/* ０除算呼び出し */
void raise(void)
{
	irq_disable();
	tprintf("0 divided\n");
	/* ０番地にアクセスして例外を発生させる */
	uint32_t value = *((volatile uint32_t*)0);
	for (;;);
}
