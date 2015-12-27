#
#
#
CPU_TYPE		= CORTEXA53
START_MEM_ADDR	= 0x80000000

# define

# include path
CFLAGS		   += -I$(ARCHDIR)/board/$(BOARD)
VPATH		   += $(ARCHDIR)/board/$(BOARD)
C_SRCS		   += debug_print.c
