/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * cruregs.h
 *
 *  Created on: 2015/04/04
 *      Author: biwa
 */

#ifndef _CRUREGS_H_
#define _CRUREGS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define	CRU_APLL_CON0				(0x0000u)					/* ARM PLL configuration register0 */
#define	CRU_APLL_CON1				(0x0004u)					/* ARM PLL configuration register1 */
#define	CRU_APLL_CON2				(0x0008u)					/* ARM PLL configuration register2 */
#define	CRU_APLL_CON3				(0x000Cu)					/* ARM PLL configuration register3 */
#define	CRU_DPLL_CON0				(0x0010u)					/* DDR PLL configuration register0 */
#define	CRU_DPLL_CON1				(0x0014u)					/* DDR PLL configuration register1 */
#define	CRU_DPLL_CON2				(0x0018u)					/* DDR PLL configuration register2 */
#define	CRU_DPLL_CON3				(0x001Cu)					/* DDR PLL configuration register3 */
#define	CRU_CPLL_CON0				(0x0020u)					/* CODEC PLL configuration register0 */
#define	CRU_CPLL_CON1				(0x0024u)					/* CODEC PLL configuration register1 */
#define	CRU_CPLL_CON2				(0x0028u)					/* CODEC PLL configuration register2 */
#define	CRU_CPLL_CON3				(0x002Cu)					/* CODEC PLL configuration register3 */
#define	CRU_GPLL_CON0				(0x0030u)					/* GENERAL PLL configuration register0 */
#define	CRU_GPLL_CON1				(0x0034u)					/* GENERAL PLL configuration register1 */
#define	CRU_GPLL_CON2				(0x0038u)					/* GENERAL PLL configuration register2 */
#define	CRU_GPLL_CON3				(0x003Cu)					/* GENERAL PLL configuration register3 */
#define	CRU_NPLL_CON0				(0x0040u)					/* NEW PLL configuration register0 */
#define	CRU_NPLL_CON1				(0x0044u)					/* NEW PLL configuration register1 */
#define	CRU_NPLL_CON2				(0x0048u)					/* NEW PLL configuration register2 */
#define	CRU_NPLL_CON3				(0x004Cu)					/* NEW PLL configuration register3 */
#define	CRU_MODE_CON				(0x0050u)					/* System work mode control register */
#define	CRU_CLKSEL0_CON				(0x0060u)					/* Internal clock select and divide register0 */
#define	CRU_CLKSEL1_CON				(0x0064u)					/* Internal clock select and divide register1 */
#define	CRU_CLKSEL2_CON				(0x0068u)					/* Internal clock select and divide register2 */
#define	CRU_CLKSEL3_CON				(0x006Cu)					/* Internal clock select and divide register3 */
#define	CRU_CLKSEL4_CON				(0x0070u)					/* Internal clock select and divide register4 */
#define	CRU_CLKSEL5_CON				(0x0074u)					/* Internal clock select and divide register5 */
#define	CRU_CLKSEL6_CON				(0x0078u)					/* Internal clock select and divide register6 */
#define	CRU_CLKSEL7_CON				(0x007Cu)					/* Internal clock select and divide register7 */
#define	CRU_CLKSEL8_CON				(0x0080u)					/* Internal clock select and divide register8 */
#define	CRU_CLKSEL9_CON				(0x0084u)					/* Internal clock select and divide register9 */
#define	CRU_CLKSEL10_CON			(0x0088u)					/* Internal clock select and divide register10 */
#define	CRU_CLKSEL11_CON			(0x008Cu)					/* Internal clock select and divide register11 */
#define	CRU_CLKSEL12_CON			(0x0090u)					/* Internal clock select and divide register12 */
#define	CRU_CLKSEL13_CON			(0x0094u)					/* Internal clock select and divide register13 */
#define	CRU_CLKSEL14_CON			(0x0098u)					/* Internal clock select and divide register14 */
#define	CRU_CLKSEL15_CON			(0x009Cu)					/* Internal clock select and divide register15 */
#define	CRU_CLKSEL16_CON			(0x00A0u)					/* Internal clock select and divide register16 */
#define	CRU_CLKSEL17_CON			(0x00A4u)					/* Internal clock select and divide register17 */
#define	CRU_CLKSEL18_CON			(0x00A8u)					/* Internal clock select and divide register18 */
#define	CRU_CLKSEL19_CON			(0x00ACu)					/* Internal clock select and divide register19 */
#define	CRU_CLKSEL20_CON			(0x00B0u)					/* Internal clock select and divide register20 */
#define	CRU_CLKSEL21_CON			(0x00B4u)					/* Internal clock select and divide register21 */
#define	CRU_CLKSEL22_CON			(0x00B8u)					/* Internal clock select and divide register22 */
#define	CRU_CLKSEL23_CON			(0x00BCu)					/* Internal clock select and divide register23 */
#define	CRU_CLKSEL24_CON			(0x00C0u)					/* Internal clock select and divide register24 */
#define	CRU_CLKSEL25_CON			(0x00C4u)					/* Internal clock select and divide register25 */
#define	CRU_CLKSEL26_CON			(0x00C8u)					/* Internal clock select and divide register26 */
#define	CRU_CLKSEL27_CON			(0x00CCu)					/* Internal clock select and divide register27 */
#define	CRU_CLKSEL28_CON			(0x00D0u)					/* Internal clock select and divide register28 */
#define	CRU_CLKSEL29_CON			(0x00D4u)					/* Internal clock select and divide register29 */
#define	CRU_CLKSEL30_CON			(0x00D8u)					/* Internal clock select and divide register30 */
#define	CRU_CLKSEL31_CON			(0x00DCu)					/* Internal clock select and divide register31 */
#define	CRU_CLKSEL32_CON			(0x00E0u)					/* Internal clock select and divide register32 */
#define	CRU_CLKSEL33_CON			(0x00E4u)					/* Internal clock select and divide register33 */
#define	CRU_CLKSEL34_CON			(0x00E8u)					/* Internal clock select and divide register34 */
#define	CRU_CLKSEL35_CON			(0x00ECu)					/* Internal clock select and divide register35 */
#define	CRU_CLKSEL36_CON			(0x00F0u)					/* Internal clock select and divide register36 */
#define	CRU_CLKSEL37_CON			(0x00F4u)					/* Internal clock select and divide register37 */
#define	CRU_CLKSEL38_CON			(0x00F8u)					/* nternal clock select and divide register38 */
#define	CRU_CLKSEL39_CON			(0x00FCu)					/* nternal clock select and divide register39 */
#define	CRU_CLKSEL40_CON			(0x0100u)					/* nternal clock select and divide register40 */
#define	CRU_CLKSEL41_CON			(0x0104u)					/* nternal clock select and divide register41 */
#define	CRU_CLKSEL42_CON			(0x0108u)					/* nternal clock select and divide register42 */
#define	CRU_CLKGATE0_CON			(0x0160u)					/* nternal clock gating control register0 */
#define	CRU_CLKGATE1_CON			(0x0164u)					/* nternal clock gating control register1 */
#define	CRU_CLKGATE2_CON			(0x0168u)					/* nternal clock gating control register2 */
#define	CRU_CLKGATE3_CON			(0x016Cu)					/* nternal clock gating control register3 */
#define	CRU_CLKGATE4_CON			(0x0170u)					/* nternal clock gating control register4 */
#define	CRU_CLKGATE5_CON			(0x0174u)					/* nternal clock gating control register5 */
#define	CRU_CLKGATE6_CON			(0x0178u)					/* nternal clock gating control register6 */
#define	CRU_CLKGATE7_CON			(0x017Cu)					/* nternal clock gating control register7 */
#define	CRU_CLKGATE8_CON			(0x0180u)					/* nternal clock gating control register8 */
#define	CRU_CLKGATE9_CON			(0x0184u)					/* nternal clock gating control register9 */
#define	CRU_CLKGATE10_CON			(0x0188u)					/* Internal clock gating control register10 */
#define	CRU_CLKGATE11_CON			(0x018Cu)					/* Internal clock gating control register11 */
#define	CRU_CLKGATE12_CON			(0x0190u)					/* Internal clock gating control register12 */
#define	CRU_CLKGATE13_CON			(0x0194u)					/* Internal clock gating control register13 */
#define	CRU_CLKGATE14_CON			(0x0198u)					/* Internal clock gating control register14 */
#define	CRU_CLKGATE15_CON			(0x019Cu)					/* Internal clock gating control register15 */
#define	CRU_CLKGATE16_CON			(0x01A0u)					/* Internal clock gating control register16 */
#define	CRU_CLKGATE17_CON			(0x01A4u)					/* Internal clock gating control register17 */
#define	CRU_CLKGATE18_CON			(0x01A8u)					/* Internal clock gating control register18 */
#define	CRU_GLB_SRST_FST_VALUE		(0x01B0u)					/* The first global software reset config value */
#define	CRU_GLB_SRST_SND_VALUE		(0x01B4u)					/* The second global software reset config value */
#define	CRU_SOFTRST0_CON			(0x01B8u)					/* nternal software reset control register0 */
#define	CRU_SOFTRST1_CON			(0x01BCu)					/* nternal software reset control register1 */
#define	CRU_SOFTRST2_CON			(0x01C0u)					/* nternal software reset control register2 */
#define	CRU_SOFTRST3_CON			(0x01C4u)					/* nternal software reset control register3 */
#define	CRU_SOFTRST4_CON			(0x01C8u)					/* nternal software reset control register4 */
#define	CRU_SOFTRST5_CON			(0x01CCu)					/* nternal software reset control register5 */
#define	CRU_SOFTRST6_CON			(0x01D0u)					/* nternal software reset control register6 */
#define	CRU_SOFTRST7_CON			(0x01D4u)					/* nternal software reset control register7 */
#define	CRU_SOFTRST8_CON			(0x01D8u)					/* nternal software reset control register8 */
#define	CRU_SOFTRST9_CON			(0x01DCu)					/* nternal software reset control register9 */
#define	CRU_SOFTRST10_CON			(0x01E0u)					/* Internal software reset control register10 */
#define	CRU_SOFTRST11_CON			(0x01E4u)					/* Internal software reset control register11 */
#define	CRU_MISC_CON				(0x01E8u)					/* SCU control register */
#define	CRU_GLB_CNT_TH				(0x01ECu)					/* global reset wait counter threshold */
#define	CRU_GLB_RST_CON				(0x01F0u)					/* global reset trigger select */
#define	CRU_GLB_RST_ST				(0x01F8u)					/* global reset status */
#define	CRU_SDMMC_CON0				(0x0200u)					/* sdmmc control0 */
#define	CRU_SDMMC_CON1				(0x0204u)					/* sdmmc control1 */
#define	CRU_SDIO0_CON0				(0x0208u)					/* sdio0 control0 */
#define	CRU_SDIO0_CON1				(0x020Cu)					/* sdio0 control1 */
#define	CRU_SDIO1_CON0				(0x0210u)					/* sdio1 control0 */
#define	CRU_SDIO1_CON1				(0x0214u)					/* sdio1 control1 */
#define	CRU_EMMC_CON0				(0x0218u)					/* emmc control0 */
#define	CRU_EMMC_CON1				(0x021Cu)					/* emmc control1 */

#ifdef __cplusplus
}
#endif

#endif /* _CRUREGS_H_ */
