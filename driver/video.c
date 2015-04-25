/*
 * video.c
 *
 *  Created on: 2015/02/01
 *      Author: biwa
 */
#include "bwos.h"
#include "driver/clock.h"
#include "driver/video.h"
#include "ioregs.h"

#define	VOP_REG_CFG_DONE			(0x0000u)			/*0x00000000*/	/* Register config done flag */
#define	VOP_VERSION_INFO			(0x0004u)			/*0x00000000*/	/*  */
#define	VOP_SYS_CTRL				(0x0008u)			/*0x00801000*/	/* System control register0 */
#define	VOP_SYS_CTRL1				(0x000cu)			/*0x00000000*/	/*  */
#define	VOP_DSP_CTRL0				(0x0010u)			/*0x00000000*/	/* Display control register0 */
#define	VOP_DSP_CTRL1				(0x0014u)			/*0x0000e400*/	/* Display control register1 */
#define	VOP_DSP_BG					(0x0018u)			/*0x00000000*/	/* background color */
#define	VOP_MCU_CTRL				(0x001cu)			/*0x00711c08*/	/* MCU mode control register */
#define	VOP_INTR_CTRL0				(0x0020u)			/*0x00000000*/	/* Interrupt ctrl register0 */
#define	VOP_INTR_CTRL1				(0x0024u)			/*0x00000000*/	/* Interrupt ctrl register1 */
#define	VOP_INTR_RESERVED0			(0x0028u)			/*0x00000000*/	/*  */
#define	VOP_INTR_RESERVED1			(0x002cu)			/*0x00000000*/	/*  */
#define	VOP_WIN0_CTRL0				(0x0030u)			/*0x00000040*/	/* win0 ctrl register0 */
#define	VOP_WIN0_CTRL1				(0x0034u)			/*0x00000000*/	/* win1 ctrl register1 */
#define	VOP_WIN0_COLOR_KEY			(0x0038u)			/*0x00000000*/	/* Win0 color key register */
#define	VOP_WIN0_VIR				(0x003cu)			/*0x00000140*/	/* Win0 virtual stride */
#define	VOP_WIN0_YRGB_MST			(0x0040u)			/*0x00000000*/	/* Win0 YRGB memory start address */
#define	VOP_WIN0_CBR_MST			(0x0044u)			/*0x00000000*/	/* Win0 Cbr memory start address */
#define	VOP_WIN0_ACT_INFO			(0x0048u)			/*0x00ef013f*/	/* Win0 active window width/height */
#define	VOP_WIN0_DSP_INFO			(0x004cu)			/*0x00ef013f*/	/* Win0 display width/height on panel */
#define	VOP_WIN0_DSP_ST				(0x0050u)			/*0x000a000a*/	/* Win0 display start point on panel */
#define	VOP_WIN0_SCL_FACTOR_YRGB	(0x0054u)			/*0x10001000*/	/* Win0 YRGB scaling factor */
#define	VOP_WIN0_SCL_FACTOR_CBR		(0x0058u)			/*0x10001000*/	/* Win0 Cbr scaling factor */
#define	VOP_WIN0_SCL_OFFSET			(0x005cu)			/*0x00000000*/	/* Win0 scaling start point offset */
#define	VOP_WIN0_SRC_ALPHA_CTRL		(0x0060u)			/*0x00000000*/	/*  */
#define	VOP_WIN0_DST_ALPHA_CTRL		(0x0064u)			/*0x00000000*/	/*  */
#define	VOP_WIN0_FADING_CTRL		(0x0068u)			/*0x00000000*/	/*  */
#define	VOP_WIN0_RESERVED0			(0x006cu)			/*0x00000000*/	/*  */
#define	VOP_WIN1_CTRL0				(0x0070u)			/*0x00000040*/	/* win1 ctrl register0 */
#define	VOP_WIN1_CTRL1				(0x0074u)			/*0x00000000*/	/* win1 ctrl register1 */
#define	VOP_WIN1_COLOR_KEY			(0x0078u)			/*0x00000000*/	/* Win1 color key register */
#define	VOP_WIN1_VIR				(0x007cu)			/*0x00000140*/	/* win1 virtual stride */
#define	VOP_WIN1_YRGB_MST			(0x0080u)			/*0x00000000*/	/* Win1 YRGB memory start address */
#define	VOP_WIN1_CBR_MST			(0x0084u)			/*0x00000000*/	/* Win1 Cbr memory start address */
#define	VOP_WIN1_ACT_INFO			(0x0088u)			/*0x00ef013f*/	/* Win1 active window width/height */
#define	VOP_WIN1_DSP_INFO			(0x008cu)			/*0x00ef013f*/	/* Win1 display width/height on panel */
#define	VOP_WIN1_DSP_ST				(0x0090u)			/*0x000a000a*/	/* Win1 display start point on panel */
#define	VOP_WIN1_SCL_FACTOR_YRGB	(0x0094u)			/*0x10001000*/	/* Win1 YRGB scaling factor */
#define	VOP_WIN1_SCL_FACTOR_CBR		(0x0098u)			/*0x10001000*/	/* Win1 Cbr scaling factor */
#define	VOP_WIN1_SCL_OFFSET			(0x009cu)			/*0x00000000*/	/* Win1 scaling start point offset */
#define	VOP_WIN1_SRC_ALPHA_CTRL		(0x00a0u)			/*0x00000000*/	/*  */
#define	VOP_WIN1_DST_ALPHA_CTRL		(0x00a4u)			/*0x00000000*/	/*  */
#define	VOP_WIN1_FADING_CTRL		(0x00a8u)			/*0x00000000*/	/*  */
#define	VOP_WIN1_RESERVED0			(0x00acu)			/*0x00000000*/	/*  */
#define	VOP_WIN2_CTRL0				(0x00b0u)			/*0x00000000*/	/* win2 ctrl register0 */
#define	VOP_WIN2_CTRL1				(0x00b4u)			/*0x00000000*/	/* win2 ctrl register0 */
#define	VOP_WIN2_VIR0_1				(0x00b8u)			/*0x01400140*/	/* Win2 virtual stride0 and virtaul stride1 */
#define	VOP_WIN2_VIR2_3				(0x00bcu)			/*0x01400140*/	/* Win2 virtual stride2 and virtaul stride3 */
#define	VOP_WIN2_MST0				(0x00c0u)			/*0x00000000*/	/* Win2 memory start address0 */
#define	VOP_WIN2_DSP_INFO0			(0x00c4u)			/*0x00ef013f*/	/* Win2 display width0/height0 on panel */
#define	VOP_WIN2_DSP_ST0			(0x00c8u)			/*0x000a000a*/	/* Win2 display start point0 on panel */
#define	VOP_WIN2_COLOR_KEY			(0x00ccu)			/*0x00000000*/	/* Win2 color key register */
#define	VOP_WIN2_MST1				(0x00d0u)			/*0x00000000*/	/* Win2 memory start address1 */
#define	VOP_WIN2_DSP_INFO1			(0x00d4u)			/*0x00ef013f*/	/* Win2 display width1/height1 on panel */
#define	VOP_WIN2_DSP_ST1			(0x00d8u)			/*0x000a000a*/	/* Win2 display start point1 on panel */
#define	VOP_WIN2_SRC_ALPHA_CTRL		(0x00dcu)			/*0x00000000*/	/*  */
#define	VOP_WIN2_MST2				(0x00e0u)			/*0x00000000*/	/* Win2 memory start address2 */
#define	VOP_WIN2_DSP_INFO2			(0x00e4u)			/*0x00ef013f*/	/* Win2 display width2/height2 on panel */
#define	VOP_WIN2_DSP_ST2			(0x00e8u)			/*0x000a000a*/	/* Win2 display start point2 on panel */
#define	VOP_WIN2_DST_ALPHA_CTRL		(0x00ecu)			/*0x00000000*/	/*  */
#define	VOP_WIN2_MST3				(0x00f0u)			/*0x00000000*/	/* Win2 memory start address3 */
#define	VOP_WIN2_DSP_INFO3			(0x00f4u)			/*0x00ef013f*/	/* Win2 display width3/height3 on panel */
#define	VOP_WIN2_DSP_ST3			(0x00f8u)			/*0x000a000a*/	/* Win2 display start point3 on panel */
#define	VOP_WIN2_FADING_CTRL		(0x00fcu)			/*0x00000000*/	/*  */
#define	VOP_WIN3_CTRL0				(0x0100u)			/*0x00000000*/	/* win0 ctrl register0 */
#define	VOP_WIN3_CTRL1				(0x0104u)			/*0x00000000*/	/* win0 ctrl register1 */
#define	VOP_WIN3_VIR0_1				(0x0108u)			/*0x01400140*/	/* Win3 virtual stride0 and virtaul stride1 */
#define	VOP_WIN3_VIR2_3				(0x010cu)			/*0x01400140*/	/* Win3 virtual stride2 and virtaul stride3 */
#define	VOP_WIN3_MST0				(0x0110u)			/*0x00000000*/	/* Win3 memory start address0 */
#define	VOP_WIN3_DSP_INFO0			(0x0114u)			/*0x00ef013f*/	/* Win3 display width0/height0 on panel */
#define	VOP_WIN3_DSP_ST0			(0x0118u)			/*0x000a000a*/	/* Win3 display start point0 on panel */
#define	VOP_WIN3_COLOR_KEY			(0x011cu)			/*0x00000000*/	/* Win3 color key register */
#define	VOP_WIN3_MST1				(0x0120u)			/*0x00000000*/	/* Win3 memory start address1 */
#define	VOP_WIN3_DSP_INFO1			(0x0124u)			/*0x00ef013f*/	/* Win3 display width1/height1 on panel */
#define	VOP_WIN3_DSP_ST1			(0x0128u)			/*0x000a000a*/	/* Win3 display start point1 on panel */
#define	VOP_WIN3_SRC_ALPHA_CTRL		(0x012cu)			/*0x00000000*/	/*  */
#define	VOP_WIN3_MST2				(0x0130u)			/*0x00000000*/	/* Win3 memory start address2 */
#define	VOP_WIN3_DSP_INFO2			(0x0134u)			/*0x00ef013f*/	/* Win3 display width2/height2 on panel */
#define	VOP_WIN3_DSP_ST2			(0x0138u)			/*0x000a000a*/	/* Win3 display start point2 on panel */
#define	VOP_WIN3_DST_ALPHA_CTRL		(0x013cu)			/*0x00000000*/	/*  */
#define	VOP_WIN3_MST3				(0x0140u)			/*0x00000000*/	/* Win3 memory start address3 */
#define	VOP_WIN3_DSP_INFO3			(0x0144u)			/*0x00ef013f*/	/* Win3 display width3/height3 on panel */
#define	VOP_WIN3_DSP_ST3			(0x0148u)			/*0x000a000a*/	/* Win3 display start point3 on panel */
#define	VOP_WIN3_FADING_CTRL		(0x014cu)			/*0x00000000*/	/*  */
#define	VOP_HWC_CTRL0				(0x0150u)			/*0x00000000*/	/* Hwc ctrl register0 */
#define	VOP_HWC_CTRL1				(0x0154u)			/*0x00000000*/	/* Hwc ctrl register1 */
#define	VOP_HWC_MST					(0x0158u)			/*0x00000000*/	/* Hwc memory start address */
#define	VOP_HWC_DSP_ST				(0x015cu)			/*0x000a000a*/	/* Hwc display start point on panel */
#define	VOP_HWC_SRC_ALPHA_CTRL		(0x0160u)			/*0x00000000*/	/*  */
#define	VOP_HWC_DST_ALPHA_CTRL		(0x0164u)			/*0x00000000*/	/*  */
#define	VOP_HWC_FADING_CTRL			(0x0168u)			/*0x00000000*/	/*  */
#define	VOP_HWC_RESERVED1			(0x016cu)			/*0x00000000*/	/*  */
#define	VOP_POST_DSP_HACT_INFO		(0x0170u)			/*0x000a014a*/	/* post scaler down horizontal start and end */
#define	VOP_POST_DSP_VACT_INFO		(0x0174u)			/*0x000a00fa*/	/* Panel active horizontal scanning start point and end point */
#define	VOP_POST_SCL_FACTOR_YRGB	(0x0178u)			/*0x10001000*/	/* post yrgb scaling factor */
#define	VOP_POST_RESERVED			(0x017cu)			/*0x00001000*/	/*  */
#define	VOP_POST_SCL_CTRL			(0x0180u)			/*0x00000000*/	/* post scaling start point offset */
#define	VOP_POST_DSP_VACT_INFO_F1	(0x0184u)			/*0x000a00fa*/	/* Panel active horizontal scanning start point and end point F1 */
#define	VOP_DSP_HTOTAL_HS_END		(0x0188u)			/*0x014a000a*/	/* Panel scanning horizontal width and hsync pulse end point */
#define	VOP_DSP_HACT_ST_END			(0x018cu)			/*0x000a014a*/	/* Panel active horizontal scanning start point and end point */
#define	VOP_DSP_VTOTAL_VS_END		(0x0190u)			/*0x00fa000a*/	/* Panel scanning vertical height and vsync pulse end point */
#define	VOP_DSP_VACT_ST_END			(0x0194u)			/*0x000a00fa*/	/* Panel active vertical scanning start point and end point */
#define	VOP_DSP_VS_ST_END_F1		(0x0198u)			/*0x00000000*/	/* Vertical scanning start point and vsync pulse end point of even filed in interlace mode */
#define	VOP_DSP_VACT_ST_END_F1		(0x019cu)			/*0x00000000*/	/* Vertical scanning active start point and end point of even filed in interlace mode */
#define	VOP_PWM_CTRL				(0x01a0u)			/*0x0000200a*/	/* PWM Control Register */
#define	VOP_PWM_PERIOD_HPR			(0x01a4u)			/*0x00000000*/	/* PWM Period Register/High Polarity Capture Register */
#define	VOP_PWM_DUTY_LPR			(0x01a8u)			/*0x00000000*/	/* PWM Duty Register/Low Polarity Capture Register */
#define	VOP_PWM_CNT					(0x01acu)			/*0x00000000*/	/* PWM Counter Register */
#define	VOP_BCSH_COLOR_BAR			(0x01b0u)			/*0x00000000*/	/* color bar config register */
#define	VOP_BCSH_BCS				(0x01b4u)			/*0xd0010000*/	/* brightness contrast saturation*contrast config register */
#define	VOP_BCSH_H					(0x01b8u)			/*0x01000000*/	/* sin hue and cos hue config register */
#define	VOP_BCSH_RESERVED			(0x01bcu)			/*0x00000000*/	/*  */
#define	VOP_CABC_CTRL0				(0x01c0u)			/*0x00000000*/	/*  */
#define	VOP_CABC_CTRL1				(0x01c4u)			/*0x00000000*/	/*  */
#define	VOP_CABC_GAUSS_LINE0_0		(0x01c8u)			/*0x15110903*/	/* Register0000 Abstract */
#define	VOP_CABC_GAUSS_LINE0_1		(0x01ccu)			/*0x00030911*/	/* Register0001 Abstract */
#define	VOP_CABC_GAUSS_LINE1_0		(0x01d0u)			/*0x1a150b04*/	/* Register0002 Abstract */
#define	VOP_CABC_GAUSS_LINE1_1		(0x01d4u)			/*0x00040b15*/	/* Register0003 Abstract */
#define	VOP_CABC_GAUSS_LINE2_0		(0x01d8u)			/*0x15110903*/	/* Register0004 Abstract */
#define	VOP_CABC_GAUSS_LINE2_1		(0x01dcu)			/*0x00030911*/	/* Register0005 Abstract */
#define	VOP_FRC_LOWER01_0			(0x01e0u)			/*0x12844821*/	/*  */
#define	VOP_FRC_LOWER01_1			(0x01e4u)			/*0x21488412*/	/*  */
#define	VOP_FRC_LOWER10_0			(0x01e8u)			/*0xa55a9696*/	/*  */
#define	VOP_FRC_LOWER10_1			(0x01ecu)			/*0x5aa56969*/	/*  */
#define	VOP_FRC_LOWER11_0			(0x01f0u)			/*0xdeb77bed*/	/*  */
#define	VOP_FRC_LOWER11_1			(0x01f4u)			/*0xed7bb7de*/	/*  */
#define	VOP_FRC_RESERVED0			(0x01f8u)			/*0x00000000*/	/*  */
#define	VOP_FRC_RESERVED1			(0x01fcu)			/*0x00000000*/	/*  */
#define	VOP_MMU_DTE_ADDR			(0x0300u)			/*0x00000000*/	/* MMU current page Table address */
#define	VOP_MMU_STATUS				(0x0304u)			/*0x00000000*/	/* MMU status register */
#define	VOP_MMU_COMMAND				(0x0308u)			/*0x00000000*/	/* MMU command register */
#define	VOP_MMU_PAGE_FAULT_ADDR		(0x030cu)			/*0x00000000*/	/* MMU logical address of last page fault */
#define	VOP_MMU_ZAP_ONE_LINE		(0x0310u)			/*0x00000000*/	/* MMU Zap cache line register */
#define	VOP_MMU_INT_RAWSTAT			(0x0314u)			/*0x00000000*/	/* MMU raw interrupt status register */
#define	VOP_MMU_INT_CLEAR			(0x0318u)			/*0x00000000*/	/* MMU raw interrupt status register */
#define	VOP_MMU_INT_MASK			(0x031cu)			/*0x00000000*/	/* MMU raw interrupt status register */
#define	VOP_MMU_INT_STATUS			(0x0320u)			/*0x00000000*/	/* MMU raw interrupt status register */
#define	VOP_MMU_AUTO_GATING			(0x0324u)			/*0x00000000*/	/* mmu auto gating */
#define	VOP_WIN2_LUT_ADDR			(0x0400u)			/*0x00000000*/	/*  */
#define	VOP_WIN3_LUT_ADDR			(0x0800u)			/*0x00000000*/	/*  */
#define	VOP_HWC_LUT_ADDR			(0x0c00u)			/*0x00000000*/	/*  */
#define	VOP_GAMMA_LUT_ADDR			(0x1000u)			/*0x00000000*/	/*  */
#define	VOP_MCU_BYPASS_WPORT		(0x2200u)			/*0x00000000*/	/* Register0000 Abstract */
#define	VOP_MCU_BYPASS_RPORT		(0x2300u)			/*0x00000000*/	/* Register0001 Abstract */

static uint32_t regbase = VOPBIG_REG_BASE;

void video_init(void)
{
	ioset32(regbase+VOP_SYS_CTRL, 0x01u<<21);
	/*
	VOP_DSP_HTOTAL_HS_END
	VOP_DSP_HACT_ST_END
	VOP_DSP_VTOTAL_HS_END
	VOP_DSP_VACT_ST_END
	VOP_DSP_VS_ST_END_F1
	VOP_DSP_VACT_ST_END_F1
	*/
	iowrite32(regbase+VOP_DSP_BG, (0xff<<20)|(0x3f<<10)|(0xf));
	iowrite32(regbase+VOP_REG_CFG_DONE, 0x01); /* DONE */
}
