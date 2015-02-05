/*
 * arch.c
 *
 *  Created on: 2014/12/07
 *      Author: biwa
 */

#include "common.h"
#include "api.h"
#include "task.h"
#include "arm.h"
#include "cp15reg.h"
#include "my_board.h"

extern void	_entry_stub(void);
extern char __heap_start;
void* heap_start_addr = &__heap_start;

/* 最初に1になるビットを左側から探す */
static inline int32_t bit_srch_l(uint32_t val)
{
	int32_t ret = 31;
	while ( (0 <= ret) && ((val & (0x01u<<ret)) == 0) ) {
		ret--;
	}
	return ret;
}

static inline void arch_init_task(TaskStruct* task)
{
	uint32_t*		ptr;
	uint32_t*		usr_stack;
	/* SVC stack */
	ptr = (uint32_t*)((PtrInt_t)(task->init_sp) + task->stack_size - TASK_FRAME_SIZE);
	ptr = PRE_ALIGN_BY(ptr, STACK_ALIGN);
	task->save_sp = ptr;
	/* USR/SYS stack */
	usr_stack = (uint32_t*)((PtrInt_t)(task->usr_init_sp) + task->usr_stack_size);
	usr_stack = PRE_ALIGN_BY(usr_stack, STACK_ALIGN);

	/* setup task-context */
	ptr[TASK_FRAME_STUB] = (PtrInt_t)_entry_stub;
	ptr[TASK_FRAME_PC] = (PtrInt_t)task->entry;
	ptr[TASK_FRAME_PSR] = FLAG_T | ((task->task_attr&TASK_SYS)?MODE_SYS:MODE_USR);
	ptr[TASK_FRAME_FPEXC] = 0x00000000;
	ptr[TASK_FRAME_SPSR] = 0xA5A5A5A5;
	ptr[TASK_FRAME_SP_USR] = (PtrInt_t)usr_stack;
	ptr[TASK_FRAME_LR_USR] = 0xF0F0F0F0;
}

void arch_init_task_create(TaskStruct* task)
{
	arch_init_task(task);
}

void arch_task_create(TaskStruct* task)
{
	arch_init_task(task);

	/* FPU(VFP)退避用領域の確保 */
	if ( task->task_attr & TASK_FPU ) {
		uint32_t*		ptr = task->save_sp;
		task->arch_tls = __sys_malloc_align(8*32+1, STACK_ALIGN); /* D0-D31, FPSCR */
		ptr[TASK_FRAME_FPEXC] = 0x40000000;
	}
}

void arch_system_preinit(void)
{
	/* PROC_ID / ASID を初期化 */
	CONTEXTIDR_set(0);

	/* データキャッシュinvalidate */
	uint32_t clid = CLIDR_get();
	int32_t ix;
	/* 下位キャッシュ層から順番にinvalidate */
	for (ix=6; 0 <= ix; ix--) {
		if ( (0x07 << (ix*3)) & clid ) {
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

	/* coprocessor CP10/CP11 有効化 (VFP full access) */
	uint32_t cpval = CPACR_get();
	cpval &= 0x0fffffff;
	cpval |= ((0x3<<(10*2))|(0x3<<(11*2)));
	CPACR_set(cpval);

	/* テスト用にVFPレジスタ初期化 */
	 FPEXC_set(0x40000000);
	 FPSCR_set(0x00000000);
}

void arch_malloc_init(void)
{
	mmgr_init();
	uint32_t size = (uint8_t*)(END_MEM_ADDR+1) - (uint8_t*)(heap_start_addr);
	size &= ~0x0000000fu;
	tprintf("mblock addr=%08X size=%08X\n", heap_start_addr, size);
	sys_malloc_add_block(heap_start_addr, size);
	tprintf("mblock ok\n");
}

void arch_system_postinit(void)
{
}

bool arch_can_dispatch(void)
{
extern	uint32_t _irq_level; /* 多重割り込みレベル */
	bool ret = false;
	if ( _irq_level == 0 ) {
		ret = true;
	}
	return ret;
}

