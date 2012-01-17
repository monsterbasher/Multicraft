TARGET = Aurora
OBJS = Aurora/Math/MathLib.o \
Aurora/Math/Vector2.o \
Aurora/Math/Vector3.o \
Aurora/Math/Vector4.o \
Aurora/Math/Matrix3.o \
Aurora/Math/Matrix4.o \
Aurora/Math/Quaternion.o \
Aurora/Math/BoundingBox.o \
Aurora/Math/BoundingSphere.o \
Aurora/Math/Plane.o \
Aurora/Math/Ray.o \
Aurora/Math/Frustum.o \
Aurora/Math/Polygon.o \
Aurora/Graphics/RenderManager.o \
Aurora/Graphics/psp/vram.o \
Aurora/Graphics/psp/GuRenderManager.o \
Aurora/Graphics/TextureManager.o \
Aurora/Graphics/Camera.o \
Aurora/Graphics/Image.o \
Aurora/Graphics/Sprite.o \
Aurora/Graphics/Sprite3D.o \
Aurora/Graphics/TrueTypeFont.o \
Aurora/Graphics/ModelObj.o \
Aurora/Network/Unix/SocketHelper.o \
Aurora/Network/psp/PspNetworkManager.o \
Aurora/Network/NetworkManager.o \
Aurora/Network/Http.o \
Aurora/Network/IPAddress.o \
Aurora/Network/Packet.o \
Aurora/Network/SelectorBase.o \
Aurora/Network/SocketTCP.o \
Aurora/Network/SocketUDP.o \
Aurora/Network/NetworkInputControllerClient.o \
Aurora/Network/NetworkInputControllerServer.o \
Aurora/System/SystemManager.o \
Aurora/System/psp/Clock.o \
Aurora/System/psp/PspSystemManager.o \
Aurora/Utils/GameLoader.o \
Aurora/Utils/GameManager.o \
Aurora/Utils/psp/PspCommon.o \
Aurora/Utils/psp/PspGameLoader.o \
Tests/ExampleGame.o \
Tests/Demo_ObjLoading.o \
Tests/ServerTest.o \
Tests/ClientTest.o \
Tests/SimpleTest.o \
Tests/NetworkControllerClient.o \
Tests/NetworkControllerServer.o \
Starter.o

INCDIR = $(PSPPATH)/include
INCDIR += ./
CFLAGS = -g -G0 -Wall -mpreferred-stack-boundary=4 #-enable-newlib-hw-fp -ffast-math
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PSP_FW_VERSION=371
PSP_LARGE_MEMORY = 1

LIBDIR += 
LDFLAGS =
LIBS= -lpng -lz -lpspgum -lpspgu -lstdc++ -lpsprtc -lpspctrl -lm -lpspvfpu -lpspsdk -lpsppower -lpspnet -lEnet

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Aurora
#PSP_EBOOT_ICON = ICON0.png
#PSP_EBOOT_ICON1 = ICON1.PMF
#PSP_EBOOT_PIC1 = PIC1.png

PSPSDK=$(shell c:/pspsdk/bin/psp-config.exe -p)
include $(PSPSDK)/lib/build.mak
