/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * archv8_common.h
 *
 *  Created on: 2015/12/13
 *      Author: biwa
 */

#ifndef ARCHV8_COMMON_H_
#define ARCHV8_COMMON_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "my_board.h"
#include "armv8.h"
#include "armv8reg.h"

#define	irq_enable()		DAIFClr_seti(0x2)
#define	irq_disable()		DAIFSet_seti(0x2)

#define	irq_save()			({ uint32_t _reg; _reg = DAIF_get(); DAIFSet_seti(0x4); _reg;})
#define	irq_restore(reg)	do { if ( !((reg) & FLAG_I) ) DAIFClr_seti(0x4); } while(0)

#define	STACK_ALIGN			(16)
#define	NORMAL_ALIGN		(16)
#define	TASK_SVC_STACK_SIZE					(2048)
#define	INITIAL_TASK_USR_STACK_SIZE			(1024)

typedef	uint64_t			MemSize_t;	/* メモリサイズを表す型 */
typedef	uint64_t			PtrInt_t;	/* ポインタ型を整数型に変換するときの型 */

extern bool arch_can_dispatch(void);
extern TimeSpec get_tick_count(void);
extern void update_first_timeout(TimeSpec tmout);

#endif /* ARCHV8_COMMON_H_ */
