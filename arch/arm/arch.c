/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * arch.c
 *
 *  Created on: 2014/12/07
 *      Author: biwa
 */

#include "kernel.h"
#include "bwos.h"
#include "arm.h"
#include "cp15reg.h"
#include "mpcore.h"
#include "arch_local.h"

extern void board_register_normal_memory(void);

extern void	_entry_stub(void);
extern char __heap_start;

CpuStruct* cpu_struct_pointer[CPU_NUM]; /* 各コアが自身のcpu_structを取り出すためのもの */

/* 初期タスクの生成パラメータ */
TaskCreateInfo	_init_task_create_param = {
	NULL, CPU_CORE0|TASK_ACT|TASK_SYS, NULL, 1024, 0, 0, (void*)0
};


/* 最初に1になるビットを左側から探す */
static inline int32_t bit_srch_l(uint32_t val)
{
	int32_t ret = 31;
	while ( (0 <= ret) && ((val & (0x01u<<ret)) == 0) ) {
		ret--;
	}
	return ret;
}

static inline void arch_init_task(TaskStruct* task, void* cre_param)
{
	uint32_t*		ptr;
	uint32_t*		usr_stack;
#if !defined(NO_USE_SEPARATE_STACK)
	/* SVC stack */
	ptr = (uint32_t*)((PtrInt_t)(task->init_sp) + task->stack_size - TASK_FRAME_SIZE);
	ptr = PRE_ALIGN_BY(ptr, STACK_ALIGN);
	task->save_sp = ptr;
#endif
	/* USR/SYS stack */
	usr_stack = (uint32_t*)((PtrInt_t)(task->usr_init_sp) + task->usr_stack_size);
	usr_stack = PRE_ALIGN_BY(usr_stack, STACK_ALIGN);

	/* setup task-context */
	ptr[TASK_FRAME_STUB] = (PtrInt_t)_entry_stub;
	ptr[TASK_FRAME_PC] = (PtrInt_t)task->entry;
	ptr[TASK_FRAME_PSR] = FLAG_T | ((task->task_attr&TASK_SYS)?MODE_SYS:MODE_USR);
	ptr[TASK_FRAME_FPEXC] = 0x00000000;
	ptr[TASK_FRAME_SPSR] = 0x00000000;
	ptr[TASK_FRAME_SP_USR] = (PtrInt_t)usr_stack;
	ptr[TASK_FRAME_LR_USR] = (PtrInt_t)task_dormant;
	/* Task entry arg */
	ptr[TASK_FRAME_R0] = (uint32_t)cre_param;
	ptr[TASK_FRAME_R1] = 0;
	ptr[TASK_FRAME_R2] = 0;
	ptr[TASK_FRAME_R3] = 0;
}

void arch_init_task_create(TaskStruct* task)
{
	/* 初期タスクのスタック確保は sys_malloc系を使う */
#if !defined(NO_USE_SEPARATE_STACK)
	task->stack_size = TASK_SVC_STACK_SIZE;
	task->init_sp = __sys_malloc_align_body(task->stack_size, STACK_ALIGN);
#endif
	task->usr_init_sp = __sys_malloc_align_body(task->usr_stack_size, STACK_ALIGN);

	/* ::init_spまたはusr_init_spがnullの時はシステムエラー */

	arch_init_task(task, NULL);
}

int arch_task_create(TaskStruct* task, void* cre_param)
{
	int ret = RT_OK;

	/* スタック領域の確保 */
#if !defined(NO_USE_SEPARATE_STACK)
	/* SVC_stack */
	task->stack_size = TASK_SVC_STACK_SIZE;
	/* stackをヒープから確保 */
	task->init_sp = __sys_malloc_align(task->stack_size, STACK_ALIGN);
	if ( !(task->init_sp) ) {
		goto ERR_RET;
	}
#endif
	/* USR stack */
	if ( task->usr_init_sp == 0 ) {
		/* stackをヒープから確保 */
		task->usr_init_sp = __sys_malloc_align(task->usr_stack_size, STACK_ALIGN);
		if ( !(task->usr_init_sp) ) {
#if !defined(NO_USE_SEPARATE_STACK)
			__sys_free(task->init_sp);
#endif
			goto ERR_RET;
		}
	}

	arch_init_task(task, cre_param);

	/* FPU(VFP)退避用領域の確保 */
	if ( task->task_attr & TASK_FPU ) {
		uint32_t*		ptr = task->save_sp;
		ptr[TASK_FRAME_FPEXC] = 0x40000000;
		task->arch_tls = __sys_malloc_align(8*32+1, STACK_ALIGN); /* D0-D31, FPSCR */
		if ( !(task->arch_tls) ) {
			ret = RT_ERR;
		}
	}
	return ret;

ERR_RET:
	return RT_ERR;
}

void arch_task_active(TaskStruct* task, void* act_param)
{
	uint32_t*		ptr = task->save_sp;
	ptr[TASK_FRAME_R1] = (uint32_t)act_param;
}

static void
smp0_handler(uint32_t irqno, void* info)
{
}

void arch_system_preinit(uint32_t cpuid)
{
	if ( cpuid == MASTER_CPU_ID ) {
		for (int cpuid=0; cpuid < CPU_NUM; cpuid++ ) {
			cpu_struct_pointer[cpuid] = &cpu_struct[cpuid];
		}
		debug_print_init();
	}

	tprintf("CPU= = %d\n", CPUID_get());
	tprintf("SCTLR = %08X\n", SCTLR_get());
	tprintf("ACTLR = %08X\n", ACTLR_get());
	tprintf("SCR = %08X\n", SCR_get());
	tprintf("ID_PFR0 = %08X\n", ID_PFR0_get());
	tprintf("ID_PFR1 = %08X\n", ID_PFR1_get());
	tprintf("ID_DFR0 = %08X\n", ID_DFR0_get());

	/* PROC_ID / ASID を初期化 */
	CONTEXTIDR_set(0);

	/* データキャッシュinvalidate */
	uint32_t clid = CLIDR_get();
	tprintf("CLID = %08X\n", clid);

	uint32_t louu_value = (clid >> 27) & 0x07; /* Level of Unification Uniprocessor for the cache hierarchy */
	int32_t ix;
	/* 下位キャッシュ層から順番にinvalidate */
	for (ix=6; 0 <= ix; ix--) {
		if ( (0x07 << (ix*3)) & clid ) {
			/* マスタCPU または 単体CPU内キャッシュのとき */
			if ( (cpuid == MASTER_CPU_ID) || ((ix+1) <= louu_value) ) {
				CSSELR_set(ix<<1);
				uint32_t ccsid = CCSIDR_get();
				uint32_t sets = ((ccsid >> 13) & 0x7fff) + 1;
				uint32_t asos = ((ccsid >> 3) & 0x3ff) + 1;
				uint32_t sizes_len = (ccsid & 0x7) + 4;
				int32_t asos_len = bit_srch_l(asos-1) + 1;
				tprintf("L:%d %d %d %d %d TOTAL=%dbytes\n", ix, sets, asos, sizes_len, asos_len, (0x1<<sizes_len)*sets*asos);
				if ( (0 < asos_len) && (0 < sizes_len) ) {
					uint32_t w, s;
					for (w = 0; w < asos; w++) {
						for (s = 0; s < sets; s++) {
							uint32_t val = (w<<(32-asos_len))|(s<<(sizes_len))|(ix<<1);
							DCISW_set(val);
						}
					}
				}
			}
		}
	}

	/* coprocessor CP10/CP11 有効化 (VFP full access) */
	uint32_t cpval = CPACR_get();
	cpval &= 0x0fffffff;
	cpval |= ((0x3<<(10*2))|(0x3<<(11*2)));
	CPACR_set(cpval);
	__isb();

	/* テスト用にVFPレジスタ初期化 */
	 FPEXC_set(0x40000000);
	 FPSCR_set(0x00000000);

	/* ページテーブル作成 */
	mmgr_init(cpuid);

	/* 割り込みコントローラ初期化 */
	arch_irq_init(cpuid);

	/* コア間割り込みハンドラ登録 */
	if ( cpuid == MASTER_CPU_ID ) {
		__irq_add_handler(0, smp0_handler, NULL);
		__irq_add_handler(1, smp0_handler, NULL);
	}
	__irq_set_enable(0, IRQ_ENABLE, CPU_SELF);
	__irq_set_enable(1, IRQ_ENABLE, CPU_SELF);
}

void arch_register_st_memory()
{
	/* 起動時メモリ登録 */
	__st_malloc_init(&__heap_start, PTRVAR(END_MEM_ADDR+1) - PTRVAR(&__heap_start));
}
void arch_register_normal_memory(void)
{
	board_register_normal_memory();
}

void arch_system_postinit(uint32_t cpuid)
{
}

void _delayed_dispatch(void)
{
}

bool arch_can_dispatch(void)
{
extern	uint32_t _irq_level[CPU_NUM]; /* 多重割り込みレベル */
	bool ret = false;
	if ( _irq_level[CPUID_get()] == 0 ) {
		ret = true;
	}
	return ret;
}

void _dispatch(void)
{
	TaskStruct* ctask = cpu_struct[CPUID_get()].ctask;
	TaskStruct* ntask = cpu_struct[CPUID_get()].ntask;
	cpu_struct[CPUID_get()].ctask = ntask;
	_switch_to(ctask, ntask, 0);
}

void ipi_request_dispatch(uint32_t other_cpu_list)
{
	__dsb();
	iowrite32(GICD_SGIR, other_cpu_list << 16);
}

void ipi_request_dispatch_one(CpuStruct* cpu)
{
	uint32_t cpuid = cpu->cpuid;
	__dsb();
	iowrite32(GICD_SGIR, 0x00010000u << cpuid);
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
