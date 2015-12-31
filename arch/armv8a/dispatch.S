/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

#include "armv8.h"
#include "armv8reg.h"

	.text
	.balign 4

	.global _dispatch
	.type   _dispatch, %function

/*
**  _dispatch
*/
_dispatch:

/*
**	_entry_stub
**		タスクエントリーから起動するためのスタブ
*/
	.global _entry_stub
	.type   _entry_stub, %function
_entry_stub:

/*
**  _dispatch_irq
*/

	.global _dispatch_irq
	.global	_nodispatch_irq
	.type   _dispatch_irq, %function
	.type   _nodispatch_irq, %function

_dispatch_irq:

_nodispatch_irq:

/*
**	_idle_loop
*/
	.type   _idle_loop, %function
_idle_loop:
