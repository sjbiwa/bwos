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

/* GIC distributer register */
#define	GICD_CTLR				(GIC_GICD_BASE+0x000)		/* Distributor Control Register */
#define	GICD_TYPER				(GIC_GICD_BASE+0x004)		/* Interrupt Controller Type Register */
#define	GICD_IIDR				(GIC_GICD_BASE+0x008)		/* Distributor Implementer Identification Register */
#define	GICD_IGROUPR			(GIC_GICD_BASE+0x080)		/* Interrupt Group Registers */
#define	GICD_ISENABLER			(GIC_GICD_BASE+0x100)		/* Interrupt Set-Enable Registers */
#define	GICD_ICENABLER			(GIC_GICD_BASE+0x180)		/* Interrupt Clear-Enable Registers */
#define	GICD_ISPENDR			(GIC_GICD_BASE+0x200)		/* Interrupt Set-Pending Registers */
#define	GICD_ICPENDR			(GIC_GICD_BASE+0x280)		/* Interrupt Clear-Pending Registers */
#define	GICD_ISACTIVER			(GIC_GICD_BASE+0x300)		/* GICv2 Interrupt Set-Active Registers */
#define	GICD_ICACTIVER			(GIC_GICD_BASE+0x380)		/* Interrupt Clear-Active Registers */
#define	GICD_IPRIORITYR			(GIC_GICD_BASE+0x400)		/* Interrupt Priority Registers */
#define	GICD_ITARGETSR			(GIC_GICD_BASE+0x800)		/* Interrupt Processor Targets Registers */
#define	GICD_ICFGR				(GIC_GICD_BASE+0xC00)		/* Interrupt Configuration Registers */
#define	GICD_NSACR				(GIC_GICD_BASE+0xE00)		/* Non-secure Access Control Registers, optional */
#define	GICD_SGIR				(GIC_GICD_BASE+0xF00)		/* Software Generated Interrupt Register */
#define	GICD_CPENDSGIR			(GIC_GICD_BASE+0xF10)		/* SGI Clear-Pending Registers */
#define	GICD_SPENDSGIR			(GIC_GICD_BASE+0xF20)		/* SGI Set-Pending Registers */

/* GIC CPU interface register */
#define	GICC_CTLR				(GIC_GICC_BASE+0x0000)		/* CPU Interface Control Register */
#define	GICC_PMR				(GIC_GICC_BASE+0x0004)		/* Interrupt Priority Mask Register */
#define	GICC_BPR				(GIC_GICC_BASE+0x0008)		/* Binary Point Register */
#define	GICC_IAR				(GIC_GICC_BASE+0x000C)		/* Interrupt Acknowledge Register */
#define	GICC_EOIR				(GIC_GICC_BASE+0x0010)		/* End of Interrupt Register */
#define	GICC_RPR				(GIC_GICC_BASE+0x0014)		/* Running Priority Register */
#define	GICC_HPPIR				(GIC_GICC_BASE+0x0018)		/* Highest Priority Pending Interrupt Register */
#define	GICC_ABPR				(GIC_GICC_BASE+0x001C)		/* Aliased Binary Point Register */
#define	GICC_AIAR				(GIC_GICC_BASE+0x0020)		/* Aliased Interrupt Acknowledge Register */
#define	GICC_AEOIR				(GIC_GICC_BASE+0x0024)		/* Aliased End of Interrupt Register */
#define	GICC_AHPPIR				(GIC_GICC_BASE+0x0028)		/* Aliased Highest Priority Pending Interrupt Register */
#define	GICC_APR				(GIC_GICC_BASE+0x00D0)		/* Active Priorities Registers */
#define	GICC_NSAPR				(GIC_GICC_BASE+0x00E0)		/* Non-secure Active Priorities Registers */
#define	GICC_DIR				(GIC_GICC_BASE+0x1000)		/* Deactivate Interrupt Register */

#ifdef __cplusplus
}
#endif

#endif /* _GICV2REG_H_ */
