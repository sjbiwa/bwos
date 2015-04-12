/*
 * gpioregs.h
 *
 *  Created on: 2015/04/11
 *      Author: biwa
 */

#ifndef _GPIOREGS_H_
#define _GPIOREGS_H_

#define	GPIO_SWPORTA_DR 	(0x0000u) 	/* Port A data register */
#define	GPIO_SWPORTA_DDR 	(0x0004u) 	/* Port A data direction register */
#define	GPIO_INTEN 			(0x0030u) 	/* Interrupt enable register */
#define	GPIO_INTMASK 		(0x0034u) 	/* Interrupt mask register */
#define	GPIO_INTTYPE_LEVEL 	(0x0038u) 	/* Interrupt level register */
#define	GPIO_INT_POLARITY 	(0x003cu) 	/* Interrupt polarity register */
#define	GPIO_INT_STATUS 	(0x0040u) 	/* Interrupt status of port A */
#define	GPIO_INT_RAWSTATUS 	(0x0044u) 	/* Raw Interrupt status of port A */
#define	GPIO_DEBOUNCE 		(0x0048u) 	/* Debounce enable register */
#define	GPIO_PORTA_EOI 		(0x004cu) 	/* Port A clear interrupt register */
#define	GPIO_EXT_PORTA 		(0x0050u) 	/* Port A external port register */
#define	GPIO_LS_SYNC 		(0x0060u) 	/* Level_sensitive synchronization enable register */

#endif /* ARCH_ARM_BOARD_FIREFLY_RK3288_GPIOREGS_H_ */
