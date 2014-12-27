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

void arch_task_create(TaskStruct* task)
{
	extern void	_entry_stub(void);
	uint32_t*		ptr;

	/* setup stack pointer */
	if ( task->init_sp == 0 ) {
		/* stackをヒープから確保 */
		task->init_sp = __sys_malloc_align(task->stack_size, 8);
	}
	ptr = (uint32_t*)((uint32_t)(task->init_sp) + task->stack_size - TASK_FRAME_SIZE);
	task->save_sp = (void*)ptr;

	/* FPU(VFP)退避用領域の確保 (arch_tlsが確保されているタスク=VFP使用タスクとする) */
	if ( task->task_attr & TASK_FPU ) {
		task->arch_tls = __sys_malloc_align(8*32+1, 8); /* D0-D31, FPSCR */
	}

	/* setup task-context */
	ptr[TASK_FRAME_STUB] = (void*)_entry_stub;
	ptr[TASK_FRAME_PC] = (uint32_t)task->start_entry;
	ptr[TASK_FRAME_PSR] = (cpsr_get() | FLAG_T ) & ~FLAG_I;
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

void arch_system_postinit(void)
{
	mmgr_init();
	uint32_t size = (uint8_t*)(END_MEM_ADDR+1) - (uint8_t*)(heap_start_addr);
	size &= ~0x0000000fu;
	tprintf("mblock addr=%08X size=%08X\n", heap_start_addr, size);
	sys_malloc_add_block(heap_start_addr, size);
	tprintf("mblock ok\n");
}
