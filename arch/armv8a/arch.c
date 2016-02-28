/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * arch.c
 *
 *  Created on: 2015/12/27
 *      Author: biwa
 */

#include "kernel.h"
#include "bwos.h"
#include "armv8.h"
#include "my_board.h"
#include "armv8reg.h"
#include "gicv3reg.h"
#include "memmgr.h"
#include "arch_local.h"

extern void board_register_normal_memory(void);
extern void board_init_task_depend(void);

extern void	_entry_stub(void);
extern char __heap_start;

uint32_t _irq_level[CPU_NUM]; /* 多重割り込みレベル */
uint8_t _dispatch_disable[CPU_NUM] = {0}; /* ディスパッチ禁止フラグ */

/* 初期タスクの生成パラメータ */
TaskCreateInfo	_init_task_create_param = {
	NULL, CPU_CORE0|TASK_ACT|TASK_SYS, NULL, 2048, 0, 0, (void*)0
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
	uint64_t*		ptr;
	uint64_t*		usr_stack;

	/* SVC stack */
	ptr = (uint64_t*)((PtrInt_t)(task->init_sp) + task->stack_size - TASK_FRAME_SIZE);
	ptr = PRE_ALIGN_BY(ptr, STACK_ALIGN);
	task->save_sp = ptr;

	if ( !(task->task_attr & TASK_SYS) ) {
		/* USR stack */
		usr_stack = (uint64_t*)((PtrInt_t)(task->usr_init_sp) + task->usr_stack_size);
		usr_stack = PRE_ALIGN_BY(usr_stack, STACK_ALIGN);
	}
	else {
		usr_stack = 0;
	}

	/* setup task-context */
	ptr[TASK_FRAME_STUB] = (uint64_t)_entry_stub;
	ptr[TASK_FRAME_PC] = (uint64_t)task->entry;
	ptr[TASK_FRAME_PSR] = (task->task_attr&TASK_SYS)?MODE_SYS:MODE_USR;
	ptr[TASK_FRAME_CPACR] = 0x00000000;
	ptr[TASK_FRAME_SP_EL0] = (uint64_t)usr_stack;
	ptr[TASK_FRAME_LR] = (uint64_t)task_dormant;
	/* Task entry arg */
	ptr[TASK_FRAME_X0] = (uint64_t)cre_param;
	ptr[TASK_FRAME_X1] = 0;
	ptr[TASK_FRAME_X2] = 0;
	ptr[TASK_FRAME_X3] = 0;
	ptr[TASK_FRAME_X4] = 4;
	ptr[TASK_FRAME_X5] = 5;
	ptr[TASK_FRAME_X6] = 6;
	ptr[TASK_FRAME_X7] = 7;
	ptr[TASK_FRAME_X8] = 8;
	ptr[TASK_FRAME_X9] = 9;
	ptr[TASK_FRAME_X10] = 10;
	ptr[TASK_FRAME_X11] = 11;
	ptr[TASK_FRAME_X12] = 12;
	ptr[TASK_FRAME_X13] = 13;
	ptr[TASK_FRAME_X14] = 14;
	ptr[TASK_FRAME_X15] = 15;
	ptr[TASK_FRAME_X16] = 16;
	ptr[TASK_FRAME_X17] = 17;
	ptr[TASK_FRAME_X18] = 18;
	ptr[TASK_FRAME_X19] = 19;
	ptr[TASK_FRAME_X20] = 20;
	ptr[TASK_FRAME_X21] = 21;
	ptr[TASK_FRAME_X22] = 22;
	ptr[TASK_FRAME_X23] = 23;
	ptr[TASK_FRAME_X24] = 24;
	ptr[TASK_FRAME_X25] = 25;
	ptr[TASK_FRAME_X26] = 26;
	ptr[TASK_FRAME_X27] = 27;
	ptr[TASK_FRAME_X28] = 28;
	ptr[TASK_FRAME_X29] = 29;
}

/* arch依存の初期タスク生成 */
void arch_init_task_create(TaskStruct* task)
{
	task->stack_size = TASK_SVC_STACK_SIZE;
	if ( task->task_attr & TASK_SYS ) {
		/* SYSタスク */
		/* タスク用スタックとSVC用スタックは共用 */
		task->stack_size += task->usr_stack_size;
		task->usr_init_sp = 0;
	}
	else {
		/* USRタスク */
		/* タスク用スタックとSVC用スタックは個別 */
		task->usr_init_sp = __sys_malloc_align_body(task->usr_stack_size, STACK_ALIGN);
	}
	task->init_sp = __sys_malloc_align_body(task->stack_size, STACK_ALIGN);

	arch_init_task(task, NULL);
}

/* arch依存のタスク生成 */
int arch_task_create(TaskStruct* task, void* cre_param)
{
	int ret = RT_OK;

	/* スタック領域の確保 */
	task->stack_size = TASK_SVC_STACK_SIZE;
	if ( task->task_attr & TASK_SYS ) {
		/* SYSタスク */
		task->stack_size += task->usr_stack_size;
		task->usr_init_sp = 0;
	}
	else {
		/* USRタスク */
		/* USRスタック確保 */
		task->usr_init_sp = __sys_malloc_align(task->usr_stack_size, STACK_ALIGN);
		if ( !(task->usr_init_sp) ) {
			goto ERR_RET;
		}
	}

	/* SYSスタック確保 */
	task->init_sp = __sys_malloc_align(task->stack_size, STACK_ALIGN);
	if ( !(task->init_sp) ) {
		goto ERR_RET;
	}

	arch_init_task(task, cre_param);

	/* FPU(VFP)退避用領域の確保 */
	if ( task->task_attr & TASK_FPU ) {
		uint64_t*		ptr = task->save_sp;
		ptr[TASK_FRAME_CPACR] = 0x00300000;
		task->arch_tls = __sys_malloc_align((32*2+2)*8, STACK_ALIGN); /* V0-V31(128bit), FPCR(64bit), FPSR(64bit) */
		if ( !(task->arch_tls) ) {
			ret = RT_ERR;
		}
		memset(task->arch_tls, 0, (32*2+2)*8);
	}
	return ret;

ERR_RET:
	if ( task->init_sp ) {
		__sys_free(task->init_sp);
	}
	if ( task->usr_init_sp ) {
		__sys_free(task->usr_init_sp);
	}
	return RT_ERR;
}

void arch_task_active(TaskStruct* task, void* act_param)
{
	uint64_t*		ptr = task->save_sp;
	ptr[TASK_FRAME_X1] = (uint64_t)act_param;
}

static void
smp0_handler(uint32_t irqno, void* info)
{
	//tprintf("smp handler:%d\n", CPUID_get());
}

void arch_system_preinit(uint32_t cpuid)
{
	if ( cpuid == MASTER_CPU_ID ) {
		debug_print_init();
	}
	tprintf("MPIDR = %08X%08X\n", hword(MPIDR_EL1_get()), lword(MPIDR_EL1_get()));
	tprintf("CPU = %d\n", cpuid);
	tprintf("SCTLR = %08X\n", (uint32_t)SCTLR_EL1_get());
	tprintf("ACTLR = %08X\n", (uint32_t)ACTLR_EL1_get());
	tprintf("ID_PFR0 = %08X\n", (uint32_t)ID_PFR0_EL1_get());
	tprintf("ID_PFR1 = %08X\n", (uint32_t)ID_PFR1_EL1_get());
	tprintf("ID_DFR0 = %08X\n", (uint32_t)ID_DFR0_EL1_get());

	/* PROC_ID / ASID を初期化 */
	CONTEXTIDR_EL1_set(0);

	/* データキャッシュinvalidate */
	uint32_t clid = CLIDR_EL1_get();
	tprintf("CLID = %08X\n", clid);

	uint32_t louu_value = (clid >> 27) & 0x07; /* Level of Unification Uniprocessor for the cache hierarchy */
	int32_t ix;
	/* 下位キャッシュ層から順番にinvalidate */
	for (ix=6; 0 <= ix; ix--) {
		if ( (0x07 << (ix*3)) & clid ) {
			/* マスタCPU または 単体CPU内キャッシュのとき */
			if ( (cpuid == MASTER_CPU_ID) || ((ix+1) <= louu_value) ) {
				CSSELR_EL1_set(ix<<1);
				uint32_t ccsid = CCSIDR_EL1_get();
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
							DC_ISW_set(val);
						}
					}
				}
			}
		}
	}

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

arch_register_normal_memory(void)
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
	bool ret = false;
	uint32_t cpuid = CPUID_get();
	if ( (_irq_level[cpuid] == 0) && (_dispatch_disable[cpuid] == 0) ) {
		ret = true;
	}
	return ret;
}

void _dispatch(void)
{
	_switch_to(&cpu_struct[CPUID_get()]);
}

void arch_init_task_depend(void)
{
	board_init_task_depend();
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

void c_exc_handler(void)
{
	tprintf("exception\n");
	for (;;);
}

/*
**  0: X19-X29/X29
** 12:ELR/SPSR
** 14:X0-X18/X30 33
*/
void general_exception_handler(uint64_t* ptr)
{
	tprintf("general exception:CPU=%d:EL=%08X\n", CPUID_get(), CurrentEL_get());
	tprintf(" X0:%08X%08X  X1:%08X%08X  X2:%08X%08X\n", hword(ptr[14]),lword(ptr[14]), hword(ptr[15]),lword(ptr[15]), hword(ptr[16]),lword(ptr[16]));
	tprintf(" X3:%08X%08X  X4:%08X%08X  X5:%08X%08X\n", hword(ptr[17]),lword(ptr[17]), hword(ptr[18]),lword(ptr[18]), hword(ptr[19]),lword(ptr[19]));
	tprintf(" X6:%08X%08X  X7:%08X%08X  X8:%08X%08X\n", hword(ptr[20]),lword(ptr[20]), hword(ptr[21]),lword(ptr[21]), hword(ptr[22]),lword(ptr[22]));
	tprintf(" X9:%08X%08X X10:%08X%08X X11:%08X%08X\n", hword(ptr[23]),lword(ptr[23]), hword(ptr[24]),lword(ptr[24]), hword(ptr[25]),lword(ptr[25]));
	tprintf("X12:%08X%08X X13:%08X%08X X14:%08X%08X\n", hword(ptr[26]),lword(ptr[26]), hword(ptr[27]),lword(ptr[27]), hword(ptr[28]),lword(ptr[28]));
	tprintf("X15:%08X%08X X16:%08X%08X X17:%08X%08X\n", hword(ptr[29]),lword(ptr[29]), hword(ptr[30]),lword(ptr[30]), hword(ptr[31]),lword(ptr[31]));
	tprintf("X18:%08X%08X X19:%08X%08X X20:%08X%08X\n", hword(ptr[32]),lword(ptr[32]), hword(ptr[0]),lword(ptr[0]), hword(ptr[1]),lword(ptr[1]));
	tprintf("X21:%08X%08X X22:%08X%08X X23:%08X%08X\n", hword(ptr[2]),lword(ptr[2]), hword(ptr[3]),lword(ptr[3]), hword(ptr[4]),lword(ptr[4]));
	tprintf("X24:%08X%08X X25:%08X%08X X26:%08X%08X\n", hword(ptr[5]),lword(ptr[5]), hword(ptr[6]),lword(ptr[6]), hword(ptr[7]),lword(ptr[7]));
	tprintf("X27:%08X%08X X28:%08X%08X X29:%08X%08X\n", hword(ptr[8]),lword(ptr[8]), hword(ptr[9]),lword(ptr[9]), hword(ptr[10]),lword(ptr[10]));
	tprintf("X30:%08X%08X\n", hword(ptr[33]),lword(ptr[33]));
	tprintf("----------------------------------------\n");
	uint64_t esr = ESR_EL1_get();
	uint64_t far = FAR_EL1_get();
	tprintf("ELR:%08X%08X SPSR:%08X  ESR:%08X FAR:%08X%08X\n", hword(ptr[12]),lword(ptr[12]), lword(ptr[13]), lword(esr), hword(far),lword(far));
	tprintf("SP:%08X%08X\n", hword(ptr),lword(ptr));

	tprintf("\n\n");
	for (int ix=0; ix < CPU_NUM; ix++ ) {
		tprintf("[%d]:C:%08X N:%08X\n", ix, cpu_struct[ix].ctask, cpu_struct[ix].ntask);
	}
	tprintf("\n\n");
	for (;;);
}

test_print(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e)
{
	tprintf("%08X\n", a);
	tprintf("%08X\n", b);
	tprintf("%08X\n", c);
	tprintf("%08X\n", d);
	tprintf("%08X\n", e);
	for (;;);
}
