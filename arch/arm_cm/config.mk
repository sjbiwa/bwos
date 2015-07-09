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

A_SRCS += boot.S dispatch.S
C_SRCS += arch.c idle.c
ifeq ($(TICK_TIMER),SYS_TICK)
C_SRCS += arch_timer.c
endif

LDSCRIPT = $(ARCHDIR)/board/$(BOARD)/link.lds

#DEFS = -DCYGWIN

DEFS += -DUSE_SMP=0 -DCPU_NUM=1 -DNO_USE_SVC_CALL -DMASTER_CPU_ID=0

ifeq ($(CPU_TYPE),CORTEXM0)
CPUFLAGS = -mcpu=cortex-m0
endif
ifeq ($(CPU_TYPE),CORTEXM4)
CPUFLAGS = -mcpu=cortex-m4
endif
ifeq ($(CPU_TYPE),CORTEXM4F)
CPUFLAGS = -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
endif
CFLAGS  += -std=gnu11 $(CPUFLAGS) -mno-thumb-interwork -mthumb $(DEFS)
CFLAGS  += -g -gdwarf-3 -Os -fno-builtin -mno-unaligned-access
AFLAGS  += $(CFLAGS) -Wa,-mthumb,-mimplicit-it=thumb -D__ASM__
LDFLAGS += $(CPUFLAGS) -mthumb -mno-thumb-interwork -g -T $(LDSCRIPT)
LDFLAGS += -nostdlib -static -Wl,-Ttext=$(START_MEM_ADDR),--build-id=none
LDLIBS   = -lgcc
