/*
 * Copyright (C) 2016 sjbiwa All rights reserved.
 */

/*
 * arm_std_md.c
 *
 *  Created on: 2016/03/19
 *      Author: biwa
 */
#include "bwos.h"

#define	SECTOR_SIZE		(512)

void sdmmc_init(uint32_t port)
{
}

void sdmmc_read_sector(uint32_t port, uint32_t sect_num, void* buff)
{
	memcpy(buff, (void*)(SDMMC_MAP_BASE + sect_num * 512), SECTOR_SIZE);
}

void sdmmc_write_sector(uint32_t port, uint32_t sect_num, void* buff)
{
	memcpy((void*)(SDMMC_MAP_BASE + sect_num * 512), buff, SECTOR_SIZE);
}

uint32_t sdmmc_get_sector_num(uint32_t port)
{
	return (uint32_t)(SDMMC_MAP_SIZE / SECTOR_SIZE);
}
