/*
 * common.h
 *
 *  Created on: 2012/12/01
 *      Author: biwa
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "config.h"
#include "types.h"
#include "arch_common.h"
#include "sys_irq.h"

/* OS固有定義マクロ */
#define	OSAPI
#define	OSAPISTUB

#ifndef NULL
#define	NULL			(0)
#endif

/* ユーティリティマクロ */
#define	arrayof(a)			(sizeof(a)/sizeof((a)[0]))
#define	containerof(ptr, type, member) ({ const typeof(((type *)0)->member) *__mptr = (ptr);(type *)((char*)__mptr-offsetof(type,member));})
#define	iowrite32(a,d)		(*((volatile uint32_t*)(a))=(d))
#define	ioread32(a)			(*((volatile uint32_t*)(a)))

/* アドレスアラインメント演算用マクロ */
#define	PRE_ALIGN_BY(x,n)		((void*)((PtrInt_t)(x) & ~((PtrInt_t)((n)-1))))
#define	POST_ALIGN_BY(x,n)		((void*)(((PtrInt_t)(x)+(PtrInt_t)((n)-1)) & ~((PtrInt_t)((n)-1))))

#define	PTRVAR(x)				((uint8_t*)(x)) /* ポインタ演算用 (バイトアドレス型に変換) */
#define	PTRVAR(x)				((uint8_t*)(x)) /* ポインタ演算用 (バイトアドレス型に変換) */
#define	PTRVAR(x)				((uint8_t*)(x)) /* ポインタ演算用 (バイトアドレス型に変換) */
#define	PTRVAR(x)				((uint8_t*)(x)) /* ポインタ演算用 (バイトアドレス型に変換) */
#define	PTRVAR(x)				((uint8_t*)(x)) /* ポインタ演算用 (バイトアドレス型に変換) */

/* メモリサイズ用 */
#define	SIZE_PRE_ALIGN_BY(x,n)	((((PtrInt_t)(x) & ~((PtrInt_t)((n)-1))))
#define	SIZE_POST_ALIGN_BY(x,n)	((((PtrInt_t)(x)+(PtrInt_t)((n)-1)) & ~((PtrInt_t)((n)-1))))

#endif /* COMMON_H_ */
