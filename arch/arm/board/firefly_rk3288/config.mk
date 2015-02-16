#
#
#
START_MEM_ADDR = 0x00000000

CFLAGS += -I$(ARCHDIR)/board/$(BOARD)
VPATH += $(ARCHDIR)/board/$(BOARD)
C_SRCS += board_memmgr.c debug_print.c

