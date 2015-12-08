/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

#ifndef _GICV3REG_H_
#define	_GICV3REG_H_


#include "target_conf.h" /* GIC base register define */

#define	GIC_VLPI_BASE			(GIC_GICR_BASE+0x10000u)
#define	GIC_SGI_BASE			(GIC_GICR_BASE+0x10000u)

#define	GICD_CTLR				(GIC_GICD_BASE+0x0000u)					/* RW See the register description Distributor Control Register */
#define	GICD_TYPER				(GIC_GICD_BASE+0x0004u)					/* RO IMPLEMENTATION DEFINED Interrupt Controller Type Register */
#define	GICD_IIDR				(GIC_GICD_BASE+0x0008u)					/* RO IMPLEMENTATION DEFINED Distributor Implementer Identification Register */
#define	GICD_STATUSR			(GIC_GICD_BASE+0x0010u)					/* RW 0x000 00000 Error Reporting Status Register, optional */
#define	GICD_SETSPI_NSR			(GIC_GICD_BASE+0x0040u)					/* WO - Set SPI Register */
#define	GICD_CLRSPI_NSR			(GIC_GICD_BASE+0x0048u)					/* WO - Clear SPI Register */
#define	GICD_SETSPI_SR			(GIC_GICD_BASE+0x0050u)					/* WO - Set SPI, Secure Register */
#define	GICD_CLRSPI_SR			(GIC_GICD_BASE+0x0058u)					/* WO - Clear SPI, Secure Register */
#define	GICD_IGROUPR	 		(GIC_GICD_BASE+0x0080u)					/* RW IMPLEMENTATION DEFINED Interrupt Group Registers */
#define	GICD_ISENABLER	 		(GIC_GICD_BASE+0x0100u)					/* RW IMPLEMENTATION DEFINED Interrupt Set-Enable Registers */
#define	GICD_ICENABLER	 		(GIC_GICD_BASE+0x0180u)					/* RW IMPLEMENTATION DEFINED Interrupt Clear-Enable Registers */
#define	GICD_ISPENDR	 		(GIC_GICD_BASE+0x0200u)					/* RW 0x0000 0000 Interrupt Set-Pending Registers */
#define	GICD_ICPENDR	 		(GIC_GICD_BASE+0x0280u)					/* RW 0x0000 0000 Interrupt Clear-Pending Registers */
#define	GICD_ISACTIVER	 		(GIC_GICD_BASE+0x0300u)					/* RW 0x0000 0000 Interrupt Set-Active Registers */
#define	GICD_ICACTIVER	 		(GIC_GICD_BASE+0x0380u)					/* RW 0x0000 0000 Interrupt Clear-Active Registers */
#define	GICD_IPRIORITYR	 		(GIC_GICD_BASE+0x0400u)					/* RW 0x0000 0000 Interrupt Priority Registers */
#define	GICD_ITARGETSR		 	(GIC_GICD_BASE+0x0800u)					/* RO IMPLEMENTATION DEFINED Interrupt Processor Targets Registers */
#define	GICD_ICFGR				(GIC_GICD_BASE+0x0C00u)					/* RW IMPLEMENTATION DEFINED Interrupt Configuration Registers */
#define	GICD_IGRPMODR			(GIC_GICD_BASE+0x0D00u)					/* - 0x0000 0000 Interrupt Group Modifier Registers */
#define	GICD_NSACR				(GIC_GICD_BASE+0x0E00u)					/* RW 0x0000 0000 Non-secure Access Control Registers */
#define	GICD_SGIR				(GIC_GICD_BASE+0x0F00u)					/* WO - Software Generated Interrupt Register */
#define	GICD_CPENDSGIR			(GIC_GICD_BASE+0x0F10u)					/* RW 0x0000 0000 SGI Clear-Pending Registers */
#define	GICD_SPENDSGIR			(GIC_GICD_BASE+0x0F20u)					/* RW 0x0000 0000 SGI Set-Pending Registers */
#define	GICD_IROUTER			(GIC_GICD_BASE+0x6000u)					/* RW 0x0000 0000 Interrupt Routing Registers */

#define	GICR_CTLR				(GIC_GICR_BASE+0x0000u)					/* RW See the register description Redistributor Control Register */
#define	GICR_IIDR				(GIC_GICR_BASE+0x0004u)					/* RO IMPLEMENTATION DEFINED Implementer Identification Register */
#define	GICR_TYPER				(GIC_GICR_BASE+0x0008u)					/* RO IMPLEMENTATION DEFINED Redistributor Type Register */
#define	GICR_STATUSR			(GIC_GICR_BASE+0x0010u)					/* RW 0x0000 0000 Error Reporting Status Register, optional */
#define	GICR_WAKER				(GIC_GICR_BASE+0x0014u)					/* RW See the register description Redistributor Wake Register */
#define	GICR_SETLPIR			(GIC_GICR_BASE+0x0040u)					/* WO - Set LPI Pending Register */
#define	GICR_CLRLPIR			(GIC_GICR_BASE+0x0048u)					/* WO - Clear LPI Pending Register */
#define	GICR_PROPBASER			(GIC_GICR_BASE+0x0070u)					/* RW - Redistributor Properties Base Address Register */
#define	GICR_PENDBASER			(GIC_GICR_BASE+0x0078u)					/* RW - Redistributor LPI Pending Table Base Address Register */
#define	GICR_INVLPIR			(GIC_GICR_BASE+0x00A0u)					/* WO - Redistributor Invalidate LPI Register */
#define	GICR_INVALLR			(GIC_GICR_BASE+0x00B0u)					/* WO - Redistributor Invalidate All Register */
#define	GICR_SYNCR				(GIC_GICR_BASE+0x00C0u)					/* RO - Redistributor Synchronize Register */

#define	GICR_VPROPBASER			(GIC_VLPI_BASE+0x0070u)					/* RW - Virtual Redistributor Properties Base Address Register */
#define	GICR_VPENDBASER			(GIC_VLPI_BASE+0x0078u)					/* RW - Virtual Pending Table Base Address Register */

#define	GICR_IGROUPR0			(GIC_SGI_BASE+0x0080u)					/* RW - Interrupt Group Register 0 */
#define	GICR_ISENABLER0			(GIC_SGI_BASE+0x0100u)					/* RW IMPLEMENTATION DEFINED Interrupt Set-Enable Register 0 */
#define	GICR_ICENABLER0			(GIC_SGI_BASE+0x0180u)					/* RW IMPLEMENTATION DEFINED Interrupt Clear-Enable Register 0 */
#define	GICR_ISPENDR0			(GIC_SGI_BASE+0x0200u)					/* RW 0x0000 0000 Interrupt Set-Pend Register 0 */
#define	GICR_ICPENDR0			(GIC_SGI_BASE+0x0280u)					/* RW 0x0000 0000 Interrupt Clear-Pend Register 0 */
#define	GICR_ISACTIVER0			(GIC_SGI_BASE+0x0300u)					/* RW 0x0000 0000 Interrupt Set-Active Register 0 */
#define	GICR_ICACTIVER0			(GIC_SGI_BASE+0x0380u)					/* RW 0x0000 0000 Interrupt Clear-Active Register 0 */
#define	GICR_IPRIORITYR			(GIC_SGI_BASE+0x0400u)					/* RW 0x0000 0000 Interrupt Priority Registers */
#define	GICR_ICFGR0				(GIC_SGI_BASE+0x0C00u)					/* RW IMPLEMENTATION DEFINED SGI Configuration Register */
#define	GICR_ICFGR1				(GIC_SGI_BASE+0x0C04u)					/* RW IMPLEMENTATION DEFINED PPI Configuration Register */
#define	GICR_IGRPMODR0			(GIC_SGI_BASE+0x0D00u)					/* RW - Interrupt Group Modifier Register 0 */
#define	GICR_NSACR				(GIC_SGI_BASE+0x0E00u)					/* RW 0x0000 0000 Non-Secure Access Control Register */



#define	GICC_CTLR				(GIC_GICC_BASE+0x0000u)					/* RW See the register description CPU Interface Control Register */
#define	GICC_PMR				(GIC_GICC_BASE+0x0004u)					/* RW 0x0000 0000 Interrupt Priority Mask Register */
#define	GICC_BPR				(GIC_GICC_BASE+0x0008u)					/* RW 0x0000 000xa Binary Point Register */
#define	GICC_IAR				(GIC_GICC_BASE+0x000Cu)					/* RO - Interrupt Acknowledge Register */
#define	GICC_EOIR				(GIC_GICC_BASE+0x0010u)					/* WO - End of Interrupt Register */
#define	GICC_RPR				(GIC_GICC_BASE+0x0014u)					/* RO - Running Priority Register */
#define	GICC_HPPIR				(GIC_GICC_BASE+0x0018u)					/* RO - Highest Priority Pending Interrupt Register */
#define	GICC_ABPR				(GIC_GICC_BASE+0x001Cu)					/* RW 0x0000 000xa Aliased Binary Point Register */
#define	GICC_AIAR				(GIC_GICC_BASE+0x0020u)					/* RO - Aliased Interrupt Acknowledge Register */
#define	GICC_AEOIR				(GIC_GICC_BASE+0x0024u)					/* WO - Aliased End of Interrupt Register */
#define	GICC_AHPPIR				(GIC_GICC_BASE+0x0028u)					/* RO - Aliased Highest Priority Pending Interrupt Register */
#define	GICC_STATUSR			(GIC_GICC_BASE+0x002Cu)					/* RW 0x0000 0000 Error Reporting Status Register, optional */
#define	GICC_APR				(GIC_GICC_BASE+0x00D0u)					/* RW 0x0000 0000 Active Priorities Registers */
#define	GICC_NSAPR				(GIC_GICC_BASE+0x00E0u)					/* RW 0x0000 0000 Non-secure Active Priorities Registers */
#define	GICC_IIDR				(GIC_GICC_BASE+0x00FCu)					/* RO IMPLEMENTATION DEFINED CPU Interface Identification Register */
#define	GICC_DIR				(GIC_GICC_BASE+0x1000u)					/* WO - Deactivate Interrupt Register */

#ifdef __ASM__

#define ICC_AP0R0_EL1_get(v)					MRS v, S3_0_C12_C8_4		/* RW Interrupt Controller Active Priorities Group 0 Registers, n = 0 - 3 */
#define ICC_AP0R0_EL1_set(v)					MSR S3_0_C12_C8_4, v    	/* RW Interrupt Controller Active Priorities Group 0 Registers, n = 0 - 3 */
#define ICC_AP0R1_EL1_get(v)					MRS v, S3_0_C12_C8_5        /* RW Interrupt Controller Active Priorities Group 0 Registers, n = 0 - 3 */
#define ICC_AP0R1_EL1_set(v)					MSR S3_0_C12_C8_5, v        /* RW Interrupt Controller Active Priorities Group 0 Registers, n = 0 - 3 */
#define ICC_AP0R2_EL1_get(v)					MRS v, S3_0_C12_C8_6        /* RW Interrupt Controller Active Priorities Group 0 Registers, n = 0 - 3 */
#define ICC_AP0R2_EL1_set(v)					MSR S3_0_C12_C8_6, v        /* RW Interrupt Controller Active Priorities Group 0 Registers, n = 0 - 3 */
#define ICC_AP0R3_EL1_get(v)					MRS v, S3_0_C12_C8_7        /* RW Interrupt Controller Active Priorities Group 0 Registers, n = 0 - 3 */
#define ICC_AP0R3_EL1_set(v)					MSR S3_0_C12_C8_7, v        /* RW Interrupt Controller Active Priorities Group 0 Registers, n = 0 - 3 */
#define ICC_AP1R0_EL1_get(v)					MRS v, S3_0_C12_C9_0        /* RW Interrupt Controller Active Priorities Group 1 Registers, n = 0 - 3 */
#define ICC_AP1R0_EL1_set(v)					MSR S3_0_C12_C9_0, v        /* RW Interrupt Controller Active Priorities Group 1 Registers, n = 0 - 3 */
#define ICC_AP1R1_EL1_get(v)					MRS v, S3_0_C12_C9_1        /* RW Interrupt Controller Active Priorities Group 1 Registers, n = 0 - 3 */
#define ICC_AP1R1_EL1_set(v)					MSR S3_0_C12_C9_1, v        /* RW Interrupt Controller Active Priorities Group 1 Registers, n = 0 - 3 */
#define ICC_AP1R2_EL1_get(v)					MRS v, S3_0_C12_C9_2        /* RW Interrupt Controller Active Priorities Group 1 Registers, n = 0 - 3 */
#define ICC_AP1R2_EL1_set(v)					MSR S3_0_C12_C9_2, v        /* RW Interrupt Controller Active Priorities Group 1 Registers, n = 0 - 3 */
#define ICC_AP1R3_EL1_get(v)					MRS v, S3_0_C12_C9_3        /* RW Interrupt Controller Active Priorities Group 1 Registers, n = 0 - 3 */
#define ICC_AP1R3_EL1_set(v)					MSR S3_0_C12_C9_3, v        /* RW Interrupt Controller Active Priorities Group 1 Registers, n = 0 - 3 */
#define ICC_ASGI1R_EL1_set(v)					MSR S3_0_C12_C11_6, v       /* WO Interrupt Controller Alias Software Generated Interrupt Group 1 Register */
#define ICC_BPR0_EL1_get(v)						MRS v, S3_0_C12_C8_3        /* RW Interrupt Controller Binary Point Register 0 */
#define ICC_BPR0_EL1_set(v)						MSR S3_0_C12_C8_3, v        /* RW Interrupt Controller Binary Point Register 0 */
#define ICC_BPR1_EL1_get(v)						MRS v, S3_0_C12_C8_3        /* RW Interrupt Controller Binary Point Register 1 */
#define ICC_BPR1_EL1_set(v)						MSR S3_0_C12_C8_3, v        /* RW Interrupt Controller Binary Point Register 1 */
#define ICC_CTLR_EL1_get(v)						MRS v, S3_0_C12_C12_4       /* RW Interrupt Controller Control Register (EL1) */
#define ICC_CTLR_EL1_set(v)						MSR S3_0_C12_C12_4, v       /* RW Interrupt Controller Control Register (EL1) */
#define ICC_CTLR_EL3_get(v)						MRS v, S3_6_C12_C12_4       /* RW Interrupt Controller Control Register (EL3) */
#define ICC_CTLR_EL3_set(v)						MSR S3_6_C12_C12_4, v       /* RW Interrupt Controller Control Register (EL3) */
#define ICC_DIR_EL1_set(v)						MSR S3_0_C12_C11_1, v       /* WO Interrupt Controller Deactivate Interrupt Register */
#define ICC_EOIR0_EL1_set(v)					MSR S3_0_C12_C8_1, v        /* WO Interrupt Controller End Of Interrupt Register 0 */
#define ICC_EOIR1_EL1_set(v)					MSR S3_0_C12_C12_1, v       /* WO Interrupt Controller End Of Interrupt Register 1 */
#define ICC_HPPIR0_EL1_get(v)					MRS v, S3_0_C12_C8_2        /* RO Interrupt Controller Highest Priority Pending Interrupt Register 0 */
#define ICC_HPPIR1_EL1_get(v)					MRS v, S3_0_C12_C12_2       /* RO Interrupt Controller Highest Priority Pending Interrupt Register 1 */
#define ICC_IAR0_EL1_get(v)						MRS v, S3_0_C12_C8_0        /* RO Interrupt Controller Interrupt Acknowledge Register 0 */
#define ICC_IAR1_EL1_get(v)						MRS v, S3_0_C12_C12_0       /* RO Interrupt Controller Interrupt Acknowledge Register 1 */
#define ICC_IGRPEN0_EL1_get(v)					MRS v, S3_0_C12_C12_6       /* RW Interrupt Controller Interrupt Group 0 Enable register */
#define ICC_IGRPEN0_EL1_set(v)					MSR S3_0_C12_C12_6, v       /* RW Interrupt Controller Interrupt Group 0 Enable register */
#define ICC_IGRPEN1_EL1_get(v)					MRS v, S3_0_C12_C12_7       /* RW Interrupt Controller Interrupt Group 1 Enable register */
#define ICC_IGRPEN1_EL1_set(v)					MSR S3_0_C12_C12_7, v       /* RW Interrupt Controller Interrupt Group 1 Enable register */
#define ICC_IGRPEN1_EL3_get(v)					MRS v, S3_6_C12_C12_7       /* RW Interrupt Controller Interrupt Group 1 Enable register (EL3) */
#define ICC_IGRPEN1_EL3_set(v)					MSR S3_6_C12_C12_7, v       /* RW Interrupt Controller Interrupt Group 1 Enable register (EL3) */
#define ICC_PMR_EL1_get(v)						MRS v, S3_0_C4_C6_0         /* RW Interrupt Controller Interrupt Priority Mask Register */
#define ICC_PMR_EL1_set(v)						MSR S3_0_C4_C6_0, v         /* RW Interrupt Controller Interrupt Priority Mask Register */
#define ICC_RPR_EL1_get(v)						MRS v, S3_0_C12_C11_3       /* RO Interrupt Controller Running Priority Register */
#define ICC_SGI0R_EL1_set(v)					MSR S3_0_C12_C11_7, v       /* WO Interrupt Controller Software Generated Interrupt Group 0 Register */
#define ICC_SGI1R_EL1_set(v)					MSR S3_0_C12_C11_5, v       /* WO Interrupt Controller Software Generated Interrupt Group 1 Register */
#define ICC_SRE_EL1_get(v)						MRS v, S3_0_C12_C12_5       /* RW Interrupt Controller System Register Enable register (EL1) */
#define ICC_SRE_EL1_set(v)						MSR S3_0_C12_C12_5, v       /* RW Interrupt Controller System Register Enable register (EL1) */
#define ICC_SRE_EL2_get(v)						MRS v, S3_4_C12_C9_5        /* RW Interrupt Controller System Register Enable register (EL2) */
#define ICC_SRE_EL2_set(v)						MSR S3_4_C12_C9_5, v        /* RW Interrupt Controller System Register Enable register (EL2) */
#define ICC_SRE_EL3_get(v)						MRS v, S3_6_C12_C12_5       /* RW Interrupt Controller System Register Enable register (EL3) */
#define ICC_SRE_EL3_set(v)						MSR S3_6_C12_C12_5, v       /* RW Interrupt Controller System Register Enable register (EL3) */

#else

#define ICC_AP0R0_EL1_get()			({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_0_C12_C8_4":"=r"(_reg_)::"memory");_reg_;})		/* RW Interrupt Controller Active Priorities Group 0 Registers, n = 0 - 3 */
#define ICC_AP0R0_EL1_set(v)		__asm__ volatile ("MSR S3_0_C12_C8_4, %0"::"r"(v):"memory")										/* RW Interrupt Controller Active Priorities Group 0 Registers, n = 0 - 3 */
#define ICC_AP0R1_EL1_get()			({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_0_C12_C8_5":"=r"(_reg_)::"memory");_reg_;})		/* RW Interrupt Controller Active Priorities Group 0 Registers, n = 0 - 3 */
#define ICC_AP0R1_EL1_set(v)		__asm__ volatile ("MSR S3_0_C12_C8_5, %0"::"r"(v):"memory")										/* RW Interrupt Controller Active Priorities Group 0 Registers, n = 0 - 3 */
#define ICC_AP0R2_EL1_get()			({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_0_C12_C8_6":"=r"(_reg_)::"memory");_reg_;})		/* RW Interrupt Controller Active Priorities Group 0 Registers, n = 0 - 3 */
#define ICC_AP0R2_EL1_set(v)		__asm__ volatile ("MSR S3_0_C12_C8_6, %0"::"r"(v):"memory")										/* RW Interrupt Controller Active Priorities Group 0 Registers, n = 0 - 3 */
#define ICC_AP0R3_EL1_get()			({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_0_C12_C8_7":"=r"(_reg_)::"memory");_reg_;})		/* RW Interrupt Controller Active Priorities Group 0 Registers, n = 0 - 3 */
#define ICC_AP0R3_EL1_set(v)		__asm__ volatile ("MSR S3_0_C12_C8_7, %0"::"r"(v):"memory")										/* RW Interrupt Controller Active Priorities Group 0 Registers, n = 0 - 3 */
#define ICC_AP1R0_EL1_get()			({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_0_C12_C9_0":"=r"(_reg_)::"memory");_reg_;})		/* RW Interrupt Controller Active Priorities Group 1 Registers, n = 0 - 3 */
#define ICC_AP1R0_EL1_set(v)		__asm__ volatile ("MSR S3_0_C12_C9_0, %0"::"r"(v):"memory")										/* RW Interrupt Controller Active Priorities Group 1 Registers, n = 0 - 3 */
#define ICC_AP1R1_EL1_get()			({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_0_C12_C9_1":"=r"(_reg_)::"memory");_reg_;})		/* RW Interrupt Controller Active Priorities Group 1 Registers, n = 0 - 3 */
#define ICC_AP1R1_EL1_set(v)		__asm__ volatile ("MSR S3_0_C12_C9_1, %0"::"r"(v):"memory")										/* RW Interrupt Controller Active Priorities Group 1 Registers, n = 0 - 3 */
#define ICC_AP1R2_EL1_get()			({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_0_C12_C9_2":"=r"(_reg_)::"memory");_reg_;})		/* RW Interrupt Controller Active Priorities Group 1 Registers, n = 0 - 3 */
#define ICC_AP1R2_EL1_set(v)		__asm__ volatile ("MSR S3_0_C12_C9_2, %0"::"r"(v):"memory")										/* RW Interrupt Controller Active Priorities Group 1 Registers, n = 0 - 3 */
#define ICC_AP1R3_EL1_get()			({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_0_C12_C9_3":"=r"(_reg_)::"memory");_reg_;})		/* RW Interrupt Controller Active Priorities Group 1 Registers, n = 0 - 3 */
#define ICC_AP1R3_EL1_set(v)		__asm__ volatile ("MSR S3_0_C12_C9_3, %0"::"r"(v):"memory")										/* RW Interrupt Controller Active Priorities Group 1 Registers, n = 0 - 3 */
#define ICC_ASGI1R_EL1_set(v)		__asm__ volatile ("MSR S3_0_C12_C11_6, %0"::"r"(v):"memory")									/* WO Interrupt Controller Alias Software Generated Interrupt Group 1 Register */
#define ICC_BPR0_EL1_get()			({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_0_C12_C8_3":"=r"(_reg_)::"memory");_reg_;})		/* RW Interrupt Controller Binary Point Register 0 */
#define ICC_BPR0_EL1_set(v)			__asm__ volatile ("MSR S3_0_C12_C8_3, %0"::"r"(v):"memory")										/* RW Interrupt Controller Binary Point Register 0 */
#define ICC_BPR1_EL1_get()			({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_0_C12_C8_3":"=r"(_reg_)::"memory");_reg_;})		/* RW Interrupt Controller Binary Point Register 1 */
#define ICC_BPR1_EL1_set(v)			__asm__ volatile ("MSR S3_0_C12_C8_3, %0"::"r"(v):"memory")										/* RW Interrupt Controller Binary Point Register 1 */
#define ICC_CTLR_EL1_get()			({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_0_C12_C12_4":"=r"(_reg_)::"memory");_reg_;})		/* RW Interrupt Controller Control Register (EL1) */
#define ICC_CTLR_EL1_set(v)			__asm__ volatile ("MSR S3_0_C12_C12_4, %0"::"r"(v):"memory")										/* RW Interrupt Controller Control Register (EL1) */
#define ICC_CTLR_EL3_get()			({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_6_C12_C12_4":"=r"(_reg_)::"memory");_reg_;})		/* RW Interrupt Controller Control Register (EL3) */
#define ICC_CTLR_EL3_set(v)			__asm__ volatile ("MSR S3_6_C12_C12_4, %0"::"r"(v):"memory")										/* RW Interrupt Controller Control Register (EL3) */
#define ICC_DIR_EL1_set(v)			__asm__ volatile ("MSR S3_0_C12_C11_1, %0"::"r"(v):"memory")										/* WO Interrupt Controller Deactivate Interrupt Register */
#define ICC_EOIR0_EL1_set(v)		__asm__ volatile ("MSR S3_0_C12_C8_1, %0"::"r"(v):"memory")										/* WO Interrupt Controller End Of Interrupt Register 0 */
#define ICC_EOIR1_EL1_set(v)		__asm__ volatile ("MSR S3_0_C12_C12_1, %0"::"r"(v):"memory")										/* WO Interrupt Controller End Of Interrupt Register 1 */
#define ICC_HPPIR0_EL1_get()		({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_0_C12_C8_2":"=r"(_reg_)::"memory");_reg_;})		/* RO Interrupt Controller Highest Priority Pending Interrupt Register 0 */
#define ICC_HPPIR1_EL1_get()		({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_0_C12_C12_2":"=r"(_reg_)::"memory");_reg_;})		/* RO Interrupt Controller Highest Priority Pending Interrupt Register 1 */
#define ICC_IAR0_EL1_get()			({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_0_C12_C8_0":"=r"(_reg_)::"memory");_reg_;})		/* RO Interrupt Controller Interrupt Acknowledge Register 0 */
#define ICC_IAR1_EL1_get()			({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_0_C12_C12_0":"=r"(_reg_)::"memory");_reg_;})		/* RO Interrupt Controller Interrupt Acknowledge Register 1 */
#define ICC_IGRPEN0_EL1_get()		({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_0_C12_C12_6":"=r"(_reg_)::"memory");_reg_;})	/* RW Interrupt Controller Interrupt Group 0 Enable register */
#define ICC_IGRPEN0_EL1_set(v)		__asm__ volatile ("MSR S3_0_C12_C12_6, %0"::"r"(v):"memory")									/* RW Interrupt Controller Interrupt Group 0 Enable register */
#define ICC_IGRPEN1_EL1_get()		({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_0_C12_C12_7":"=r"(_reg_)::"memory");_reg_;})	/* RW Interrupt Controller Interrupt Group 1 Enable register */
#define ICC_IGRPEN1_EL1_set(v)		__asm__ volatile ("MSR S3_0_C12_C12_7, %0"::"r"(v):"memory")									/* RW Interrupt Controller Interrupt Group 1 Enable register */
#define ICC_IGRPEN1_EL3_get()		({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_6_C12_C12_7":"=r"(_reg_)::"memory");_reg_;})	/* RW Interrupt Controller Interrupt Group 1 Enable register (EL3) */
#define ICC_IGRPEN1_EL3_set(v)		__asm__ volatile ("MSR S3_6_C12_C12_7, %0"::"r"(v):"memory")									/* RW Interrupt Controller Interrupt Group 1 Enable register (EL3) */
#define ICC_PMR_EL1_get()			({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_0_C4_C6_0":"=r"(_reg_)::"memory");_reg_;})		/* RW Interrupt Controller Interrupt Priority Mask Register */
#define ICC_PMR_EL1_set(v)			__asm__ volatile ("MSR S3_0_C4_C6_0, %0"::"r"(v):"memory")										/* RW Interrupt Controller Interrupt Priority Mask Register */
#define ICC_RPR_EL1_get()			({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_0_C12_C11_3":"=r"(_reg_)::"memory");_reg_;})		/* RO Interrupt Controller Running Priority Register */
#define ICC_SGI0R_EL1_set(v)		__asm__ volatile ("MSR S3_0_C12_C11_7, %0"::"r"(v):"memory")										/* WO Interrupt Controller Software Generated Interrupt Group 0 Register */
#define ICC_SGI1R_EL1_set(v)		__asm__ volatile ("MSR S3_0_C12_C11_5, %0"::"r"(v):"memory")										/* WO Interrupt Controller Software Generated Interrupt Group 1 Register */
#define ICC_SRE_EL1_get()			({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_0_C12_C12_5":"=r"(_reg_)::"memory");_reg_;})		/* RW Interrupt Controller System Register Enable register (EL1) */
#define ICC_SRE_EL1_set(v)			__asm__ volatile ("MSR S3_0_C12_C12_5, %0"::"r"(v):"memory")										/* RW Interrupt Controller System Register Enable register (EL1) */
#define ICC_SRE_EL2_get()			({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_4_C12_C9_5":"=r"(_reg_)::"memory");_reg_;})		/* RW Interrupt Controller System Register Enable register (EL2) */
#define ICC_SRE_EL2_set(v)			__asm__ volatile ("MSR S3_4_C12_C9_5, %0"::"r"(v):"memory")										/* RW Interrupt Controller System Register Enable register (EL2) */
#define ICC_SRE_EL3_get()			({uint64_t _reg_;__asm__ volatile ("MRS %0, S3_6_C12_C12_5":"=r"(_reg_)::"memory");_reg_;})		/* RW Interrupt Controller System Register Enable register (EL3) */
#define ICC_SRE_EL3_set(v)			__asm__ volatile ("MSR S3_6_C12_C12_5, %0"::"r"(v):"memory")										/* RW Interrupt Controller System Register Enable register (EL3) */

#endif

#endif
