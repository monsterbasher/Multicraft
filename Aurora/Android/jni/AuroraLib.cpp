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

#include <Aurora/System/touch/TouchSystemManager.h>
#include <Aurora/Graphics/opengl/OpenGLES1RenderManager.h>
#include <Aurora/Utils/GameManager.h>
#include <Aurora/System/FileManager.h>

#include "Tests/SimpleTest.h"

#define STRINGIFY(x) #x
#define LOG_TAG    __FILE__ ":" STRINGIFY(__LINE__)
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

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

//simple test manager
class ExampleGameManager : public GameManager
{
private:

	SimpleTest* exampleState;

public:

	void Configure()
	{
		//init render manager properties
		RenderManager::Instance()->setSesize(320,480);
	}

	void Init()
	{
		//init whatever you need
		exampleState = new SimpleTest();
		exampleState->Init();

		ChangeState(exampleState);
	}

	void CleanUp()
	{
		exampleState->CleanUp();
		delete exampleState;
	}
};

ExampleGameManager *_gmanager;

FileManager* _fileManager;
TouchSystemManager *_imanager;

void Java_com_drakon_AuroraActivity_nativePause(JNIEnv * env, jobject javaThis)
{

}

void Java_com_drakon_AuroraActivity_nativeResume(JNIEnv * env, jobject javaThis)
{

}

void Java_com_drakon_AuroraGLSurfaceView_nativeTouchEvent(JNIEnv * env, jobject javaThis,jint action,jint number,jint x,jint y)
{
	pthread_mutex_lock(&g_mutex);

	int posX = x;
	int posY = y;

	LOGI("Action %d x:%d y:%d",number,x,y);

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

void appRender(int width, int height)
{
	if (_gmanager != NULL)
	{
		_gmanager->HandleEvents();
		_gmanager->Update();
		_gmanager->Draw();
	}
}


void Java_com_drakon_AuroraRenderer_nativeInit(JNIEnv * env, jobject javaThis, jstring apkPath, jstring savePath, jint width, jint height)
{
	sWindowWidth = width;
	sWindowHeight = height;

	const char* str;
	jboolean isCopy;
	str = env->GetStringUTFChars(apkPath, &isCopy);

	
	//initialize game here
	_fileManager = new FileManager(ZipType);
	_fileManager->SetMainFile(str);
	_fileManager->SetMainDirPath("assets/");

	_gmanager = new ExampleGameManager();
	_gmanager->Configure();

	Aurora::Graphics::RenderManager::Instance()->Init();
	Aurora::Graphics::RenderManager::Instance()->setSesize(sWindowWidth,sWindowHeight);
	OpenGLES1RenderManager::setDeviceOrientation(DeviceOrientationPortrait);

	_gmanager->Init();
	_imanager = (Aurora::System::TouchSystemManager*)Aurora::System::SystemManager::Instance();


	//appInit(sWindowWidth, sWindowHeight);
	/*glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);*/

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
