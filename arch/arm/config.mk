#
#
#

PROJECT = bwos
TARGET = $(PROJECT).bin
TARGET_BIN = $(PROJECT).bin
TARGET_ELF = $(PROJECT).elf

# for CYGWIN
TOOL_PREFIX = arm-linux-gnueabihf-

#for Linux
#TOOL_PREFIX = arm-unknown-linux-gnueabi-

CC = $(TOOL_PREFIX)gcc
AS = $(TOOL_PREFIX)gcc
LD = $(TOOL_PREFIX)gcc
OBJCOPY = $(TOOL_PREFIX)objcopy
RM = rm

A_SRCS += boot.S dispatch.S handler.S arch_api.S
C_SRCS += timer.c arch.c memmgr.c api_table.c api_caller.c api_callee.c
LDSCRIPT = $(ARCHDIR)/link.lds

#DEFS = -DCYGWIN
DEFS += -DUSE_TICKLESS
CFLAGS  +=  -mcpu=cortex-a7 -mfloat-abi=hard -mfpu=vfpv4 -mno-thumb-interwork -mthumb $(DEFS)
CFLAGS  += -Os -fno-builtin
AFLAGS  += $(CFLAGS) -Wa,-mthumb,-mimplicit-it=thumb -D__ASM__
LDFLAGS += -v -mcpu=cortex-a7 -mno-thumb-interwork -g -T $(LDSCRIPT) -Wl,-Ttext=0x40000000,--build-id=none -static -nostdlib

