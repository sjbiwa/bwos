#
#
#

include $(ARCHDIR)/board/$(BOARD)/config.mk

PROJECT = bwos
TARGET = $(PROJECT).bin
TARGET_BIN = $(PROJECT).bin
TARGET_ELF = $(PROJECT).elf

# for CYGWIN
TOOL_PREFIX = arm-linux-gnueabihf-
#TOOL_PREFIX = arm-none-eabi-

#for Linux
#TOOL_PREFIX = arm-unknown-linux-gnueabi-

CC = $(TOOL_PREFIX)gcc
AS = $(TOOL_PREFIX)gcc
LD = $(TOOL_PREFIX)gcc
OBJCOPY = $(TOOL_PREFIX)objcopy
RM = rm

A_SRCS += boot.S dispatch.S handler.S arch_api.S
C_SRCS += arch.c memmgr.c api_table.c api_caller.c api_callee.c idle.c
C_SRCS += arch_timer.c

LDSCRIPT = $(ARCHDIR)/link.lds

#DEFS = -DCYGWIN

ifeq ($(SMP),1)
DEFS += -DSMP_ENABLE -DCPU_NUM=$(SMP_CPU_NUM)
C_SRCS += arch_smp.c
A_SRCS += spinlock.S
else
DEFS += -DSMP_ENABLE -DCPU_NUM=1
endif

DEFS += -DUSE_TICKLESS -DMASTER_CPU_ID=0
CFLAGS  +=  -std=gnu11 -mcpu=cortex-a7 -mfloat-abi=hard -mfpu=vfpv4 -mno-thumb-interwork -mthumb $(DEFS)
CFLAGS  += -g -gdwarf-3 -O1 -fno-builtin -mno-unaligned-access
AFLAGS  += $(CFLAGS) -Wa,-mthumb,-mimplicit-it=thumb -D__ASM__
LDFLAGS += -mcpu=cortex-a7 -mno-thumb-interwork -g -T $(LDSCRIPT)
LDFLAGS += -nostdlib -static -Wl,-Ttext=$(START_MEM_ADDR),--build-id=none
LDLIBS   = -lgcc
