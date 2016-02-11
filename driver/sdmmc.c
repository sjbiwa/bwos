/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * sdmmc.c
 *
 *  Created on: 2015/02/01
 *      Author: biwa
 */
/*
 * Firefly RK3288 SD/MMC driver
 */
#include "bwos.h"
#include "driver/sdmmc.h"
#include "driver/clock.h"

#define	SDMMC_CTRL					0x0				/* Control Register */
#define	SDMMC_PWREN					0x4				/* Power Enable Register */
#define	SDMMC_CLKDIV				0x8				/* Clock Divider Register */
#define	SDMMC_CLKSRC				0xC				/* SD Clock Source Register */
#define	SDMMC_CLKENA				0x10			/* Clock Enable Register */
#define	SDMMC_TMOUT					0x14			/* Timeout Register */
#define	SDMMC_CTYPE					0x18			/* Card Type Register */
#define	SDMMC_BLKSIZ				0x1C			/* Block Size Register */
#define	SDMMC_BYTCNT				0x20			/* Byte Count Register */
#define	SDMMC_INTMASK				0x24			/* Interrupt Mask Register */
#define	SDMMC_CMDARG				0x28			/* Command Argument Register */
#define	SDMMC_CMD					0x2C			/* Command Register */
#define	SDMMC_RESP0					0x30			/* Response Register 0 */
#define	SDMMC_RESP1					0x34			/* Response Register 1 */
#define	SDMMC_RESP2					0x38			/* Response Register 2 */
#define	SDMMC_RESP3					0x3C			/* Response Register 3 */
#define	SDMMC_MINTSTS				0x40			/* Masked Interrupt Status Register */
#define	SDMMC_RINTSTS				0x44			/* Raw Interrupt Status Register */
#define	SDMMC_STATUS				0x48			/* Status Register */
#define	SDMMC_FIFOTH				0x4C			/* FIFO Threshold Watermark Register */
#define	SDMMC_CDETECT				0x50			/* Card Detect Register */
#define	SDMMC_WRTPRT				0x54			/* Write Protect Register */
#define	SDMMC_TCBCNT				0x5C			/* Transferred CIU Card Byte Count Register */
#define	SDMMC_TBBCNT				0x60			/* Transferred Host to BIU-FIFO Byte Count Register */
#define	SDMMC_DEBNCE				0x64			/* Debounce Count Register */
#define	SDMMC_USRID					0x68			/* User ID Register */
#define	SDMMC_VERID					0x6C			/* Version ID Register */
#define	SDMMC_HCON					0x70			/* Hardware Configuration Register */
#define	SDMMC_UHS_REG				0x74			/* UHS-1 Register */
#define	SDMMC_RST_N					0x78			/* Hardware Reset Register */
#define	SDMMC_BMOD					0x80			/* Bus Mode Register */
#define	SDMMC_PLDMND				0x84			/* Poll Demand Register */
#define	SDMMC_DBADDR				0x88			/* Descriptor List Base Address Register */
#define	SDMMC_IDSTS					0x8C			/* Internal DMAC Status Register */
#define	SDMMC_IDINTEN				0x90			/* Internal DMAC Interrupt Enable Register */
#define	SDMMC_DSCADDR				0x94			/* Current Host Descriptor Address Register */
#define	SDMMC_BUFADDR				0x98			/* Current Buffer Descriptor Address Register */
#define	SDMMC_CARDTHRCTL			0x100			/* Card Threshold Control Register */
#define	SDMMC_BACK_END_POWER_R		0x104			/* Back End Power Register */
#define	SDMMC_DATA					0x200			/* Data FIFO Access */

/* SDMMC_CTRL bit */
#define	SDMMC_CTRL_USE_INTERNAL_DMAC						(1u<<25)
#define	SDMMC_CTRL_CEATA_DEVICE_INTERRUPT_STATUS			(1u<<11)
#define	SDMMC_CTRL_SEND_AUTO_STOP_CCSD						(1u<<10)
#define	SDMMC_CTRL_SEND_CCSD								(1u<<9)
#define	SDMMC_CTRL_ABORT_READ_DATA							(1u<<8)
#define	SDMMC_CTRL_SEND_IRQ_RESPONSE						(1u<<7)
#define	SDMMC_CTRL_READ_WAIT								(1u<<6)
#define	SDMMC_CTRL_INT_ENABLE								(1u<<4)
#define	SDMMC_CTRL_DMA_RESET								(1u<<2)
#define	SDMMC_CTRL_FIFO_RESET								(1u<<1)
#define	SDMMC_CTRL_CONTROLLER_RESET							(1u<<0)

/* SDMMC_PWREN bit */
#define	SDMMC_PWREN_POWER_ENABLE							(1u<<0)

/* SDMMC_CLKDIV bit */
#define	SDMMC_CLKDIV_CLK_DIVIDER0(n)					((n) & 0xFF)

/* SDMMC_CLKSRC bit */
#define	SDMMC_CLKSRC_CLK_SOURCE(n)						((n) & 0x03)

/* SDMMC_CLKENA bit */
#define	SDMMC_CLKENA_CCLK_LOW_POWER						(0x01u<<16)
#define	SDMMC_CLKENA_CCLK_ENABLE						(0x01u<<0)

/* SDMMC_TMOUT bit */
#define	SDMMC_TMOUT_DATA_TIMEOUT(n)						((n) << 8)
#define	SDMMC_TMOUT_RESPONSE_TIMEOUT(n)					((n) & 0xFF)

/* SDMMC_CTYPE bit */
#define	SDMMC_CTYPE_CARD_WIDTH1							(0x1u<<16)
#define	SDMMC_CTYPE_CARD_WIDTH2							(0x1u<<0)

/* SDMMC_BLKSIZ bit */
#define	SDMMC_BLKSIZ_BLOCK_SIZE(n)						((n) & 0xFFFF)

/* SDMMC_BYTCNT bit */

/* SDMMC_INTMASK bit */
#define	SDMMC_INTMASK_SDIO_INT_MASK						(0x01u<<16)
#define	SDMMC_INTMASK_EBE								(0x01u<<15)
#define	SDMMC_INTMASK_ACD								(0x01u<<14)
#define	SDMMC_INTMASK_SBE								(0x01u<<13)
#define	SDMMC_INTMASK_HLE								(0x01u<<12)
#define	SDMMC_INTMASK_FRUN								(0x01u<<11)
#define	SDMMC_INTMASK_HTO								(0x01u<<10)
#define	SDMMC_INTMASK_DRT								(0x01u<<9)
#define	SDMMC_INTMASK_RTO								(0x01u<<8)
#define	SDMMC_INTMASK_DCRC								(0x01u<<7)
#define	SDMMC_INTMASK_RCRC								(0x01u<<6)
#define	SDMMC_INTMASK_RXDR								(0x01u<<5)
#define	SDMMC_INTMASK_TXDR								(0x01u<<4)
#define	SDMMC_INTMASK_DTO								(0x01u<<3)
#define	SDMMC_INTMASK_CMD								(0x01u<<2)
#define	SDMMC_INTMASK_RE								(0x01u<<1)
#define	SDMMC_INTMASK_CD								(0x01u<<0)

/* SDMMC_CMDARG bit */

/* SDMMC_CMD bit */
#define	SDMMC_CMD_START_CMD								(0x1u<<31)
#define	SDMMC_CMD_USE_HOLD_REG							(0x1u<<29)
#define	SDMMC_CMD_VOLT_SWITCH							(0x1u<<28)
#define	SDMMC_CMD_BOOT_MODE								(0x1u<<27)
#define	SDMMC_CMD_DISABLE_BOOT							(0x1u<<26)
#define	SDMMC_CMD_EXPECT_BOOT_ACK						(0x1u<<25)
#define	SDMMC_CMD_ENABLE_BOOT							(0x1u<<24)
#define	SDMMC_CMD_CCS_EXPECTED							(0x1u<<23)
#define	SDMMC_CMD_READ_CEATA_DEVICE						(0x1u<<22)
#define	SDMMC_CMD_UPDATE_CLOCK_REGISTERS_ONLY			(0x1u<<21)
#define	SDMMC_CMD_CARD_NUMBER(n)						(((n) & 1F)<<16)
#define	SDMMC_CMD_SEND_INITIALIZATION					(0x1u<<15)
#define	SDMMC_CMD_STOP_ABORT_CMD						(0x1u<<14)
#define	SDMMC_CMD_WAIT_PRVDATA_COMPLETE					(0x1u<<13)
#define	SDMMC_CMD_SEND_AUTO_STOP						(0x1u<<12)
#define	SDMMC_CMD_TRANSFER_MODE							(0x1u<<11)
#define	SDMMC_CMD_READ_WRITE							(0x1u<<10)
#define	SDMMC_CMD_DATA_EXPECTED							(0x1u<<9)
#define	SDMMC_CMD_CHECK_RESPONSE_CRC					(0x1u<<8)
#define	SDMMC_CMD_RESPONSE_LENGTH						(0x1u<<7)
#define	SDMMC_CMD_RESPONSE_EXPECT						(0x1u<<6)
#define	SDMMC_CMD_CMD_INDEX(n)							((n) & 0x3F)

/* SDMMC_RESP0 bit */
/* SDMMC_RESP1 bit */
/* SDMMC_RESP2 bit */
/* SDMMC_RESP3 bit */
/* SDMMC_MINTSTS bit (use SDMMC_INTMASK bit) */
/* SDMMC_RINTSTS bit (use SDMMC_INTMASK bit) */

/* SDMMC_STATUS bit */
#define	SDMMC_STATUS_FIFO_COUNT							((n)>>17)&0x1FFF)
#define	SDMMC_STATUS_RESPONSE_INDEX(n)					(((n)>>11)&0x3F)
#define	SDMMC_STATUS_DATA_STATE_MC_BUSY					(0x1u<<10)
#define	SDMMC_STATUS_DATA_BUSY							(0x1u<<9)
#define	SDMMC_STATUS_DATA_3_STATUS						(0x1u<<8)
#define	SDMMC_STATUS_COMMAND_FSM_STATES(n)				(((n)>>4)&0xF)
#define	SDMMC_STATUS_FIFO_FULL							(0x1u<<3)
#define	SDMMC_STATUS_FIFO_EMPTY							(0x1u<<2)
#define	SDMMC_STATUS_FIFO_TX_WATERMARK					(0x1u<<1)
#define	SDMMC_STATUS_FIFO_RX_WATERMARK					(0x1u<<0)
/* COMMAND_FSM_STATES */
#define	FSM_IDLE					0x0		 /* Idle, Wait for CCS, Send CCSD, or Boot Mode */
#define	FSM_SEND_INIT				0x1		 /* Send init sequence */
#define	FSM_TX_CMD_START			0x2		 /* Tx cmd start bit */
#define	FSM_TX_CMD_TX				0x3		 /* Tx cmd tx bit */
#define	FSM_TX_CMD_INDEX			0x4		 /* Tx cmd index + arg */
#define	FSM_TX_CMD_CRC7				0x5		 /* Tx cmd crc7 */
#define	FSM_TX_CMD_END				0x6		 /* Tx cmd end bit */
#define	FSM_RX_RESP_START			0x7		 /* Rx resp start bit */
#define	FSM_RX_RESP_IRQ				0x8		 /* Rx resp IRQ response */
#define	FSM_RX_RESP_TX				0x9		 /* Rx resp tx bit */
#define	FSM_RX_RESP_CMD_IDX			0xa		 /* Rx resp cmd idx */
#define	FSM_RX_RESP_DATA			0xb		 /* Rx resp data */
#define	FSM_RX_RESP_CRC7			0xc		 /* Rx resp crc7 */
#define	FSM_RX_RESP_END				0xd		 /* Rx resp end bit */
#define	FSM_CMD_PATH				0xe		 /* Cmd path wait NCC */
#define	FSM_WAIT					0xf		 /* Wait: CMD-to-reponse turnaround */

/* SDMMC_FIFOTH bit */
#define	SDMMC_FIFOTH_DW_DMA_MULTIPLE_TRANSACTION_SIZE(n)	(((n) & 0x7u) << 28)
#define	SDMMC_FIFOTH_RX_WMARK(n)							(((n) & 0xfffu) << 16)
#define	SDMMC_FIFOTH_TX_WMARK(n)							((n) & 0xfffu)

/* SDMMC_CDETECT bit */
#define	SDMMC_CDETECT_CARD_DETECT_N						(0x1u<<0)

/* SDMMC_WRTPRT bit */
#define	SDMMC_WRTPRT_WRITE_PROTECT						(0x1u<<0)

/* SDMMC_TCBCNT bit */
/* SDMMC_TBBCNT bit */

/* SDMMC_DEBNCE bit */
#define	SDMMC_DEBNCE_DEBOUNCE_COUNT(n)					((n) & 0xffffffu)

/* SDMMC_USRID bit */
/* SDMMC_VERID bit */

/* SDMMC_HCON bit */
#define	SDMMC_HCON_ARO									(0x1u<<26)
#define	SDMMC_HCON_NCD(n)								((n)>>24) & 0x3u)
#define	SDMMC_HCON_SCFP									(0x1u<<23)
#define	SDMMC_HCON_IHR									(0x1u<<22)
#define	SDMMC_HCON_RIOS									(0x1u<<21)
#define	SDMMC_HCON_DMADATAWIDTH(n)						((n)>>18) & 0x7u)
#define	SDMMC_HCON_DMAINTF(n)							((n)>>16) & 0x3u)
#define	SDMMC_HCON_HADDRWIDTH(n)						((n)>>10) & 0x3fu)
#define	SDMMC_HCON_HDATAWIDTH(n)						(((n)>>7) & 0x7u)
#define	SDMMC_HCON_HBUS									(0x1u<<6)
#define	SDMMC_HCON_NC(n)								((n)>>1) & 0x1fu)
#define	SDMMC_HCON_CT									(0x1u<<0)

/* SDMMC_UHS_REG bit */
#define	SDMMC_UHS_REG_DDR_REG							(0x1u<<16)
#define	SDMMC_UHS_REG_VOLT_REG							(0x1u<<0)

/* SDMMC_RST_N bit */
#define	SDMMC_RST_N_CARD_RESET							(0x1u<<0)

/* SDMMC_BMOD bit */
#define	SDMMC_BMOD_PBL(n)								(((n)>>8)&0x7)
#define	SDMMC_BMOD_DE									(0x1u<<7)
#define	SDMMC_BMOD_DSL(n)								(((n)&0x1fu)<<2)
#define	SDMMC_BMOD_FB									(0x1u<<1)
#define	SDMMC_BMOD_SWR									(0x1u<<0)

/* SDMMC_PLDMND bit */

/* SDMMC_DBADDR bit */
#define	SDMMC_DBADDR_SDL(n)								((n)&0xfffffffcu)

/* SDMMC_IDSTS bit */
#define	SDMMC_IDSTS_FSM(n)								(((n)>>13)&0xf)
#define	SDMMC_IDSTS_EB(n)								(((n)>>10)&0x7)
#define	SDMMC_IDSTS_AIS									(0x1u<<9)
#define	SDMMC_IDSTS_NIS									(0x1u<<8)
#define	SDMMC_IDSTS_CES									(0x1u<<5)
#define	SDMMC_IDSTS_DU									(0x1u<<4)
#define	SDMMC_IDSTS_FBE									(0x1u<<2)
#define	SDMMC_IDSTS_RI									(0x1u<<1)
#define	SDMMC_IDSTS_TI									(0x1u<<0)

/* SDMMC_IDINTEN bit */
#define	SDMMC_IDINTEN_AI								(0x1u<<9)
#define	SDMMC_IDINTEN_NI								(0x1u<<8)
#define	SDMMC_IDINTEN_CES								(0x1u<<5)
#define	SDMMC_IDINTEN_DU								(0x1u<<4)
#define	SDMMC_IDINTEN_FBE								(0x1u<<2)
#define	SDMMC_IDINTEN_RI								(0x1u<<1)
#define	SDMMC_IDINTEN_TI								(0x1u<<0)

/* SDMMC_DSCADDR bit */
/* SDMMC_BUFADDR bit */

/* SDMMC_CARDTHRCTL bit */
#define	SDMMC_CARDTHRCTL_CARDRDTHRESHOLD(n)				((n)&0xFFFu)<<16)
#define	SDMMC_CARDTHRCTL_CARDRDTHREN					(0x1u<<0)

/* SDMMC_BACK_END_POWER bit */
/* SDMMC_DATA bit */

typedef	struct {
	SdmmcDeviceInfo*	dev;					/* デバイス情報 */
	bool				active;					/* デバイス有効フラグ */
	int					mutex;					/* チャネル毎の排他用 */
	int					ev_flag;				/* 転送完了待ち用フラグ */
	TimeOut				tmout;					/* タイムアウト指定(transfer呼び出し時に設定) */
} SdmmcObject;


static SdmmcObject*		sdmmc_obj_tbl;
static uint32_t			sdmmc_obj_num;

static void sdmmc_irq_handler(uint32_t irqno, void* info)
{
}

void sdmmc_register(SdmmcDeviceInfo* info, uint32_t info_num)
{
	int ix;
	sdmmc_obj_num = info_num;

	sdmmc_obj_tbl = sys_malloc(sizeof(SdmmcObject) * info_num);
	for ( ix=0; ix < info_num; ix++ ) {
		sdmmc_obj_tbl[ix].dev = &info[ix];
		sdmmc_obj_tbl[ix].active = false;
		sdmmc_obj_tbl[ix].mutex = mutex_create();
		sdmmc_obj_tbl[ix].ev_flag = flag_create();

		irq_set_enable(sdmmc_obj_tbl[ix].dev->irq, IRQ_DISABLE, CPU_SELF);
		irq_add_handler(sdmmc_obj_tbl[ix].dev->irq, sdmmc_irq_handler, &sdmmc_obj_tbl[ix]);
	}
}

void sdmmc_reset(void)
{
	uint32_t rbase = sdmmc_obj_tbl[0].dev->io_addr;
	uint32_t reset_param = SDMMC_CTRL_DMA_RESET|SDMMC_CTRL_FIFO_RESET|SDMMC_CTRL_CONTROLLER_RESET;
	iowrite32(rbase+SDMMC_CTRL, reset_param);
	while (	ioread32(rbase+SDMMC_CTRL) & reset_param ) {
		task_tsleep(MSEC(100));
	}
	lprintf("SD/MMC reset end\n");
}

void sdmmc_check(void)
{
	uint32_t rbase = sdmmc_obj_tbl[0].dev->io_addr;
	lprintf(" STAT:%08X\n", ioread32(rbase+SDMMC_STATUS));
	lprintf(" DTCT:%08X\n", ioread32(rbase+SDMMC_CDETECT));
	lprintf(" PROT:%08X\n", ioread32(rbase+SDMMC_WRTPRT));
}
