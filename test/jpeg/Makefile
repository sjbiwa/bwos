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
CONTRIBDIR = $(TOPDIR)/contrib
JPEGDIR = ./jpeg-9b

INC   = -I$(ARCHDIR) -I$(APPDIR) -I$(KERNDIR) -I$(UTILDIR) -I$(DRVDIR) -I$(TOPDIR)/include -I$(JPEGDIR)
VPATH = $(ARCHDIR) $(APPDIR) $(KERNDIR) $(UTILDIR) $(DRVDIR) $(JPEGDIR)

OBJS = $(C_SRCS:%.c=%.o) $(CXX_SRCS:%.cxx=%.o) $(A_SRCS:%.S=%.o)

DEFS += -DTEST_MODE
DEFS += -DOS_VER="0.01"

DEFS += -DTASK_MAX_NUM=100
DEFS += -DFLAG_MAX_NUM=100
DEFS += -DSEM_MAX_NUM=100
DEFS += -DMUTEX_MAX_NUM=100
DEFS += -DMSGQ_MAX_NUM=100
DEFS += -DFIXMB_MAX_NUM=100

LDLIBS  += -lm

all: $(TARGET)

include $(ARCHDIR)/config.mk
include $(KERNDIR)/config.mk
include $(UTILDIR)/config.mk
include $(DRVDIR)/config.mk
include $(CONTRIBDIR)/config.mk
include source.mk
include rule.mk

