TARGET_LIB = libEnet.a
OBJS = callbacks.o compress.o host.o list.o packet.o peer.o protocol.o unix.o

PSPSDK = $(shell psp-config --pspsdk-path)
PSPPATH = $(shell psp-config --psp-prefix)

INCDIR = $(PSPPATH)/include
INCDIR += ./
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti -fsingle-precision-constant -fomit-frame-pointer
ASFLAGS = $(CFLAGS)

include $(PSPSDK)/lib/build.mak
