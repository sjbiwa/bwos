#
#
#

VPATH += $(CONTRIB_JPEGTURBODIR) $(CONTRIB_JPEGTURBODIR)/simd
INC += -I$(CONTRIB_JPEGTURBODIR)

JPEG_TURBO_SRC = jcapimin.c jcapistd.c jccoefct.c jccolor.c \
				jcdctmgr.c jchuff.c jcinit.c jcmainct.c jcmarker.c jcmaster.c \
				jcomapi.c jcparam.c jcphuff.c jcprepct.c jcsample.c jctrans.c \
				jdapimin.c jdapistd.c jdatasrc.c jdcoefct.c jdcolor.c \
				jddctmgr.c jdhuff.c jdinput.c jdmainct.c jdmarker.c jdmaster.c \
				jdmerge.c jdphuff.c jdpostct.c jdsample.c jdtrans.c jerror.c \
				jfdctflt.c jfdctfst.c jfdctint.c jidctflt.c jidctfst.c jidctint.c \
				jidctred.c jquant1.c jquant2.c jutils.c jmemmgr.c jmemnobs.c

JPEG_TURBO_SRC += jsimd_arm.c
A_SRCS += jsimd_arm_neon.S

#JPEG_TURBO_SRC += jsimd_none.c

ifeq ($(ARCH),armv8a)
FPU_C_SRCS += $(JPEG_TURBO_SRC)
else
C_SRCS += $(JPEG_TURBO_SRC)
endif
