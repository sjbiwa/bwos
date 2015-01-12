/*
 * string.c
 *
 *  Created on: 2015/01/10
 *      Author: biwa
 */
#include "common.h"

char* strcpy(char *dest, const char *src)
{
	while ( *src ) {
		*dest++ = *src++;
	}
	return dest;
}

char* strncpy(char *dest, const char *src, uint32_t n)
{
	while ( *src && n ) {
		*dest++ = *src++;
		n--;
	}
	if ( n ) {
		*dest = '\0';
	}
	return dest;
}
