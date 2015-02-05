#
#
#
CFLAGS += -I$(ARCHDIR)/board/$(BOARD)
VPATH += $(ARCHDIR)/board/$(BOARD)
C_SRCS += board_memmgr.c
