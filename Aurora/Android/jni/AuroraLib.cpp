#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <GLES/gl.h>

#include "AuroraLib.h"

bool g_renderEnabled = true;
bool g_reloadTextures = false;

int   gAppAlive   = 1;

static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;
static int  sDemoStopped  = 0;
static long sTimeOffset   = 0;
static int  sTimeOffsetInit = 0;
static long sTimeStopped  = 0;
static long long prevTime = 0;

static int ACTION_DOWN = 0;

// Field descriptor #13 I
static int ACTION_UP = 1;

// Field descriptor #13 I
static int ACTION_MOVE = 2;

// Field descriptor #13 I
static int ACTION_CANCEL = 3;

JavaVM * g_javaVM;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

void Java_com_drakon_AuroraActivity_nativePause(JNIEnv * env, jobject javaThis)
{

}

void Java_com_drakon_AuroraActivity_nativeResume(JNIEnv * env, jobject javaThis)
{

}

void Java_com_drakon_AuroraGLSurfaceView_nativeTouchEvent(JNIEnv * env, jobject javaThis,jint action,jint x,jint y)
{
	pthread_mutex_lock(&g_mutex);

	int posX = x;
	int posY = y;

	//LOGI("Action %d x:%d y:%d",action,x,y);

	if(action == ACTION_DOWN)
	{
		
	}

	if(action == ACTION_UP)
	{
		
	}

	if(action == ACTION_MOVE)
	{
		
	}

	if(action == ACTION_CANCEL)
	{
		
	}

	pthread_mutex_unlock(&g_mutex);
}

inline long long GetCurrentTimeMillis()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long long time = (long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000;
	return time;
}

void appRender(int width, int height)
{
	int deltaT = GetCurrentTimeMillis() - prevTime;
	prevTime = GetCurrentTimeMillis();

}


void Java_com_drakon_AuroraRenderer_nativeInit(JNIEnv * env, jobject javaThis, jstring apkPath, jstring savePath, jint width, jint height)
{
	sWindowWidth = width;
	sWindowHeight = height;

	const char* str;
	jboolean isCopy;
	str = env->GetStringUTFChars(apkPath, &isCopy);

	

	//appInit(sWindowWidth, sWindowHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	gAppAlive    = 1;
	sDemoStopped = 0;
	sTimeOffsetInit = 0;
}

void Java_com_drakon_AuroraRenderer_nativeResize(JNIEnv* env, jobject javaThis,jint w, jint h )
{
	sWindowWidth = w;
	sWindowHeight = h;
}

void Java_com_drakon_AuroraRenderer_nativeRender(JNIEnv* env, jobject javaThis)
{
	pthread_mutex_lock(&g_mutex);

	if(g_renderEnabled)
		appRender(sWindowWidth, sWindowHeight);

	pthread_mutex_unlock(&g_mutex);
}
