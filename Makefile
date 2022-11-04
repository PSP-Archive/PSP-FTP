PSPSDK = $(shell psp-config --pspsdk-path)
PSPLIBSDIR = $(PSPSDK)/..
PSP_DIR_NAME = FTPC
TARGET = ftpc

#COMMON
OBJS=src/main.o src/sutils.o src/ftpclient.o src/debug.o src/_itoa.o
#NET UTILS
OBJS+=src/loadutil.o src/nlh.o src/stubs.o src/picker.o 

LIBS = -lpsppower -lpsputility -lpng -lz -lm -lpspwlan

CFLAGS = -O2 -G0 -Wall -march=r6000

CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS) -c

EXTRA_TARGETS = EBOOT.PBP

PSP_EBOOT_TITLE = FTP Client by liquid8d

include $(PSPSDK)/lib/build.mak

