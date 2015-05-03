/*
 * spi.c
 *
 *  Created on: 2015/05/02
 *      Author: biwa
 */
#include "bwos.h"
#include "driver/clock.h"
#include "driver/spi.h"

#define	SPI_CTRLR0				(0x0000u)		/* Control Register 0 */
#define	SPI_CTRLR1				(0x0004u)		/* Control Register 1 */
#define	SPI_ENR					(0x0008u)		/* SPI Enable */
#define	SPI_SER					(0x000cu)		/* Slave Enable Register */
#define	SPI_BAUDR				(0x0010u)		/* Baud Rate Select */
#define	SPI_TXFTLR				(0x0014u)		/* Transmit FIFO Threshold Level */
#define	SPI_RXFTLR				(0x0018u)		/* Receive FIFO Threshold Level */
#define	SPI_TXFLR				(0x001cu)		/* Transmit FIFO Level */
#define	SPI_RXFLR				(0x0020u)		/* Receive FIFO Level */
#define	SPI_SR					(0x0024u)		/* SPI Status */
#define	SPI_IPR					(0x0028u)		/* Interrupt Polarity */
#define	SPI_IMR					(0x002cu)		/* Interrupt Mask */
#define	SPI_ISR					(0x0030u)		/* Interrupt Status */
#define	SPI_RISR				(0x0034u)		/* Raw Interrupt Status */
#define	SPI_ICR					(0x0038u)		/* Interrupt Clear */
#define	SPI_DMACR				(0x003cu)		/* DMA Control */
#define	SPI_DMATDLR				(0x0040u)		/* DMA Transmit Data Level */
#define	SPI_DMARDLR				(0x0044u)		/* DMA Receive Data Level */
#define	SPI_TXDR				(0x0400u)		/* Transmit FIFO Data  */
#define	SPI_RXDR				(0x0800u)		/* Receive FIFO Data */


/* SPI CTRLR0 */
#define	CTRL_MTM_SEQ			(0x01u<<21)		/* Microwire Transfer Mode */
#define	CTRL_OPM_MASTER			(0x00u<<20)		/* Operation Mode */
#define	CTRL_OPM_SLAVE			(0x01u<<20)		/* Operation Mode */
#define	CTRL_XFM_MASK			(0x03u<<18)		/* Transfer Mode mask */
#define	CTRL_XFM_TXRX			(0x00u<<18)		/* Transmit & Receive */
#define	CTRL_XFM_TX				(0x01u<<18)		/* Transmit Only */
#define	CTRL_XFM_RX				(0x02u<<18)		/* Receive Only */
#define	CTRL_FRF_MASK			(0x03u<<16)		/* Frame Format mask */
#define	CTRL_FRF_SPI			(0x01u<<16)		/* Motorola SPI */
#define	CTRL_FRF_SSP			(0x02u<<16)		/* Texas Instruments SSP */
#define	CTRL_FRF_MICROWIRE		(0x03u<<16)		/* National Semiconductors Microwire */
#define	CTRL_RSD_MASK			(0x03u<<14)		/* Rxd Sample Delay */
#define	CTRL_RSD_NOT			(0x00u<<14)		/* do not delay */
#define	CTRL_RSD_1				(0x01u<<14)		/* 1 cycle delay */
#define	CTRL_RSD_2				(0x02u<<14)		/* 2 cycle delay */
#define	CTRL_RSD_3				(0x03u<<14)		/* 3 cycle delay */
#define	CTRL_BHT_APB16			(0x00u<<13)		/* APB16bit SPI8bit */
#define	CTRL_BHT_APB8			(0x01u<<13)		/* APB8bit SPI8bit */
#define	CTRL_FBM_MSB			(0x00u<<12)		/* first bit is MSB */
#define	CTRL_FBM_LSB			(0x01u<<12)		/* first bit is LSB */
#define	CTRL_EM_LITTLE			(0x00u<<11)		/* little endian */
#define	CTRL_EM_BIT				(0x01u<<11)		/* big endian */
#define	CTRL_SSD_HALF			(0x00u<<10)		/* half sclk_out cycles */
#define	CTRL_SSD_ONE			(0x01u<<10)		/* one sclk_out cycles */
#define	CTRL_CSM_MASK			(0x03u<<8)		/* Chip select mode mask */
#define	CTRL_CSM_KEEP			(0x00u<<8)		/* keep after transferred */
#define	CTRL_CSM_HALF			(0x01u<<8)		/* high half cycles after transferred */
#define	CTRL_CSM_ONE			(0x02u<<8)		/* high one cycles after transferred */
#define	CTRL_SCPOL_LOW			(0x00u<<7)		/* Inactive state of serial clock is low */
#define	CTRL_SCPOL_HIGH			(0x01u<<7)		/* Inactive state of serial clock is high */
#define	CTRL_SCPH_MIDDLE		(0x00u<<6)		/* Serial clock toggles in middle of first data bit */
#define	CTRL_SCPH_FIRST			(0x01u<<6)		/* Serial clock toggles at start of first data bit */
#define	CTRL_CFS_MASK			(0x0Fu<<2)		/* Control Frame Size */
#define	CTRL_CFS_4				(0x03u<<2)		/* 4-bit serial data transfer */
#define	CTRL_CFS_5				(0x04u<<2)		/* 5-bit serial data transfer */
#define	CTRL_CFS_6				(0x05u<<2)		/* 6-bit serial data transfer */
#define	CTRL_CFS_7				(0x06u<<2)		/* 7-bit serial data transfer */
#define	CTRL_CFS_8				(0x07u<<2)		/* 8-bit serial data transfer */
#define	CTRL_CFS_9				(0x08u<<2)		/* 9-bit serial data transfer */
#define	CTRL_CFS_10				(0x09u<<2)		/* 10-bit serial data transfer */
#define	CTRL_CFS_11				(0x0Au<<2)		/* 11-bit serial data transfer */
#define	CTRL_CFS_12				(0x0Bu<<2)		/* 12-bit serial data transfer */
#define	CTRL_CFS_13				(0x0Cu<<2)		/* 13-bit serial data transfer */
#define	CTRL_CFS_14				(0x0Du<<2)		/* 14-bit serial data transfer */
#define	CTRL_CFS_15				(0x0Eu<<2)		/* 15-bit serial data transfer */
#define	CTRL_CFS_16				(0x0Fu<<2)		/* 16-bit serial data transfer */
#define	CTRL_DFS_MASK			(0x03u<<0)		/* Data Frame Size mask */
#define	CTRL_DFS_4				(0x00u<<0)		/* 4bit data */
#define	CTRL_DFS_8				(0x01u<<0)		/* 8bit data */
#define	CTRL_DFS_16				(0x02u<<0)		/* 16bit data */


#define	ENR_DISABLE				(0x00u<<0)		/* SPI disable */
#define	ENR_ENABLE				(0x01u<<0)		/* SPI enable */

#define	SER_ENABLE0				(0x01u<<0)		/* Slave Select Enable 0 */
#define	SER_ENABLE1				(0x01u<<1)		/* Slave Select Enable 1 */

#define	FIFO_THRESHOLD_MASK		(0x1Fu)			/* FIFO Threshold mask */
#define	FIFO_LEVEL_MASK			(0x3Fu)			/* FIFO Level mask */

/* SPI SR */
#define	SR_RFF					(0x01u<<4)		/* Receive FIFO Full */
#define	SR_RFE					(0x01u<<3)		/* Receive FIFO Empty */
#define	SR_TFE					(0x01u<<2)		/* Transmit FIFO Empty */
#define	SR_TFF					(0x01u<<1)		/* Transmit FIFO Full */
#define	SR_BSF					(0x01u<<0)		/* SPI Busy Flag */

/* Interrupt */
#define	IPR_HIGH				(0x00u<<0)		/* Active Interrupt Polarity Level is HIGH */
#define	IPR_LOW					(0x01u<<0)		/* Active Interrupt Polarity Level is LOW */
/* Interrupt mask */
#define	IMR_RFFIM				(0x01u<<4)		/* Receive FIFO Full Interrupt Mask */
#define	IMR_RFOIM				(0x01u<<3)		/* Receive FIFO Overflow Interrupt Mask */
#define	IMR_RFUIM				(0x01u<<2)		/* Receive FIFO Underflow Interrupt Mask */
#define	IMR_TFOIM				(0x01u<<1)		/* Transmit FIFO Overflow Interrupt Mask */
#define	IMR_TFEIM				(0x01u<<0)		/* Transmit FIFO Empty Interrupt Mask */
/* Interrupt status */
#define	ISR_RFFIS				(0x01u<<4)		/* Receive FIFO Full Interrupt Status */
#define	ISR_RFOIS				(0x01u<<3)		/* Receive FIFO Overflow Interrupt Status */
#define	ISR_RFUIS				(0x01u<<2)		/* Receive FIFO Underflow Interrupt Status */
#define	ISR_TFOIS				(0x01u<<1)		/* Transmit FIFO Overflow Interrupt Status */
#define	ISR_TFEIS				(0x01u<<0)		/* Transmit FIFO Empty Interrupt Status */
/* Interrupt raw status */
#define	RISR_RFFRIS				(0x01u<<4)		/* Receive FIFO Full Raw Interrupt Status */
#define	RISR_RFORIS				(0x01u<<3)		/* Receive FIFO Overflow Raw Interrupt Status */
#define	RISR_RFURIS				(0x01u<<2)		/* Receive FIFO Underflow Raw Interrupt Status */
#define	RISR_TFORIS				(0x01u<<1)		/* Transmit FIFO Overflow Raw Interrupt Status */
#define	RISR_TFERIS				(0x01u<<0)		/* Transmit FIFO Empty Raw Interrupt Status */
/* Interrupt clear */
#define	ICR_CTFOI				(0x01u<<3)		/* Clear Transmit FIFO Overflow Interrupt */
#define	ICR_CRFOI				(0x01u<<2)		/* Clear Receive FIFO Overflow Interrupt */
#define	ICR_CRFUI				(0x01u<<1)		/* Clear Receive FIFO Underflow Interrupt */
#define	ICR_CCI					(0x01u<<0)		/* Clear Combined Interrupt */

/* DMA */
#define	DMACR_TDE_ENABLE		(0x01u<<1)		/* Transmit DMA Enable */
#define	DMACR_RDE_ENABLE		(0x01u<<0)		/* Receive DMA Enable */
