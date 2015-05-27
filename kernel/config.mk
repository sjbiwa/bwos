#
#
#

C_SRCS += startup.c task.c irq.c st_malloc.c malloc.c flag.c mutex.c sem.c msgq.c fixmb.c init_task.c timer.c
ifeq ($(SMP),1)
C_SRCS += smp.c
endif
