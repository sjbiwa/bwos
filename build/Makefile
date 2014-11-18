#
#
#

PROJECT = bwos
TARGET = $(PROJECT).bin
TARGET_BIN = $(PROJECT).bin
TARGET_ELF = $(PROJECT).elf

CC = arm-unknown-linux-gnueabi-gcc
AS = arm-unknown-linux-gnueabi-gcc
LD = arm-unknown-linux-gnueabi-gcc
OBJCOPY = arm-unknown-linux-gnueabi-objcopy
#CC = arm-none-eabi-gcc
#AS = arm-none-eabi-gcc
#LD = arm-none-eabi-gcc
#OBJCOPY = arm-none-eabi-objcopy
RM = rm

ARCH = -march=armv7-a -mcpu=cortex-a9

INC = -I../inc
VPATH = ../src

C_SRCS = main.c printf.c task.c irq.c timer.c idle.c resource.c target.c flag.c mutex.c
A_SRCS = boot.S dispatch.S handler.S
OBJS = $(C_SRCS:%.c=%.o) $(A_SRCS:%.S=%.o)

LDSCRIPT = link.lds

CFLAGS = $(ARCH) -mno-thumb-interwork -mthumb -g $(INC) -D__QEMU__ -O2
AFLAGS = $(CFLAGS) -Wa,-mthumb,-mimplicit-it=thumb -D__ASM__ -D__QEMU__
LDFLAGS = -v -mcpu=cortex-a9 -mno-thumb-interwork -g -T $(LDSCRIPT) -Wl,-Ttext=0x10000 -static -nostdlib

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
