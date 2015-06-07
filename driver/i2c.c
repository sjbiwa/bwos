/*
 * i2c.c
 *
 *  Created on: 2015/05/19
 *      Author: biwa
 */
#include "bwos.h"
#include "driver/i2c.h"
#include "driver/clock.h"

#define	I2C_CON					(0x0000u)		/* control register */
#define	I2C_CLKDIV				(0x0004u)		/* Clock divisor register */
#define	I2C_MRXADDR				(0x0008u)		/* the slave address accessed for master receive mode */
#define	I2C_MRXRADDR			(0x000cu)		/* the slave register address accessed for master recevie mode */
#define	I2C_MTXCNT				(0x0010u)		/* master transmit count */
#define	I2C_MRXCNT				(0x0014u)		/* master receive count */
#define	I2C_IEN					(0x0018u)		/* interrupt enable register */
#define	I2C_IPD					(0x001cu)		/* interrupt pending register */
#define	I2C_FCNT				(0x0020u)		/* finished count */
#define	I2C_TXDATA0				(0x0100u)		/* I2C transmit data register 0 */
#define	I2C_TXDATA1				(0x0104u)		/* I2C transmit data register 1 */
#define	I2C_TXDATA2				(0x0108u)		/* I2C transmit data register 2 */
#define	I2C_TXDATA3				(0x010cu)		/* I2C transmit data register 3 */
#define	I2C_TXDATA4				(0x0110u)		/* I2C transmit data register 4 */
#define	I2C_TXDATA5				(0x0114u)		/* I2C transmit data register 5 */
#define	I2C_TXDATA6				(0x0118u)		/* I2C transmit data register 6 */
#define	I2C_TXDATA7				(0x011cu)		/* I2C transmit data register 7 */
#define	I2C_RXDATA0				(0x0200u)		/* I2C receive data register 0 */
#define	I2C_RXDATA1				(0x0204u)		/* I2C receive data register 1 */
#define	I2C_RXDATA2				(0x0208u)		/* I2C receive data register 2 */
#define	I2C_RXDATA3				(0x020cu)		/* I2C receive data register 3 */
#define	I2C_RXDATA4				(0x0210u)		/* I2C receive data register 4 */
#define	I2C_RXDATA5				(0x0214u)		/* I2C receive data register 5 */
#define	I2C_RXDATA6				(0x0218u)		/* I2C receive data register 6 */
#define	I2C_RXDATA7				(0x021cu)		/* I2C receive data register 7 */


/* I2C_CON */
#define	CON_ACT2NAK_IGNORED		(0x00u<<6)
#define	CON_ACT2NAK_STOP		(0x01u<<6)
#define	CON_ACK_ACK				(0x00u<<5)
#define	CON_ACK_NAK				(0x01u<<5)
#define	CON_STOP_ENABLE			(0x01u<<4)
#define	CON_START_ENABLE		(0x01u<<3)
#define	CON_I2C_MODE_MASK		(0x03u<<1)
#define	CON_I2C_MODE_TXONLY		(0x00u<<1)
#define	CON_I2C_MODE_TX_ADDR	(0x01u<<1)
#define	CON_I2C_MODE_RX_ONLY	(0x10u<<1)
#define	CON_I2C_MODE_RX_ONLY	(0x10u<<1)
