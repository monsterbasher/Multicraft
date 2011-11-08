TARGET = Aurora
OBJS = src/Aurora/Math/MathLib.o \
src/Aurora/Math/Matrix3.o \
src/Aurora/Math/Matrix4.o \
src/Aurora/Math/Quaternion.o \
src/Aurora/Math/BoundingBox.o \
src/Aurora/Math/BoundingSphere.o \
src/Aurora/Math/Plane.o \
src/Aurora/Math/Ray.o \
src/Aurora/Math/Frustum.o \
src/Aurora/Math/Polygon.o \
src/Aurora/Graphics/RenderManager.o \
src/Aurora/Graphics/psp/vram.o \
src/Aurora/Graphics/psp/GuRenderManager.o \
src/Aurora/Graphics/TextureManager.o \
src/Aurora/Graphics/Camera.o \
src/Aurora/Graphics/Image.o \
src/Aurora/Graphics/Sprite.o \
src/Aurora/Graphics/Sprite3D.o \
src/Aurora/Graphics/TrueTypeFont.o \
src/Aurora/System/SystemManager.o \
src/Aurora/System/psp/Clock.o \
src/Aurora/System/psp/PspSystemManager.o \
src/Aurora/Utils/GameLoader.o \
src/Aurora/Utils/GameManager.o \
src/Aurora/Utils/psp/PspCommon.o \
src/Aurora/Utils/psp/PspGameLoader.o \
ExampleGame.o \
Starter.o

INCDIR = $(PSPPATH)/include
INCDIR += ./incl ./src 
CFLAGS = -g -G0 -Wall -mpreferred-stack-boundary=4 #-enable-newlib-hw-fp -ffast-math
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PSP_FW_VERSION=371
PSP_LARGE_MEMORY = 1

LIBDIR += 
LDFLAGS =
LIBS= -lpng -lz -lpspgum -lpspgu -lstdc++ -lpsprtc -lpspctrl -lm -lpspvfpu -lpspsdk -lpsppower

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Aurora
#PSP_EBOOT_ICON = ICON0.png
#PSP_EBOOT_ICON1 = ICON1.PMF
#PSP_EBOOT_PIC1 = PIC1.png

PSPSDK=$(shell c:/pspsdk/bin/psp-config.exe -p)
include $(PSPSDK)/lib/build.mak
