/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * my_board.h
 *
 *  Created on: 2015/12/13
 *      Author: biwa
 */

#ifndef MY_BOARD_H_
#define MY_BOARD_H_

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#define	START_MEM_ADDR		(0x80000000u)
#define	END_MEM_ADDR		(0xEFFFFFFFu)
#define	FRAME_BUFFER_BASE	(0xF0000000u)
#define	FRAME_BUFFER_SIZE	(0x10000000u)
#define	SDMMC_MAP_BASE		(0x0880000000uLL)
#define	SDMMC_MAP_SIZE		(0x180000000uLL)

/* IO register */
#define	SYSTEM_REG_BASE		(0x1C010000u)
#define	SYSTEM_REG_SIZE		(0x10000LL)
#define	UART_REG_BASE		(0x1C090000u)
#define	UART_REG_SIZE		(0x10000LL)
#define	CLCD_REG_BASE		(0x1C1F0000u)
#define	CLCD_REG_SIZE		(0x10000LL)

/* GICv3 */
#if defined(USE_GICV3)
#define	GIC_GICD_BASE		(0x002F000000)
#define	GIC_GICR_BASE		(0x002F100000)
#define	GIC_GICC_BASE		(0x002C002000)
#else
/* GICv2 */
#define	GIC_GICD_BASE		(0x002C001000)
#define	GIC_GICC_BASE		(0x002C002000)
#endif

#define	IRQ_NUM				(256)

#define	CACHE_LINE_SIZE		(64)

#define	CPU_PER_CLUSTER		(4)				/* 1クラスタあたり4コア */

#define	GENERIC_TIMER_INTR	(30)			/* genericTimerの割り込み番号 */


#define	CPU_CLOCK			(1000000000)	/* CPUクロック(未使用) */
#define	CNTFRQ_VALUE		(100000000)		/* GenericTimerのクロック */
#define	TIME_UNIT_VALUE		(1)				/* 1TIME-UNITのクロック数 */


#define	SYS_MALLOC_REGION_LOW		0
#define	SYS_MALLOC_REGION_HIGH		1
#define	SYS_MALLOC_REGION_DEFAULT	SYS_MALLOC_REGION_LOW


/*** Tick管理パラメータ ***/
typedef	uint64_t			TimeSpec;		/* OS内部での時間単位 */
typedef	uint32_t			TimeOut;		/* APIで指定するタイムアウト単位 */

/* TIME_UNIT値に追従すること */
#define	SEC(n)				((TimeOut)(n) * (CNTFRQ_VALUE/TIME_UNIT_VALUE))
#define	MSEC(n)				((TimeOut)(n) * ((CNTFRQ_VALUE/1000)/TIME_UNIT_VALUE))
#define	USEC(n)				((TimeOut)(n) * ((CNTFRQ_VALUE/1000000)/TIME_UNIT_VALUE))
#define	NSEC(n)				((TimeOut)1)

#ifdef __cplusplus
}
#endif

#endif /* MY_BOARD_H_ */
