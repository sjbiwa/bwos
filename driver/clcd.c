/*
 * clcd.c
 *
 *  Created on: 2016/03/18
 *      Author: biwa
 */
#include "bwos.h"
#include "driver/clock.h"
#include "driver/clcd.h"

#define	LCDTiming0			(0x000u)	/* Horizontal Axis Panel Control Register on page 3-5 */
#define	LCDTiming1			(0x004u)	/* Vertical Axis Panel Control Register on page 3-6 */
#define	LCDTiming2			(0x008u)	/* Clock and Signal Polarity Control Register on page 3-8 */
#define	LCDTiming3			(0x00Cu)	/* Line End Control Register on page 3-10 */
#define	LCDUPBASE			(0x010u)	/* Upper and Lower Panel Frame Base Address Registers on page 3-10 */
#define	LCDLPBASE			(0x014u)	/* Upper and Lower Panel Frame Base Address Registers on page 3-10 */
#define	LCDControl			(0x018u)	/* LCD Control Register on page 3-12 */
#define	LCDIMSC				(0x01Cu)	/* Interrupt Mask Set/Clear Register on page 3-14 */
#define	LCDRIS				(0x020u)	/* Raw Interrupt Status Register on page 3-15 */
#define	LCDMIS				(0x024u)	/* Masked Interrupt Status Register on page 3-16 */
#define	LCDICR				(0x028u)	/* LCD Interrupt Clear Register on page 3-17 */
#define	LCDUPCURR			(0x02Cu)	/* LCD Upper and Lower Panel Current Address Value Registers on page 3-18 */
#define	LCDLPCURR			(0x030u)	/* LCD Upper and Lower Panel Current Address Value Registers on page 3-18 */
#define	LCDPalette			(0x200u)	/* 256x16-bit Color Palette Registers on page 3-19 */
#define	CursorImage			(0x800u)	/* Cursor Image RAM Register on page 3-20 */
#define	ClcdCrsrCtrl		(0xC00u)	/* Cursor Control Register on page 3-20 */
#define	ClcdCrsrConfig		(0xC04u)	/* Cursor Configuration Register on page 3-21 */
#define	ClcdCrsrPalette0	(0xC08u)	/* Cursor Palette Registers on page 3-22 */
#define	ClcdCrsrPalette1	(0xC0Cu)	/* Cursor Palette Registers on page 3-22 */
#define	ClcdCrsrXY			(0xC10u)	/* Cursor XY Position Register on page 3-23 */
#define	ClcdCrsrClip		(0xC14u)	/* Cursor Clip Position Register on page 3-24 */
#define	ClcdCrsrIMSC		(0xC20u)	/* Cursor Interrupt Mask Set/Clear Register on page 3-25 */
#define	ClcdCrsrICR			(0xC24u)	/* Cursor Interrupt Clear Register on page 3-25 */
#define	ClcdCrsrRIS			(0xC28u)	/* Cursor Raw Interrupt Status Register on page 3-26 */
#define	ClcdCrsrMIS			(0xC2Cu)	/* Cursor Masked Interrupt Status Register on page 3-26 */
#define	CLCDPeriphID0		(0xFE0u)	/* Peripheral Identification Register 0 on page 3-28 */
#define	CLCDPeriphID1		(0xFE4u)	/* Peripheral Identification Register 1 on page 3-28 */
#define	CLCDPeriphID2		(0xFE8u)	/* Peripheral Identification Register 2 on page 3-29 */
#define	CLCDPeriphID3		(0xFECu)	/* Peripheral Identification Register 3 on page 3-30 */
#define	CLCDPCellID0		(0xFF0u)	/* PrimeCell Identification Register 0 on page 3-31 */
#define	CLCDPCellID1		(0xFF4u)	/* PrimeCell Identification Register 1 on page 3-31 */
#define	CLCDPCellID2		(0xFF8u)	/* PrimeCell Identification Register 2 on page 3-32 */
#define	CLCDPCellID3		(0xFFCu)	/* PrimeCell Identification Register 3 on page 3-32 */

static ClcdDeviceInfo* clcd_info;

static void clcd_irq_handler(uint32_t irqno, void* irq_info)
{

}

void clcd_register(ClcdDeviceInfo* info, uint32_t info_num)
{
	clcd_info = info;
	uint32_t rbase = info->io_addr;
#if 0
	memset((void*)FRAME_BUFFER_BASE, 0, 640*480*4);
	iowrite32(rbase+LCDTiming0, (39u<<24) | (23u<<16) | (95u<<8) | (39u<<2));
	iowrite32(rbase+LCDTiming1, (32u<<24) | (11u<<16) | (2u<<10) | (480u<<0));
	iowrite32(rbase+LCDTiming2, (639u<<16));
#else
	memset((void*)FRAME_BUFFER_BASE, 0, 1024*768*4);
	iowrite32(rbase+LCDTiming0, (39u<<24) | (23u<<16) | (95u<<8) | (63u<<2));
	iowrite32(rbase+LCDTiming1, (32u<<24) | (11u<<16) | (2u<<10) | (768u<<0));
	iowrite32(rbase+LCDTiming2, (1023u<<16));
#endif
	iowrite32(rbase+LCDTiming3, (1u<<16));
	iowrite32(rbase+LCDUPBASE, FRAME_BUFFER_BASE);
	iowrite32(rbase+LCDLPBASE, FRAME_BUFFER_BASE);
	iowrite32(rbase+LCDControl, (1u<<5) | (5u<<1) | 1u);

	irq_set_enable(clcd_info->irq, IRQ_DISABLE, CPU_SELF);
	irq_add_handler(clcd_info->irq, clcd_irq_handler, clcd_info);
}
