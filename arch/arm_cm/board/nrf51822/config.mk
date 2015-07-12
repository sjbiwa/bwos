#
#
#
CPU_TYPE		= CORTEXM0
TICK_TIMER		= BOARD
START_MEM_ADDR	= 0x00000000
NRF_DIR			= $(ARCHDIR)/board/$(BOARD)/nRF51_SDK_8.0.0

# define
CFLAGS		   += -DCORE_M0
CFLAGS		   += -DMAX_CLOCK_FREQ=16000000
CFLAGS		   += -DDEBUG_ENABLE
CFLAGS		   += -DDEBUG_SEMIHOSTING
#CFLAGS		   += -DUSE_VFP
#CFLAGS		   += -DHAVE_VTOR
#CFLAGS		   += -DMPU_FAULT_PRIORITY=0x00
#CFLAGS		   += -DBUS_FAULT_PRIORITY=0x00
#CFLAGS		   += -DUSAGE_FAULT_PRIORITY=0x00
CFLAGS		   += -DSVCALL_PRIORITY=0xff
#CFLAGS		   += -DDEBUG_MONITOR_PRIORITY=0xff
CFLAGS		   += -DPENDSV_PRIORITY=0xff
#CFLAGS		   += -DSYSTICK_PRIORITY=0x40

# include path
CFLAGS		   += -I$(ARCHDIR)/board/$(BOARD) -I$(NRF_DIR)/components/toolchain -I$(NRF_DIR)/components/toolchain/gcc
CFLAGS		   += -I$(NRF_DIR)/components/device
VPATH		   += $(ARCHDIR)/board/$(BOARD)
C_SRCS		   += vector.c debug_print.c boards.c
ifeq ($(TICK_TIMER),BOARD)
C_SRCS		   += board_timer.c
CFLAGS		   += -DUSE_TICKLESS
endif
