include MakeDefs

GFX_COMMON=Common

PPU_INCDIRS			+= -Iinclude -I./../../ -I./../../libs/libzip/
PPU_CPPFLAGS		+= -DPSGL -DAURORA_PS3 -DUSE_FILE32API
PPU_CXXSTDFLAGS		+= -fno-exceptions

PPU_SRCS			:= ../../Aurora/Math/BoundingBox.cpp \
					../../Aurora/Math/BoundingSphere.cpp \
					../../Aurora/Math/Frustum.cpp \
					../../Aurora/Math/MathLib.cpp \
					../../Aurora/Math/Matrix3.cpp \
					../../Aurora/Math/Matrix4.cpp \
					../../Aurora/Math/Plane.cpp \
					../../Aurora/Math/Polygon.cpp \
					../../Aurora/Math/Quaternion.cpp \
					../../Aurora/Math/Ray.cpp \
					../../Aurora/Math/Vector2.cpp \
					../../Aurora/Math/Vector3.cpp \
					../../Aurora/Math/Vector4.cpp \
					../../Aurora/Graphics/Camera.cpp \
					../../Aurora/Graphics/Image.cpp \
					../../Aurora/Graphics/ModelObj.cpp \
					../../Aurora/Graphics/RenderManager.cpp \
					../../Aurora/Graphics/opengl/PSGLRenderManager.cpp \
					../../Aurora/Graphics/SplashScreen.cpp \
					../../Aurora/Graphics/Sprite.cpp \
					../../Aurora/Graphics/Sprite3D.cpp \
					../../Aurora/Graphics/TextureManager.cpp \
					../../Aurora/Graphics/TrueTypeFont.cpp \
					../../Aurora/System/DiskFile.cpp \
					../../Aurora/System/File.cpp \
					../../Aurora/System/FileManager.cpp \
					../../Aurora/System/VFSFile.cpp \
					../../Aurora/System/VFSPack.cpp \
					../../Aurora/System/ZipFile.cpp \
					../../Aurora/System/ZipPack.cpp \
					../../Aurora/Utils/md5.c \
					../../Aurora/Utils/AES.cpp \
					../../Aurora/Utils/Compression.cpp \
					../../Aurora/Utils/Crypto.cpp \
					../../Aurora/Utils/GameManager.cpp \
					../../libs/libzip/ioapi.c \
					../../libs/libzip/unzip.c \
					../../libs/libzip/zip.c \
					main.cpp


PPU_TARGET			:= Aurora.elf
PPU_LDLIBS			:= $(GRAPHICS_SAMPLE_CORE_LIBS) -lz

VPSHADER_SOURCES        := $(CELL_GRAPHICS_DATA_DIR)/shaders/Tutorial/vs_basicSample.cg
FPSHADER_SOURCES        := $(CELL_GRAPHICS_DATA_DIR)/shaders/Tutorial/fs_basicSample.cg

include MakeRules
