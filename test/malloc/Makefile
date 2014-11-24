#
#
#
TARGET = main.exe
TARGET_DIR = ../..

#CC = arm-unknown-linux-gnueabi-gcc
#AS = arm-unknown-linux-gnueabi-gcc
#LD = arm-unknown-linux-gnueabi-gcc
#OBJCOPY = arm-unknown-linux-gnueabi-objcopy
#CC = arm-none-eabi-gcc
#AS = arm-none-eabi-gcc
#LD = arm-none-eabi-gcc
#OBJCOPY = arm-none-eabi-objcopy
CC = gcc
RM = rm

INC = -I$(TARGET_DIR)/inc
VPATH = $(TARGET_DIR)/src

C_SRCS = main.c malloc.c
OBJS = $(C_SRCS:%.c=%.o) $(A_SRCS:%.S=%.o)

CFLAGS = -g $(INC) -O0

all: $(TARGET)

.c.o:
	$(CC) -c $(CFLAGS) -v $<

.S.o:
	$(AS) -c $(AFLAGS) -v $<
	
$(TARGET): $(OBJS) $(HEADER)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS)

clean:
	$(RM) -f $(TARGET) $(OBJS)
