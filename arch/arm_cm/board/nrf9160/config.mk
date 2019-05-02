#
#
#
CPU_TYPE		= CORTEXM33
TICK_TIMER		= BOARD
START_MEM_ADDR	= 0x00000000
CMSIS_DIR		= $(ARCHDIR)/board/$(BOARD)/CMSIS_5
NRF_DIR			= $(ARCHDIR)/board/$(BOARD)/nrfx

# define
CFLAGS		   += -DCORE_M33
CFLAGS		   += -DMAX_CLOCK_FREQ=64000000
CFLAGS		   += -DNRF9160_XXAA
CFLAGS		   += -DDEBUG_ENABLE
CFLAGS		   += -DDEBUG_SEMIHOSTING
CFLAGS		   += -DUSE_VFP
CFLAGS		   += -DHAVE_VTOR
CFLAGS		   += -DMPU_FAULT_PRIORITY=0x00
CFLAGS		   += -DBUS_FAULT_PRIORITY=0x00
CFLAGS		   += -DUSAGE_FAULT_PRIORITY=0x00
CFLAGS		   += -DSVCALL_PRIORITY=0xff
CFLAGS		   += -DDEBUG_MONITOR_PRIORITY=0xff
CFLAGS		   += -DPENDSV_PRIORITY=0xff
CFLAGS		   += -DSYSTICK_PRIORITY=0x40

# include path
CFLAGS		   += -I$(ARCHDIR)/board/$(BOARD)
CFLAGS		   += -I$(CMSIS_DIR)/Device/ARM/ARMCM33/Include
CFLAGS		   += -I$(CMSIS_DIR)/CMSIS/Core/Include
CFLAGS		   += -I$(NRF_DIR)
CFLAGS		   += -I$(NRF_DIR)/drivers
CFLAGS		   += -I$(NRF_DIR)/drivers/include
CFLAGS		   += -I$(NRF_DIR)/hal
CFLAGS		   += -I$(NRF_DIR)/mdk
CFLAGS		   += -I$(NRF_DIR)/soc
CFLAGS		   += -I$(NRF_DIR)/templates

VPATH		   += $(ARCHDIR)/board/$(BOARD)
VPATH		   += $(NRF_DIR)/drivers/src

C_SRCS		   += vector.c debug_print.c boards.c
ifeq ($(TICK_TIMER),BOARD)
C_SRCS		   += rtc_timer.c
CFLAGS		   += -DUSE_TICKLESS
endif
