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

FPU_OBJS   = $(FPU_C_SRCS:%.c=%.o) $(FPU_CXX_SRCS:%.cxx=%.o)
NOFPU_OBJS = $(C_SRCS:%.c=%.o) $(CXX_SRCS:%.cxx=%.o)
OTHER_OBJS = $(A_SRCS:%.S=%.o)
OBJS       = $(FPU_OBJS) $(NOFPU_OBJS) $(OTHER_OBJS)

DEFS += -DTEST_MODE
DEFS += -DOS_VER="0.01"

DEFS += -DTASK_MAX_NUM=1000
DEFS += -DFLAG_MAX_NUM=1000
DEFS += -DSEM_MAX_NUM=1000
DEFS += -DMUTEX_MAX_NUM=1000
DEFS += -DMSGQ_MAX_NUM=1000
DEFS += -DFIXMB_MAX_NUM=1000
DEFS += -DTIMER_MAX_NUM=1000

ifeq ($(USE_GICV2),1)
GICOPT = --no-gicv3
else
GICOPT = --gicv3
endif

ifeq ($(NOFPU_TARGET_OBJ),1)
CFLAGS += -mgeneral-regs-only
CXXFLAGS += -mgeneral-regs-only
endif

all:
	make NOFPU_TARGET_OBJ=1 build_nofpu_objs
	make build_fpu_objs
	make $(TARGET)

include $(ARCHDIR)/config.mk
include $(KERNDIR)/config.mk
include $(UTILDIR)/config.mk
include $(DRVDIR)/config.mk
include source.mk
include rule.mk
