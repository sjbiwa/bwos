#
#
#
include config.mk
export ARCH CPU BOARD APPLICATION TOPDIR

PROJECT = bwos
TARGET = $(PROJECT).bin
TARGET_BIN = $(PROJECT).bin
TARGET_ELF = $(PROJECT).elf

MAKE = make
RM = rm

TOPDIR = ../..
ARCHDIR = $(TOPDIR)/arch/$(ARCH)
APPDIR  = .
KERNDIR = $(TOPDIR)/kernel
UTILDIR = $(TOPDIR)/util
DRVDIR  = $(TOPDIR)/driver

INC   = -I$(ARCHDIR) -I$(APPDIR) -I$(KERNDIR) -I$(UTILDIR) -I$(DRVDIR) -I$(TOPDIR)/include
VPATH = $(ARCHDIR) $(APPDIR) $(KERNDIR) $(UTILDIR) $(DRVDIR)

OBJS = $(C_SRCS:%.c=%.o) $(A_SRCS:%.S=%.o)

DEFS += -DTEST_MODE
DEFS += -DOS_VER="0.01"

DEFS += -DTASK_MAX_NUM=5
DEFS += -DFLAG_MAX_NUM=3
DEFS += -DSEM_MAX_NUM=3
DEFS += -DMUTEX_MAX_NUM=3
DEFS += -DMSGQ_MAX_NUM=0
DEFS += -DFIXMB_MAX_NUM=0
DEFS += -DTIMER_MAX_NUM=0

C_SRCS += target.c

all: $(TARGET)

include $(ARCHDIR)/config.mk
include $(KERNDIR)/config.mk
include $(UTILDIR)/config.mk
include $(DRVDIR)/config.mk
include rule.mk
