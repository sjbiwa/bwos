#
#
#
CPU_TYPE		= CORTEXM4F
TICK_TIMER		= BOARD
START_MEM_ADDR	= 0x14000000
CMSIS_DIR		= $(ARCHDIR)/board/$(BOARD)/CMSIS
LPC_DIR			= $(ARCHDIR)/board/$(BOARD)/lpc_core

# define
CFLAGS		   += -DCORE_M4
CFLAGS		   += -DMAX_CLOCK_FREQ=120000000
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

#include path
CFLAGS		   +=  -I$(ARCHDIR)/board/$(BOARD) -I$(LPC_DIR)/lpc_board/board_common -I$(LPC_DIR)/lpc_board/boards_43xx/ngx_xplorer_4330
CFLAGS		   += -I$(CMSIS_DIR)/Include -I$(LPC_DIR)/lpc_chip/chip_18xx_43xx -I$(LPC_DIR)/lpc_chip/chip_common \
				-I$(LPC_DIR)/lpc_chip/chip_18xx_43xx/config_43xx

VPATH		   += $(ARCHDIR)/board/$(BOARD)
C_SRCS		   += vector.c debug_print.c boards.c
ifeq ($(TICK_TIMER),BOARD)
C_SRCS		   += board_timer.c
CFLAGS		   += -DUSE_TICKLESS
endif
VPATH		   += $(LPC_DIR)/lpc_board/boards_43xx/ngx_xplorer_4330 $(LPC_DIR)/lpc_chip/chip_18xx_43xx
C_SRCS		   += board_sysinit.c board.c sysinit_18xx_43xx.c chip_18xx_43xx.c clock_18xx_43xx.c
C_SRCS		   += gpio_18xx_43xx.c