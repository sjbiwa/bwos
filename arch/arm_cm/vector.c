/*
 * vector.c
 *
 *  Created on: 2015/07/03
 *      Author: biwa
 */
#include "common.h"

#define	RESET_STACK_NO				(0)
#define	RESET_ENTRY_NO				(1)
#define	NMI_ENTRY_NO				(2)
#define	HARD_FAULT_ENTRY_NO			(3)
#define	MEM_MANAGE_ENTRY_NO			(4)
#define	BUS_FAULT_ENTRY_NO			(5)
#define	USAGE_FAULT_ENTRY_NO		(6)
#define	SVC_ENTRY_NO				(11)
#define	DEBUG_ENTRY_NO				(12)
#define	PENDSVC_ENTRY_NO			(14)
#define	SYSTICK_ENTRY_NO			(15)


typedef	void (*HandlerEntry)(void);

extern	void _reset_entry(void);
extern	void _nmi_entry(void);
extern	void _hard_fault_entry(void);
extern	void _mem_manage_entry(void);
extern	void _bus_fault_entry(void);
extern	void _usage_fault_entry(void);
extern	void _svc_entry(void);
extern	void _debug_entry(void);
extern	void _pendsvc_entry(void);
extern	void _systick_entry(void);

extern	uint8_t			stack_master;

const HandlerEntry handler_entry[IRQ_NUM] = {
	[RESET_STACK_NO]		= (HandlerEntry)(&stack_master),
	[RESET_ENTRY_NO]		= _reset_entry,
	[NMI_ENTRY_NO]			= _nmi_entry,
	[HARD_FAULT_ENTRY_NO]	= _hard_fault_entry,
	[MEM_MANAGE_ENTRY_NO]	= _mem_manage_entry,
	[BUS_FAULT_ENTRY_NO]	= _bus_fault_entry,
	[USAGE_FAULT_ENTRY_NO]	= _usage_fault_entry,
	[SVC_ENTRY_NO]			= _svc_entry,
	[DEBUG_ENTRY_NO]		= _debug_entry,
	[PENDSVC_ENTRY_NO]		= _pendsvc_entry,
	[SYSTICK_ENTRY_NO]		= _systick_entry,
};

