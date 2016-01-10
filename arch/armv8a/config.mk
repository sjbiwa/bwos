#
# config.mk for ARMv8-A
#

include $(ARCHDIR)/board/$(BOARD)/config.mk

PROJECT = bwos
TARGET = $(PROJECT).bin
TARGET_BIN = $(PROJECT).bin
TARGET_ELF = $(PROJECT).elf

# for CYGWIN
TOOL_PREFIX =  aarch64-none-elf-

CC = $(TOOL_PREFIX)gcc
AS = $(TOOL_PREFIX)gcc
LD = $(TOOL_PREFIX)gcc
OBJCOPY = $(TOOL_PREFIX)objcopy
RM = rm

A_SRCS += boot.S handler.S arch_api.S dispatch.S
C_SRCS += api_table.c arch.c idle.c gicv3.c arch_timer.c memmgr.c arch_ptbl.c

LDSCRIPT = $(ARCHDIR)/link.lds

#DEFS = -DCYGWIN

DEFS += -DCPU_NUM=$(SMP_CPU_NUM)
ifneq ($(SMP_CPU_NUM),1)
DEFS += -DUSE_SMP=1
else
DEFS += -DUSE_SMP=0
endif
C_SRCS += arch_smp.c
A_SRCS += spinlock.S

DEFS += -DMASTER_CPU_ID=0 -DUSE_TICKLESS

CPUFLAGS = -mcpu=$(CPU)

CFLAGS  += -std=gnu11 $(CPUFLAGS) $(DEFS)
CFLAGS  += -g -gdwarf-3 -O2 -fno-builtin
AFLAGS  += $(CFLAGS) -D__ASM__
LDFLAGS += $(CPUFLAGS) -g -T $(LDSCRIPT)
LDFLAGS += -nostdlib -static -Wl,-Ttext=$(START_MEM_ADDR),--build-id=none
LDLIBS   = -lgcc
