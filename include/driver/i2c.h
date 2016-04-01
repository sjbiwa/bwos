/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * i2c.h
 *
 *  Created on: 2015/05/19
 *      Author: biwa
 */

#ifndef I2C_H_
#define I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef	struct {
	uint32_t		io_addr;				/* ベースアドレス */
	uint32_t		irq;					/* IRQ番号 */
	uint32_t		clock_src;				/* クロック識別子 */
} I2cDeviceInfo;

typedef	struct {
	uint32_t		baudrate;				/* ボーレート */
} I2cPortConfig;

typedef	enum {
	I2C_7BIT = 0,
	I2C_10BIT,
} I2cAddressBits;

typedef	enum {
	I2C_TX_MODE = 0,
	I2C_RX_MODE,
} I2cTransferMode;

typedef	struct {
	uint32_t		addr;					/* スレーブアドレス */
	I2cAddressBits	bits;					/* アドレスビット */
	I2cTransferMode	mode;					/* Read/Write モード */
	uint8_t*		buff;					/* 転送バッファ */
	uint32_t		length;					/* 転送サイズ */
} I2cTransferMethod;

typedef	struct {
	uint32_t			method_num;
	I2cTransferMethod*	method;
} I2cTransferParam;

extern void i2c_register(I2cDeviceInfo* info, uint32_t info_num);
extern int i2c_set_port_config(uint32_t port_no, I2cPortConfig* config);
extern int i2c_transfer(uint32_t port_no, I2cTransferParam* param, TimeOut tmout);

#ifdef __cplusplus
}
#endif

#endif /* I2C_H_ */
