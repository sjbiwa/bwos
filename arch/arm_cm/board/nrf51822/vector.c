/*
 * vector.c
 *
 *  Created on: 2015/07/03
 *      Author: biwa
 */
#include "common.h"

#define	VECT_NO(n)			((n)+16)

typedef	void (*HandlerEntry)(void);

extern	void _reset_entry(void);
extern	void _nmi_entry(void);
extern	void _hard_fault_entry(void);
extern	void _svc_entry(void);
extern	void _debug_entry(void);
extern	void _pendsvc_entry(void);
extern	void _rtc0_entry(void);

extern	uint8_t			stack_master;

const HandlerEntry handler_entry[IRQ_NUM]  __attribute__ ((section (".vector"))) = {
	[0]									= (HandlerEntry)(&stack_master),
	[VECT_NO(Reset_IRQn)]				= _reset_entry,
	[VECT_NO(NonMaskableInt_IRQn)]		= _nmi_entry,
	[VECT_NO(HardFault_IRQn)]			= _hard_fault_entry,
	[VECT_NO(SVCall_IRQn)]				= _svc_entry,
	[VECT_NO(DebugMonitor_IRQn)]		= _debug_entry,
	[VECT_NO(PendSV_IRQn)]				= _pendsvc_entry,
	[VECT_NO(RTC0_IRQn)]				= _rtc0_entry,
};

