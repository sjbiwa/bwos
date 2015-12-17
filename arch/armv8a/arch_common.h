/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * arch_common.h
 *
 *  Created on: 2015/12/13
 *      Author: biwa
 */

#ifndef ARCH_COMMON_H_
#define ARCH_COMMON_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "my_board.h"
#include "arm.h"
#include "armv8reg.h"

#define	irq_enable()		DAIFClr_seti(0x4)
#define	irq_disable()		DAIFSet_seti(0x4)

#define	irq_save()			({ uint32_t _reg; _reg = DAIF_get(); DAIFSet_seti(0x4); _reg;})
#define	irq_restore(reg)	do { if ( !((reg) & FLAG_I) ) DAIFClr_seti(0x4); } while(0)

#define	STACK_ALIGN			(8)
#define	NORMAL_ALIGN		(8)

typedef	uint64_t			MemSize_t;	/* メモリサイズを表す型 */
typedef	uint64_t			PtrInt_t;	/* ポインタ型を整数型に変換するときの型 */

extern bool arch_can_dispatch(void);
extern TimeSpec get_tick_count(void);
extern void update_first_timeout(TimeSpec tmout);

#endif /* ARCH_COMMON_H_ */