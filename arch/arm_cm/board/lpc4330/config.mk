#
#
#
START_MEM_ADDR = 0x10000000
CMSIS_DIR = $(ARCHDIR)/board/$(BOARD)/CMSIS
LPC_DIR = $(ARCHDIR)/board/$(BOARD)/lpc_core
CFLAGS += -DMAX_CLOCK_FREQ=120000000 -DDEBUG_ENABLE -DDEBUG_SEMIHOSTING
CFLAGS += -DCORE_M4 -DUSE_VFP -I$(ARCHDIR)/board/$(BOARD) -I$(LPC_DIR)/lpc_board/board_common -I$(LPC_DIR)/lpc_board/boards_43xx/ngx_xplorer_4330
CFLAGS += -I$(CMSIS_DIR)/Include -I$(LPC_DIR)/lpc_chip/chip_18xx_43xx -I$(LPC_DIR)/lpc_chip/chip_common \
			-I$(LPC_DIR)/lpc_chip/chip_18xx_43xx/config_43xx
VPATH += $(ARCHDIR)/board/$(BOARD)
C_SRCS += vector.c debug_print.c boards.c

VPATH += $(LPC_DIR)/lpc_board/boards_43xx/ngx_xplorer_4330 $(LPC_DIR)/lpc_chip/chip_18xx_43xx
C_SRCS += board_sysinit.c board.c sysinit_18xx_43xx.c chip_18xx_43xx.c clock_18xx_43xx.c
C_SRCS += gpio_18xx_43xx.c