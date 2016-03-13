#
# config file
#
ARCH = armv8a
CPU  = cortex-a53
BOARD = db410c
APPLICATION = application
SMP_CPU_NUM=1
USE_GICV2=1
DEFS += -DNO_WFI_WAIT
