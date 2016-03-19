#
#
#

VPATH += $(CONTRIB_JPEGDIR)
INC += -I$(CONTRIB_JPEGDIR)

JPEG_CMPRESS_SRC  = jcapimin.c jcapistd.c jctrans.c jcinit.c jcmaster.c jcmainct.c jcprepct.c jccoefct.c jccolor.c jcsample.c
JPEG_CMPRESS_SRC += jcdctmgr.c jfdctint.c jfdctfst.c jfdctflt.c jchuff.c jcarith.c jcmarker.c jdatadst.c

JPEG_DECOMPRESS_SRC  = jdapimin.c jdapistd.c jdtrans.c jdmaster.c jdinput.c jdmainct.c jdcoefct.c jdpostct.c jdmarker.c jdhuff.c jdarith.c jddctmgr.c 
JPEG_DECOMPRESS_SRC += jidctint.c jidctfst.c jidctflt.c jdsample.c jdcolor.c jdmerge.c jquant1.c jquant2.c jdatasrc.c

JPEG_COMMON_SRC = jcomapi.c jcparam.c jaricom.c jerror_local.c jmemmgr.c jutils.c

JPEG_MEMMAN_SRC = jmemnobs.c

ifeq ($(ARCH),armv8a)
FPU_C_SRCS += $(JPEG_DECOMPRESS_SRC) $(JPEG_COMMON_SRC) $(JPEG_MEMMAN_SRC)
else
C_SRCS += $(JPEG_DECOMPRESS_SRC) $(JPEG_COMMON_SRC) $(JPEG_MEMMAN_SRC)
endif
