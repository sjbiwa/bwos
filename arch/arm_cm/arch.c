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

extern char __heap_start;

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
	ptr[TASK_FRAME_PC] = (PtrInt_t)_entry_stub;
	ptr[TASK_FRAME_PSR] = FLAG_T;
	ptr[TASK_FRAME_EXC_RETURN] = 0xFFFFFFFD; /* Thread/ process */
	/* Task entry arg */
	ptr[TASK_FRAME_R0] = (uint32_t)cre_param;
	ptr[TASK_FRAME_R1] = 0; /* task_active()で設定可 */
	ptr[TASK_FRAME_R2] = 0;
	ptr[TASK_FRAME_R3] = 0;
	ptr[TASK_FRAME_R12] = (PtrInt_t)task->entry; /* _entry_stubからここに分岐する */
	ptr[TASK_FRAME_LR] = (PtrInt_t)task_dormant;
}

void arch_init_task_create(TaskStruct* task)
{
	arch_init_task(task, NULL);
}

int arch_task_create(TaskStruct* task, void* cre_param)
{
	int ret = RT_OK;
	arch_init_task(task, cre_param);

	return ret;
}

void arch_task_active(TaskStruct* task, void* act_param)
{
	uint32_t*		ptr = task->save_sp;
	ptr[TASK_FRAME_R1] = (uint32_t)act_param;
}

void arch_system_preinit(uint32_t cpuid)
{
	Board_SystemInit();
	Board_Init();

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

void ipi_request_dispatch(uint32_t other_cpu_list)
{
}

void ipi_request_dispatch_one(CpuStruct* cpu)
{
}

void init_task_arch_depend(void)
{
extern void init_task_board_depend(void);
	init_task_board_depend();
}

void system_init(void)
{
	extern uint32_t	handler_entry;
	SCB->VTOR = (uint32_t)(&handler_entry);
	SCB->CCR |= SCB_CCR_STKALIGN_Msk | SCB_CCR_UNALIGN_TRP_Msk;
	SCB->SHP[MEM_MANAGE_ENTRY_NO-4]		= 0x00;
	SCB->SHP[BUS_FAULT_ENTRY_NO-4]		= 0x00;
	SCB->SHP[USAGE_FAULT_ENTRY_NO-4]	= 0x00;
	SCB->SHP[SVC_ENTRY_NO-4]			= 0xff;
	SCB->SHP[PENDSVC_ENTRY_NO-4]		= 0xff;
	SCB->SHP[SYSTICK_ENTRY_NO-4]		= 0x80;
	SCB->SHCSR = SCB_SHCSR_USGFAULTENA_Msk|SCB_SHCSR_BUSFAULTENA_Msk|SCB_SHCSR_MEMFAULTENA_Msk;
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
