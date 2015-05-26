#
#
#

ifeq ($(BOARD),firefly_rk3288)
C_SRCS += clock.c uart.c gpio.c video.c spi.c i2c.c
endif
