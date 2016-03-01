/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * gicv2reg.h
 *
 *  Created on: 2015/01/15
 *      Author: biwa
 */

#ifndef _GICV2REG_H_
#define _GICV2REG_H_

#include "my_board.h"

#ifdef __cplusplus
extern "C" {
#endif

#define	MPCORE_SCU_BASE			(MPCORE_BASE)
#define	MPCORE_GICD_BASE		(MPCORE_BASE+0x1000)
#define	MPCORE_GICC_BASE		(MPCORE_BASE+0x2000)

/* GIC distributer register */
#define	GICD_CTLR				(MPCORE_GICD_BASE+0x000)		/* Distributor Control Register */
#define	GICD_TYPER				(MPCORE_GICD_BASE+0x004)		/* Interrupt Controller Type Register */
#define	GICD_IIDR				(MPCORE_GICD_BASE+0x008)		/* Distributor Implementer Identification Register */
#define	GICD_IGROUPR			(MPCORE_GICD_BASE+0x080)		/* Interrupt Group Registers */
#define	GICD_ISENABLER			(MPCORE_GICD_BASE+0x100)		/* Interrupt Set-Enable Registers */
#define	GICD_ICENABLER			(MPCORE_GICD_BASE+0x180)		/* Interrupt Clear-Enable Registers */
#define	GICD_ISPENDR			(MPCORE_GICD_BASE+0x200)		/* Interrupt Set-Pending Registers */
#define	GICD_ICPENDR			(MPCORE_GICD_BASE+0x280)		/* Interrupt Clear-Pending Registers */
#define	GICD_ISACTIVER			(MPCORE_GICD_BASE+0x300)		/* GICv2 Interrupt Set-Active Registers */
#define	GICD_ICACTIVER			(MPCORE_GICD_BASE+0x380)		/* Interrupt Clear-Active Registers */
#define	GICD_IPRIORITYR			(MPCORE_GICD_BASE+0x400)		/* Interrupt Priority Registers */
#define	GICD_ITARGETSR			(MPCORE_GICD_BASE+0x800)		/* Interrupt Processor Targets Registers */
#define	GICD_ICFGR				(MPCORE_GICD_BASE+0xC00)		/* Interrupt Configuration Registers */
#define	GICD_NSACR				(MPCORE_GICD_BASE+0xE00)		/* Non-secure Access Control Registers, optional */
#define	GICD_SGIR				(MPCORE_GICD_BASE+0xF00)		/* Software Generated Interrupt Register */
#define	GICD_CPENDSGIR			(MPCORE_GICD_BASE+0xF10)		/* SGI Clear-Pending Registers */
#define	GICD_SPENDSGIR			(MPCORE_GICD_BASE+0xF20)		/* SGI Set-Pending Registers */

/* GIC CPU interface register */
#define	GICC_CTLR				(MPCORE_GICC_BASE+0x0000)		/* CPU Interface Control Register */
#define	GICC_PMR				(MPCORE_GICC_BASE+0x0004)		/* Interrupt Priority Mask Register */
#define	GICC_BPR				(MPCORE_GICC_BASE+0x0008)		/* Binary Point Register */
#define	GICC_IAR				(MPCORE_GICC_BASE+0x000C)		/* Interrupt Acknowledge Register */
#define	GICC_EOIR				(MPCORE_GICC_BASE+0x0010)		/* End of Interrupt Register */
#define	GICC_RPR				(MPCORE_GICC_BASE+0x0014)		/* Running Priority Register */
#define	GICC_HPPIR				(MPCORE_GICC_BASE+0x0018)		/* Highest Priority Pending Interrupt Register */
#define	GICC_ABPR				(MPCORE_GICC_BASE+0x001C)		/* Aliased Binary Point Register */
#define	GICC_AIAR				(MPCORE_GICC_BASE+0x0020)		/* Aliased Interrupt Acknowledge Register */
#define	GICC_AEOIR				(MPCORE_GICC_BASE+0x0024)		/* Aliased End of Interrupt Register */
#define	GICC_AHPPIR				(MPCORE_GICC_BASE+0x0028)		/* Aliased Highest Priority Pending Interrupt Register */
#define	GICC_APR				(MPCORE_GICC_BASE+0x00D0)		/* Active Priorities Registers */
#define	GICC_NSAPR				(MPCORE_GICC_BASE+0x00E0)		/* Non-secure Active Priorities Registers */
#define	GICC_DIR				(MPCORE_GICC_BASE+0x1000)		/* Deactivate Interrupt Register */

#ifdef __cplusplus
}
#endif

#endif /* _GICV2REG_H_ */
