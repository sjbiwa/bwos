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

A_SRCS += boot.S
C_SRCS += gicv3.c

LDSCRIPT = $(ARCHDIR)/link.lds

#DEFS = -DCYGWIN

DEFS += -DUSE_SMP=0 -DCPU_NUM=1 -DMASTER_CPU_ID=0

CPUFLAGS = -mcpu=$(CPU)

CFLAGS  += -std=gnu11 $(CPUFLAGS) $(DEFS)
CFLAGS  += -g -gdwarf-3 -O2 -fno-builtin
AFLAGS  += $(CFLAGS) -D__ASM__
LDFLAGS += $(CPUFLAGS) -g -T $(LDSCRIPT)
LDFLAGS += -nostdlib -static -Wl,-Ttext=$(START_MEM_ADDR),--build-id=none
LDLIBS   = -lgcc

printf.o: printf.c
	$(CC) -c -mgeneral-regs-only $(CFLAGS) $(INC) $(DEFS) $<
