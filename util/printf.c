#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include "common.h"
#include "mutex.h"

//#define	UART_BASE		(0x10009018)
#define	UART_BASE		(0x01C28000)

static int vtsprintf(char* buff,char* fmt,va_list arg);

static int tsprintf_string(char* ,char* );
static int tsprintf_char(int ,char* );
static int tsprintf_decimal(signed long,char* ,int ,int );
static int tsprintf_hexadecimal(unsigned long ,char* ,int ,int ,int );

static void
debug_print(uint8_t* str)
{
	for (;*str;str++) {
		while ( (*(volatile uint32_t*)(UART_BASE+0x7C) & (0x01<<2)) == 0 );
		*((volatile uint32_t*)(UART_BASE+0x00)) = *str;
	}
}

static int
vtsprintf(char* buff,char* fmt,va_list arg){
	int len;
	int size;
	int zeroflag,width;

	size = 0;
	len = 0;

	while(*fmt){
		if(*fmt=='%'){		/* % に関する処理 */
			zeroflag = width = 0;
			fmt++;

			if (*fmt == '0'){
				fmt++;
				zeroflag = 1;
			}
			if ((*fmt >= '0') && (*fmt <= '9')){
				width = *(fmt++) - '0';
			}

			/* printf ("zerof = %d,width = %d\n",zeroflag,width); */

			switch(*fmt){
			case 'd':		/* 10進数 */
				size = tsprintf_decimal(va_arg(arg,signed long),buff,zeroflag,width);
				break;
			case 'x':		/* 16進数 0-f */
				size = tsprintf_hexadecimal(va_arg(arg,unsigned long),buff,0,zeroflag,width);
				break;
			case 'X':		/* 16進数 0-F */
				size = tsprintf_hexadecimal(va_arg(arg,unsigned long),buff,1,zeroflag,width);
				break;
			case 'c':		/* キャラクター */
				size = tsprintf_char(va_arg(arg,int),buff);
				break;
			case 's':		/* ASCIIZ文字列 */
				size = tsprintf_string(va_arg(arg,char*),buff);
				break;
			default:		/* コントロールコード以外の文字 */
				/* %%(%に対応)はここで対応される */
				len++;
				*(buff++) = *fmt;
				break;
			}
			len += size;
			buff += size;
			fmt++;
		} else {
			*(buff++) = *(fmt++);
			len++;
		}
	}

	*buff = '\0';		/* 終端を入れる */

	va_end(arg);
	return (len);
}




/*
  数値 => 10進文字列変換
*/
static int
tsprintf_decimal(signed long val,char* buff,int zf,int wd){
	int i;
	char tmp[10];
	char* ptmp = tmp + 10;
	int len = 0;
	int minus = 0;

	if (!val){		/* 指定値が0の場合 */
		*(ptmp--) = '0';
		len++;
	} else {
		/* マイナスの値の場合には2の補数を取る */
		if (val < 0){
			val = ~val;
			val++;
			minus = 1;
		}
		while (val){
			/* バッファアンダーフロー対策 */
			if (len >= 8){
				break;
			}

			*ptmp = (val % 10) + '0';
			val /= 10;
			ptmp--;
			len++;
		}

	}

	/* 符号、桁合わせに関する処理 */
	if (zf){
		if (minus){
			wd--;
		}
		while (len < wd){
			*(ptmp--) =  '0';
			len++;
		}
		if (minus){
			*(ptmp--) = '-';
			len++;
		}
	} else {
		if (minus){
			*(ptmp--) = '-';
			len++;
		}
		while (len < wd){
			*(ptmp--) =  ' ';
			len++;
		}
	}

	/* 生成文字列のバッファコピー */
	for (i=0;i<len;i++){
		*(buff++) = *(++ptmp);
	}

	return (len);
}

/*
  数値 => 16進文字列変換
*/
static int
tsprintf_hexadecimal(unsigned long val,char* buff,
								int capital,int zf,int wd){
	int i;
	char tmp[10];
	char* ptmp = tmp + 10;
	int len = 0;
	char str_a;

	/* A～Fを大文字にするか小文字にするか切り替える */
	if (capital){
		str_a = 'A';
	} else {
		str_a = 'a';
	}

	if (!val){		/* 指定値が0の場合 */
		*(ptmp--) = '0';
		len++;
	} else {
		while (val){
			/* バッファアンダーフロー対策 */
			if (len >= 8){
				break;
			}

			*ptmp = (val % 16);
			if (*ptmp > 9){
				*ptmp += str_a - 10;
			} else {
				*ptmp += '0';
			}

			val >>= 4;		/* 16で割る */
			ptmp--;
			len++;
		}
	}
	while (len < wd){
		*(ptmp--) =  zf ? '0' : ' ';
		len++;
	}

	for (i=0;i<len;i++){
		*(buff++) = *(++ptmp);
	}

	return(len);
}

/*
  数値 => 1文字キャラクタ変換
*/
static int
tsprintf_char(int ch,char* buff){
	*buff = (char)ch;
	return(1);
}

/*
  数値 => ASCIIZ文字列変換
*/
static int
tsprintf_string(char* str,char* buff){
	int count = 0;
	while(*str){
		*(buff++) = *str;
		str++;
		count++;
	}
	return(count);
}


/*
  Tiny sprintf関数
*/
int
tsprintf(char* buff,char* fmt, ...){
	va_list arg;
	int len;
	int size;
	int zeroflag,width;

	size = 0;
	len = 0;
	va_start(arg, fmt);

	vtsprintf(buff,fmt,arg);

	va_end(arg);
}

/*
  Tiny sprintf関数
*/
static MutexStruct printf_mutex;

void lprintf_init(void)
{
	mutex_create(&printf_mutex);
#if 0
	*(volatile uint32_t*)(UART_BASE+0x2C) = (0x07<<4); /* UARTLCR_H */
	*(volatile uint32_t*)(UART_BASE+0x30) = (0x03<<8)|(0x01); /* UARTCR */
#endif
}

int tprintf(char* fmt, ...)
{
static char		buff[1024];
	va_list arg;
	int len;
	int size;
	int zeroflag,width;

	size = 0;
	len = 0;
	va_start(arg, fmt);

	vtsprintf(buff,fmt,arg);

	va_end(arg);
	debug_print(buff);
}

int lprintf(char* fmt, ...)
{
static char		buff[1024];
	va_list arg;
	int len;
	int size;
	int zeroflag,width;

	size = 0;
	len = 0;
	va_start(arg, fmt);

	mutex_lock(&printf_mutex, 0);
	vtsprintf(buff,fmt,arg);

	va_end(arg);
	debug_print(buff);
	mutex_unlock(&printf_mutex);
}
