include MakeDefs

GFX_COMMON=Common

PPU_INCDIRS			+= -Iinclude -I./../../
PPU_CPPFLAGS		+= -DPSGL
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
					main.cpp


PPU_TARGET			:= Aurora.elf
PPU_LDLIBS			:= $(GRAPHICS_SAMPLE_CORE_LIBS) -lz

VPSHADER_SOURCES        := $(CELL_GRAPHICS_DATA_DIR)/shaders/Tutorial/vs_basicSample.cg
FPSHADER_SOURCES        := $(CELL_GRAPHICS_DATA_DIR)/shaders/Tutorial/fs_basicSample.cg

include MakeRules
