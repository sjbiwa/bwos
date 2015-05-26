#
#
#

include $(ARCHDIR)/board/$(BOARD)/config.mk

PROJECT = bwos
TARGET = $(PROJECT).bin
TARGET_BIN = $(PROJECT).bin
TARGET_ELF = $(PROJECT).elf

# for CYGWIN
#TOOL_PREFIX = arm-linux-gnueabihf-
TOOL_PREFIX = arm-none-eabi-

#for Linux
#TOOL_PREFIX = arm-unknown-linux-gnueabi-

CC = $(TOOL_PREFIX)gcc
AS = $(TOOL_PREFIX)gcc
LD = $(TOOL_PREFIX)gcc
OBJCOPY = $(TOOL_PREFIX)objcopy
RM = rm

A_SRCS += boot.S dispatch.S handler.S arch_api.S spinlock.S
C_SRCS += arch.c memmgr.c api_table.c api_caller.c api_callee.c idle.c
C_SRCS += arch_timer.c

LDSCRIPT = $(ARCHDIR)/link.lds

#DEFS = -DCYGWIN
DEFS += -DUSE_TICKLESS -DSMP_ENABLE -DSMP_CPU_NUM=2
CFLAGS  +=  -std=gnu11 -mcpu=cortex-a7 -mfloat-abi=hard -mfpu=vfpv4 -mno-thumb-interwork -mthumb $(DEFS)
CFLAGS  += -g -O0 -fno-builtin
AFLAGS  += $(CFLAGS) -Wa,-mthumb,-mimplicit-it=thumb -D__ASM__
LDFLAGS += -v -mcpu=cortex-a7 -mno-thumb-interwork -g -T $(LDSCRIPT) -Wl,-Ttext=$(START_MEM_ADDR),--build-id=none -static -nostdlib
