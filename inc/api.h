/*
 * api.h
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */

#ifndef INC_API_H_
#define INC_API_H_

#include "common.h"
#include "task.h"
#include "flag.h"

#define	RT_OK				(0)
#define	RT_ERR				(-1)
#define	RT_TIMEOUT			(-2)
#define	RT_WAKEUP			(-3)
#define	RT_EVENT			(-4)

OSAPI int task_create(TaskStruct* task);
OSAPI void task_sleep(void);
OSAPI void task_wakeup(TaskStruct* task);
OSAPI int32_t task_tsleep(uint32_t tm);
OSAPI int flag_create(FlagStruct* flag);
OSAPI void flag_set(FlagStruct* flag);
OSAPI int flag_wait(FlagStruct* flag, uint32_t tmout);

#endif /* INC_API_H_ */
