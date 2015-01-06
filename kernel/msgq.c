/*
 * msgq.c
 *
 *  Created on: 2015/01/07
 *      Author: biwa
 */

#include "common.h"
#include "task.h"
#include "msgq.h"
#include "link.h"
#include "api.h"

OSAPI int msgq_create(MsgqStruct* msgq, uint32_t length)
{

}

OSAPI int msgq_send(MsgqStruct* msgq, void* ptr)
{

}

OSAPI int msgq_tsend(MsgqStruct* msgq, void* ptr, TimeOut tmout)
{

}

OSAPI int msgq_recv(MsgqStruct* msgq, void** ptr)
{

}

OSAPI int msgq_trecv(MsgqStruct* msgq, void** ptr, TimeOut tmout)
{

}
