#
#
#
ifeq ($(USE_FATFSLIB_MODULE),1)
CONTRIB_FATFSDIR = $(CONTRIBDIR)/fatfs
include $(CONTRIB_FATFSDIR)/config.mk
endif

ifeq ($(USE_JPEGLIB_MODULE),1)
CONTRIB_JPEGDIR = $(CONTRIBDIR)/jpeg-9b
include $(CONTRIB_JPEGDIR)/config.mk
endif
