LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES 		:= $(LOCAL_PATH)/../../ 
LOCAL_STATIC_LIBRARIES 	:= 
LOCAL_MODULE 			:= multicraft
LOCAL_CFLAGS 			:= -D__ANDROID__ -isystem $(SYSROOT)/usr/include -DANDROID_NDK -DPVRSDK
LOCAL_LDLIBS 			:= -ldl -llog

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
	
    

include $(BUILD_SHARED_LIBRARY)