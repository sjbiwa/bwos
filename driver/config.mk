#
#
#

C_SRCS += clock.c

ifeq ($(BOARD),firefly_rk3288)
C_SRCS += uart.c gpio.c video.c spi.c i2c.c sdmmc.c
endif

ifeq ($(BOARD),arm_std)
C_SRCS += clcd.c arm_std_md.c
endif
