#
#
#
include config.mk
export ARCH CPU BOARD

PROJECT = bwos
TARGET = $(PROJECT).bin
TARGET_BIN = $(PROJECT).bin
TARGET_ELF = $(PROJECT).elf

MAKE = make
RM = rm

TOPDIR = ..
DRVDIR  = $(TOPDIR)/driver

INC = -I$(TOPDIR)/include
VPATH = $(TOPDIR)/kernel $(TOPDIR)/util

KERNEL_SRCS = main.c task.c irq.c timer.c idle.c flag.c mutex.c malloc.c
DRIVER_SRCS =
UTIL_SRCS = printf.c memory.c

C_SRCS = $(KERNEL_SRCS) $(DRIVER_SRCS) $(UTIL_SRCS) $(APP_SRCS)
OBJS = $(C_SRCS:%.c=%.o) $(A_SRCS:%.S=%.o)

all: $(TARGET)

.c.o:
	$(CC) -c $(CFLAGS) -v $<

.S.o:
	$(AS) -c $(AFLAGS) -v $<
	
$(TARGET_BIN): $(OBJS) $(HEADER)
	$(LD) $(LDFLAGS) -o $(TARGET_ELF) $(OBJS)
	$(OBJCOPY) -O binary $(TARGET_ELF) $(TARGET_BIN)

clean:
	$(RM) -f $(TARGET) $(TARGET_BIN) $(TARGET_ELF) $(OBJS)

include $(ARCH)/config.mk
include $(APPLICATION)/config.mk
	