/*
 * Copyright (C) 2016 sjbiwa All rights reserved.
 */

/*
 * sdmmc.h
 *
 *  Created on: 2016/02/11
 *      Author: biwa
 */

#ifndef _SDMMC_H_
#define SDMMC_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef	struct {
	uint32_t		io_addr;				/* ベースアドレス */
	uint32_t		irq;					/* IRQ番号 */
	uint32_t		clock_src;				/* クロック識別子 */
} SdmmcDeviceInfo;

extern void sdmmc_register(SdmmcDeviceInfo* info, uint32_t info_num);

#ifdef __cplusplus
}
#endif

#endif /* _SDMMC_H_ */
