/*
 * malloc.h
 *
 *  Created on: 2014/11/19
 *      Author: biwa
 */

#ifndef MALLOC_H_
#define MALLOC_H_

#include <stdint.h>
#include "common.h"

OSAPI void* malloc(uint32_t size);
OSAPI void free(void* ptr);

#endif /* MALLOC_H_ */
