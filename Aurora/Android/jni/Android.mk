LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES 		:= $(LOCAL_PATH)/../../ $(LOCAL_PATH)/../../libs/libzip
LOCAL_STATIC_LIBRARIES 	:= 
LOCAL_MODULE 			:= auroraLib
LOCAL_CFLAGS 			:= -DANDROID_NDK -DAURORA_ANDROID -DUSE_FILE32API
LOCAL_LDLIBS 			:= -lz -lGLESv1_CM -ldl -llog

LOCAL_SRC_FILES := \
    ../../Aurora/Math/BoundingBox.cpp \
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
	../../Aurora/Graphics/SplashScreen.cpp \
	../../Aurora/Graphics/Sprite.cpp \
	../../Aurora/Graphics/Sprite3D.cpp \
	../../Aurora/Graphics/TextureManager.cpp \
	../../Aurora/Graphics/TrueTypeFont.cpp \
	../../Aurora/Graphics/opengl/OpenGLES1RenderManager.cpp \
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
	../../libs/libzip/ioapi.c \
	../../libs/libzip/unzip.c \
	../../libs/libzip/zip.c \
	AuroraLib.cpp
    

include $(BUILD_SHARED_LIBRARY)