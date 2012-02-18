#ifndef AURORALIB_H
#define AURORALIB_H

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void Java_com_drakon_AuroraActivity_nativePause(JNIEnv * env, jobject javaThis);
JNIEXPORT void Java_com_drakon_AuroraActivity_nativeResume(JNIEnv * env, jobject javaThis);

JNIEXPORT void Java_com_drakon_AuroraGLSurfaceView_nativeTouchEvent(JNIEnv * env, jobject javaThis,jint action,jint number,jint x,jint y);

JNIEXPORT void Java_com_drakon_AuroraRenderer_nativeInit(JNIEnv * env, jobject javaThis, jstring apkPath, jstring savePath, jint width, jint height);
JNIEXPORT void Java_com_drakon_AuroraRenderer_nativeResize(JNIEnv* env, jobject javaThis,jint w, jint h );
JNIEXPORT void Java_com_drakon_AuroraRenderer_nativeRender(JNIEnv* env, jobject javaThis);

#ifdef __cplusplus
}
#endif

#endif
