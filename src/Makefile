TARGET = pspftp
APPOBJS=  \
debug.o \
ftpclient.o \
_itoa.o \
loadutil.o \
main.o \
nlh.o \
picker.o \
sutils.o

OBJS = $(APPOBJS) 
LIBS=-lpspwlan -lpsppower -lm

INCDIR = 
CFLAGS = -O2 -G0 -Wall 
# -DDEBUG
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS) -c

LIBDIR =
LDFLAGS =

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = FTP Client by liquid8d
PSP_EBOOT_ICON= pspftp.png

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

install: kxploit
	cp __SCE__pspftp/EBOOT.PBP /media/usbdisk/psp/game/__SCE__pspftp/
	cp %__SCE__pspftp/EBOOT.PBP /media/usbdisk/psp/game/%__SCE__pspftp/

