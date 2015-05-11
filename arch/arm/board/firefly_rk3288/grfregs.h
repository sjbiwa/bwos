/*
 * grfregs.h
 *
 *  Created on: 2015/04/04
 *      Author: biwa
 */

#ifndef _GRFREGS_H_
#define _GRFREGS_H_

#define	GRF_GPIO1D_IOMUX				(0x000Cu)		/* GPIO1D iomux control */
#define	GRF_GPIO2A_IOMUX				(0x0010u)		/* GPIO2A iomux control */
#define	GRF_GPIO2B_IOMUX				(0x0014u)		/* GPIO2B iomux control */
#define	GRF_GPIO2C_IOMUX				(0x0018u)		/* GPIO2C iomux control */
#define	GRF_GPIO3A_IOMUX				(0x0020u)		/* GPIO3A iomux control */
#define	GRF_GPIO3B_IOMUX				(0x0024u)		/* GPIO3B iomux control */
#define	GRF_GPIO3C_IOMUX				(0x0028u)		/* GPIO3C iomux control */
#define	GRF_GPIO3DL_IOMUX				(0x002Cu)		/* GPIO3D iomux control */
#define	GRF_GPIO3DH_IOMUX				(0x0030u)		/* GPIO3D iomux control */
#define	GRF_GPIO4AL_IOMUX				(0x0034u)		/* GPIO4A iomux control */
#define	GRF_GPIO4AH_IOMUX				(0x0038u)		/* GPIO4A iomux control */
#define	GRF_GPIO4BL_IOMUX				(0x003Cu)		/* GPIO4B iomux control */
#define	GRF_GPIO4C_IOMUX				(0x0044u)		/* GPIO4C iomux control */
#define	GRF_GPIO4D_IOMUX				(0x0048u)		/* GPIO4D iomux control */
#define	GRF_GPIO5B_IOMUX				(0x0050u)		/* GPIO5B iomux control */
#define	GRF_GPIO5C_IOMUX				(0x0054u)		/* GPIO5C iomux control */
#define	GRF_GPIO6A_IOMUX				(0x005Cu)		/* GPIO6A iomux control */
#define	GRF_GPIO6B_IOMUX				(0x0060u)		/* GPIO6B iomux control */
#define	GRF_GPIO6C_IOMUX				(0x0064u)		/* GPIO6C iomux control */
#define	GRF_GPIO7A_IOMUX				(0x006Cu)		/* GPIO7A iomux control */
#define	GRF_GPIO7B_IOMUX				(0x0070u)		/* GPIO7B iomux control */
#define	GRF_GPIO7CL_IOMUX				(0x0074u)		/* GPIO7CL iomux control */
#define	GRF_GPIO7CH_IOMUX				(0x0078u)		/* GPIO7CH iomux control */
#define	GRF_GPIO8A_IOMUX				(0x0080u)		/* GPIO8A iomux control */
#define	GRF_GPIO8B_IOMUX				(0x0084u)		/* GPIO8B iomux control */
#define	GRF_GPIO1H_SR					(0x0104u)		/* GPIO1C/D SR control */
#define	GRF_GPIO2L_SR					(0x0108u)		/* GPIO2A/B SR control */
#define	GRF_GPIO2H_SR					(0x010Cu)		/* GPIO2C/D SR control */
#define	GRF_GPIO3L_SR					(0x0110u)		/* GPIO3A/B SR control */
#define	GRF_GPIO3H_SR					(0x0114u)		/* GPIO3C/D SR control */
#define	GRF_GPIO4L_SR					(0x0118u)		/* GPIO4A/B SR control */
#define	GRF_GPIO4H_SR					(0x011Cu)		/* GPIO4C/D SR control */
#define	GRF_GPIO5L_SR					(0x0120u)		/* GPIO5A/B SR control */
#define	GRF_GPIO5H_SR					(0x0124u)		/* GPIO5C/D SR control */
#define	GRF_GPIO6L_SR					(0x0128u)		/* GPIO6A/B SR control */
#define	GRF_GPIO6H_SR					(0x012Cu)		/* GPIO6C/D SR control */
#define	GRF_GPIO7L_SR					(0x0130u)		/* GPIO7A/B SR control */
#define	GRF_GPIO7H_SR					(0x0134u)		/* GPIO7C/D SR control */
#define	GRF_GPIO8L_SR					(0x0138u)		/* GPIO8A/B SR control */
#define	GRF_GPIO1D_P					(0x014Cu)		/* GPIO1D PU/PD control */
#define	GRF_GPIO2A_P					(0x0150u)		/* GPIO2A PU/PD control */
#define	GRF_GPIO2B_P					(0x0154u)		/* GPIO2B PU/PD control */
#define	GRF_GPIO2C_P					(0x0158u)		/* GPIO2C PU/PD control */
#define	GRF_GPIO3A_P					(0x0160u)		/* GPIO3A PU/PD control */
#define	GRF_GPIO3B_P					(0x0164u)		/* GPIO3B PU/PD control */
#define	GRF_GPIO3C_P					(0x0168u)		/* GPIO3C PU/PD control */
#define	GRF_GPIO3D_P					(0x016Cu)		/* GPIO3D PU/PD control */
#define	GRF_GPIO4A_P					(0x0170u)		/* GPIO4A PU/PD control */
#define	GRF_GPIO4B_P					(0x0174u)		/* GPIO4B PU/PD control */
#define	GRF_GPIO4C_P					(0x0178u)		/* GPIO4C PU/PD control */
#define	GRF_GPIO4D_P					(0x017Cu)		/* GPIO4D PU/PD control */
#define	GRF_GPIO5B_P					(0x0184u)		/* GPIO5B PU/PD control */
#define	GRF_GPIO5C_P					(0x0188u)		/* GPIO5C PU/PD control */
#define	GRF_GPIO6A_P					(0x0190u)		/* GPIO6A PU/PD control */
#define	GRF_GPIO6B_P					(0x0194u)		/* GPIO6B PU/PD control */
#define	GRF_GPIO6C_P					(0x0198u)		/* GPIO6C PU/PD control */
#define	GRF_GPIO7A_P					(0x01A0u)		/* GPIO7A PU/PD control */
#define	GRF_GPIO7B_P					(0x01A4u)		/* GPIO7B PU/PD control */
#define	GRF_GPIO7C_P					(0x01A8u)		/* GPIO7C PU/PD control */
#define	GRF_GPIO8A_P					(0x01B0u)		/* GPIO8A PU/PD control */
#define	GRF_GPIO8B_P					(0x01B4u)		/* GPIO8B PU/PD control */
#define	GRF_GPIO1D_E					(0x01CCu)		/* GPIO1D drive strength control */
#define	GRF_GPIO2A_E					(0x01D0u)		/* GPIO2A drive strength control */
#define	GRF_GPIO2B_E					(0x01D4u)		/* GPIO2B drive strength control */
#define	GRF_GPIO2C_E					(0x01D8u)		/* GPIO2C drive strength control */
#define	GRF_GPIO3A_E					(0x01E0u)		/* GPIO3A drive strength control */
#define	GRF_GPIO3B_E					(0x01E4u)		/* GPIO3B drive strength control */
#define	GRF_GPIO3C_E					(0x01E8u)		/* GPIO3C drive strength control */
#define	GRF_GPIO3D_E					(0x01ECu)		/* GPIO3D drive strength control */
#define	GRF_GPIO4A_E					(0x01F0u)		/* GPIO4A drive strength control */
#define	GRF_GPIO4B_E					(0x01F4u)		/* GPIO4B drive strength control */
#define	GRF_GPIO4C_E					(0x01F8u)		/* GPIO4C drive strength control */
#define	GRF_GPIO4D_E					(0x01FCu)		/* GPIO4D drive strength control */
#define	GRF_GPIO5B_E					(0x0204u)		/* GPIO5B drive strength control */
#define	GRF_GPIO5C_E					(0x0208u)		/* GPIO5C drive strength control */
#define	GRF_GPIO6A_E					(0x0210u)		/* GPIO6A drive strength control */
#define	GRF_GPIO6B_E					(0x0214u)		/* GPIO6B drive strength control */
#define	GRF_GPIO6C_E					(0x0218u)		/* GPIO6C drive strength control */
#define	GRF_GPIO7A_E					(0x0220u)		/* GPIO7A drive strength control */
#define	GRF_GPIO7B_E					(0x0224u)		/* GPIO7B drive strength control */
#define	GRF_GPIO7C_E					(0x0228u)		/* GPIO7C drive strength control */
#define	GRF_GPIO8A_E					(0x0230u)		/* GPIO8A drive strength control */
#define	GRF_GPIO8B_E					(0x0234u)		/* GPIO8B drive strength control */
#define	GRF_GPIO_SMT					(0x0240u)		/* GPIO smitter control register */
#define	GRF_SOC_CON0					(0x0244u)		/* SoC control register 0 */
#define	GRF_SOC_CON1					(0x0248u)		/* SoC control register 1 */
#define	GRF_SOC_CON2					(0x024Cu)		/* SoC control register 2 */
#define	GRF_SOC_CON3					(0x0250u)		/* SoC control register 3 */
#define	GRF_SOC_CON4					(0x0254u)		/* SoC control register 4 */
#define	GRF_SOC_CON5					(0x0258u)		/* SoC control register 5 */
#define	GRF_SOC_CON6					(0x025Cu)		/* SoC control register 6 */
#define	GRF_SOC_CON7					(0x0260u)		/* SoC control register 7 */
#define	GRF_SOC_CON8					(0x0264u)		/* SoC control register 8 */
#define	GRF_SOC_CON9					(0x0268u)		/* SoC control register 9 */
#define	GRF_SOC_CON10					(0x026Cu)		/*  SoC control register 10 */
#define	GRF_SOC_CON11					(0x0270u)		/*  SoC control register 11 */
#define	GRF_SOC_CON12					(0x0274u)		/*  SoC control register 12 */
#define	GRF_SOC_CON13					(0x0278u)		/*  SoC control register 13 */
#define	GRF_SOC_CON14					(0x027Cu)		/*  SoC control register 14 */
#define	GRF_SOC_STATUS0					(0x0280u)		/* SoC status register 0 */
#define	GRF_SOC_STATUS1					(0x0284u)		/* SoC status register 1 */
#define	GRF_SOC_STATUS2					(0x0288u)		/* SoC status register 2 */
#define	GRF_SOC_STATUS3					(0x028Cu)		/* SoC status register 3 */
#define	GRF_SOC_STATUS4					(0x0290u)		/* SoC status register 4 */
#define	GRF_SOC_STATUS5					(0x0294u)		/* SoC status register 5 */
#define	GRF_SOC_STATUS6					(0x0298u)		/* SoC status register 6 */
#define	GRF_SOC_STATUS7					(0x029Cu)		/* SoC status register 7 */
#define	GRF_SOC_STATUS8					(0x02A0u)		/* SoC status register 8 */
#define	GRF_SOC_STATUS9					(0x02A4u)		/* SoC status register 9 */
#define	GRF_SOC_STATUS10				(0x02A8u)		/*  SoC status register 10 */
#define	GRF_SOC_STATUS11				(0x02ACu)		/*  SoC status register 11 */
#define	GRF_SOC_STATUS12				(0x02B0u)		/*  SoC status register 12 */
#define	GRF_SOC_STATUS13				(0x02B4u)		/*  SoC status register 13 */
#define	GRF_SOC_STATUS14				(0x02B8u)		/*  SoC status register 14 */
#define	GRF_SOC_STATUS15				(0x02BCu)		/*  SoC status register 15 */
#define	GRF_SOC_STATUS16				(0x02C0u)		/*  SoC status register 16 */
#define	GRF_SOC_STATUS17				(0x02C4u)		/*  SoC status register 17 */
#define	GRF_SOC_STATUS18				(0x02C8u)		/*  SoC status register 18 */
#define	GRF_SOC_STATUS19				(0x02CCu)		/*  SoC status register 19 */
#define	GRF_SOC_STATUS20				(0x02D0u)		/*  SoC status register 20 */
#define	GRF_SOC_STATUS21				(0x02D4u)		/*  SoC status register 21 */
#define	GRF_PERIDMAC_CON0				(0x02E0u)		/* PERI DMAC control register 0 */
#define	GRF_PERIDMAC_CON1				(0x02E4u)		/* PERI DMAC control register 1 */
#define	GRF_PERIDMAC_CON2				(0x02E8u)		/* PERI DMAC control register 2 */
#define	GRF_PERIDMAC_CON3				(0x02ECu)		/* PERI DMAC control register 3 */
#define	GRF_DDRC0_CON0					(0x02F0u)		/* DDRC0 control register 0 */
#define	GRF_DDRC1_CON0					(0x02F4u)		/* DDRC1 control register 0 */
#define	GRF_CPU_CON0					(0x02F8u)		/* CPU control register 0 */
#define	GRF_CPU_CON1					(0x02FCu)		/* CPU control register 1 */
#define	GRF_CPU_CON2					(0x0300u)		/* CPU control register 2 */
#define	GRF_CPU_CON3					(0x0304u)		/* CPU control register 3 */
#define	GRF_CPU_CON4					(0x0308u)		/* CPU control register 4 */
#define	GRF_CPU_STATUS0					(0x0318u)		/* CPU status register 0 */
#define	GRF_UOC0_CON0					(0x0320u)		/* UOC0 control register 0 */
#define	GRF_UOC0_CON1					(0x0324u)		/* UOC0 control register 1 */
#define	GRF_UOC0_CON2					(0x0328u)		/* UOC0 control register 2 */
#define	GRF_UOC0_CON3					(0x032Cu)		/* UOC0 control register 3 */
#define	GRF_UOC0_CON4					(0x0330u)		/* UOC0 control register 4 */
#define	GRF_UOC1_CON0					(0x0334u)		/* UOC1 control register 0 */
#define	GRF_UOC1_CON1					(0x0338u)		/* UOC1 control register 1 */
#define	GRF_UOC1_CON2					(0x033Cu)		/* UOC1 control register 2 */
#define	GRF_UOC1_CON3					(0x0340u)		/* UOC1 control register 3 */
#define	GRF_UOC1_CON4					(0x0344u)		/* UOC1 control register 4 */
#define	GRF_UOC2_CON0					(0x0348u)		/* UOC2 control register 0 */
#define	GRF_UOC2_CON1					(0x034Cu)		/* UOC2 control register 1 */
#define	GRF_UOC2_CON2					(0x0350u)		/* UOC2 control register 2 */
#define	GRF_UOC2_CON3					(0x0354u)		/* UOC2 control register 3 */
#define	GRF_UOC3_CON0					(0x0358u)		/* UOC3 control register 0 */
#define	GRF_UOC3_CON1					(0x035Cu)		/* UOC3 control register 1 */
#define	GRF_UOC4_CON0					(0x0360u)		/* UOC4 control register 0 */
#define	GRF_UOC4_CON1					(0x0364u)		/* UOC4 control register 1 */
#define	GRF_PVTM_CON0					(0x0368u)		/* PVT monitor control register 0 */
#define	GRF_PVTM_CON1					(0x036Cu)		/* PVT monitor control register 1 */
#define	GRF_PVTM_CON2					(0x0370u)		/* PVT monitor control register 2 */
#define	GRF_PVTM_STATUS0				(0x0374u)		/* PVT monitor status register 0 */
#define	GRF_PVTM_STATUS1				(0x0378u)		/* PVT monitor status register 1 */
#define	GRF_PVTM_STATUS2				(0x037Cu)		/* PVT monitor status register 2 */
#define	GRF_IO_VSEL						(0x0380u)		/* IO voltage select */
#define	GRF_SARADC_TESTBIT				(0x0384u)		/* SARADC Test bit register */
#define	GRF_TSADC_TESTBIT_L				(0x0388u)		/* TSADC Test bit low register */
#define	GRF_TSADC_TESTBIT_H				(0x038Cu)		/* TSADC Test bit high register */
#define	GRF_OS_REG0						(0x0390u)		/* OS register 0 */
#define	GRF_OS_REG1						(0x0394u)		/* OS register 1 */
#define	GRF_OS_REG2						(0x0398u)		/* OS register 2 */
#define	GRF_OS_REG3						(0x039Cu)		/* OS register 3 */
#define	GRF_SOC_CON15					(0x03A4u)		/* SoC control register 15 */
#define	GRF_SOC_CON16					(0x03A8u)		/* SoC control register 16 */

#endif /* _GRFREGS_H_ */
