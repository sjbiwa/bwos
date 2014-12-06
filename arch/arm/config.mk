#
#
#

PROJECT = bwos
TARGET = $(PROJECT).bin
TARGET_BIN = $(PROJECT).bin
TARGET_ELF = $(PROJECT).elf

CC = arm-unknown-linux-gnueabi-gcc
AS = arm-unknown-linux-gnueabi-gcc
LD = arm-unknown-linux-gnueabi-gcc
OBJCOPY = arm-unknown-linux-gnueabi-objcopy
#CC = arm-none-eabi-gcc
#AS = arm-none-eabi-gcc
#LD = arm-none-eabi-gcc
#OBJCOPY = arm-none-eabi-objcopy
RM = rm

A_SRCS += boot.S dispatch.S handler.S
C_SRCS += timer.c
LDSCRIPT = $(ARCHDIR)/link.lds

CFLAGS  =  -march=armv7-a -mcpu=cortex-a9 -mno-thumb-interwork -mthumb -D__QEMU__
AFLAGS  = $(CFLAGS) -Wa,-mthumb,-mimplicit-it=thumb -D__ASM__ -D__QEMU__
LDFLAGS = -v -mcpu=cortex-a9 -mno-thumb-interwork -g -T $(LDSCRIPT) -Wl,-Ttext=0x10000 -static -nostdlib
