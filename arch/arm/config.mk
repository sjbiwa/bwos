#
#
#

PROJECT = bwos
TARGET = $(PROJECT).bin
TARGET_BIN = $(PROJECT).bin
TARGET_ELF = $(PROJECT).elf

TOOL_PREFIX = arm-linux-gnueabihf-
#CC = arm-unknown-linux-gnueabi-gcc
#AS = arm-unknown-linux-gnueabi-gcc
#LD = arm-unknown-linux-gnueabi-gcc
#OBJCOPY = arm-unknown-linux-gnueabi-objcopy
#CC = arm-none-eabi-gcc
#AS = arm-none-eabi-gcc
#LD = arm-none-eabi-gcc
#OBJCOPY = arm-none-eabi-objcopy

CC = $(TOOL_PREFIX)gcc
AS = $(TOOL_PREFIX)gcc
LD = $(TOOL_PREFIX)gcc
OBJCOPY = $(TOOL_PREFIX)objcopy
RM = rm

A_SRCS += boot.S dispatch.S handler.S
C_SRCS += timer.c arch.c memmgr.c
LDSCRIPT = $(ARCHDIR)/link.lds

DEFS = -DCYGWIN
CFLAGS  =  -march=armv7-a -mcpu=cortex-a9 -mfloat-abi=hard -mfpu=vfpv3 -mno-thumb-interwork -mthumb $(DEFS)
AFLAGS  = $(CFLAGS) -Wa,-mthumb,-mimplicit-it=thumb -D__ASM__
LDFLAGS = -v -mcpu=cortex-a9 -mno-thumb-interwork -g -T $(LDSCRIPT) -Wl,-Ttext=0x10000 -static -nostdlib
