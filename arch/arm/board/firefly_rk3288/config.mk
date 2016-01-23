#
#
#
START_MEM_ADDR = 0x00001000

CFLAGS += -I$(ARCHDIR)/board/$(BOARD)
VPATH += $(ARCHDIR)/board/$(BOARD)
C_SRCS += board_memmgr.c debug_print.c board_init_task.c

