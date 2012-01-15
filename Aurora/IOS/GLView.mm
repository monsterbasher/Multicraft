#import "GLView.h"

#include <Aurora/Graphics/ios/OpenGLES1RenderManager.h>
#include <Aurora/Utils/GameLoader.h>
#include <Aurora/Utils/GameManager.h>

#include "Tests/ExampleGame.h"
#include "Tests/Demo_ObjLoading.h"
#include "Tests/SimpleTest.h"

class ExampleGameManager : public GameManager
{
private:
    
	Demo_ObjLoading* exampleState;
    
public:
    
	void Configure()
	{
		//init render manager properties
		RenderManager::Instance()->setSesize(320,480);
	}
    
	void Init()
	{
		//init whatever you need
		exampleState = new Demo_ObjLoading();
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

#define GL_RENDERBUFFER 0x8d41

@implementation GLView

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

void setupAppDirectory()
{
	const int BUFFER_SIZE = 2048;
	
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
	
	char* bundlePath = new char[BUFFER_SIZE];
	
	CFURLGetFileSystemRepresentation(mainBundleURL, false, (UInt8*)  bundlePath, BUFFER_SIZE - 1);
	chdir(bundlePath);
	delete[] bundlePath;
}

- (id) initWithFrame: (CGRect) frame
{
    if (self = [super initWithFrame:frame])
    {
        CAEAGLLayer* eaglLayer = (CAEAGLLayer*) self.layer;
        eaglLayer.opaque = YES;
        
        //change active directory to where my app is
        setupAppDirectory();

        //EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
        //m_context = [[EAGLContext alloc] initWithAPI:api];
        
        EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES1;
        m_context = [[EAGLContext alloc] initWithAPI:api];
        
        if (!m_context || ![EAGLContext setCurrentContext:m_context]) {
            [self release];
            return nil;
        }
        
        if (api == kEAGLRenderingAPIOpenGLES1)
        {
            NSLog(@"Using OpenGL ES 1.1");
            //m_renderingEngine = CreateRenderer1();
        }/* else
        {
            NSLog(@"Using OpenGL ES 2.0");
           // m_renderingEngine = CreateRenderer2();
        }*/
        
        //init game stuff
        _gmanager = new ExampleGameManager();
        
        _gmanager->Configure();
        
        Aurora::Graphics::RenderManager::Instance()->Init();
        
        OpenGLES1RenderManager::setDeviceOrientation(DeviceOrientationPortrait);
        
        _gmanager->Init();
        
        [m_context
            renderbufferStorage:GL_RENDERBUFFER
            fromDrawable: eaglLayer];
        
        //m_renderingEngine->Initialize(CGRectGetWidth(frame), CGRectGetHeight(frame));
        
        [self drawView: nil];
        m_timestamp = CACurrentMediaTime();
        
        CADisplayLink* displayLink;
        displayLink = [CADisplayLink displayLinkWithTarget:self
                                                  selector:@selector(drawView:)];
        
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop]
                          forMode:NSDefaultRunLoopMode];
        
        [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
        
        [[NSNotificationCenter defaultCenter]
         addObserver:self
         selector:@selector(didRotate:)
         name:UIDeviceOrientationDidChangeNotification
         object:nil];
    }
    return self;
}

- (void) didRotate: (NSNotification*) notification
{	
    UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
    
    OpenGLES1RenderManager::setDeviceOrientation((DeviceOrientation) orientation);

    [self drawView: nil];
}

- (void) drawView: (CADisplayLink*) displayLink
{
    if (displayLink != nil)
    {
        float elapsedSeconds = displayLink.timestamp - m_timestamp;
        m_timestamp = displayLink.timestamp;
        //m_renderingEngine->UpdateAnimation(elapsedSeconds);
    }
    
    if (_gmanager != NULL) 
    {
        _gmanager->HandleEvents();
        _gmanager->Update();
        _gmanager->Draw();
    }
    
    [m_context presentRenderbuffer:GL_RENDERBUFFER];
}

- (void) touchesBegan: (NSSet*) touches withEvent: (UIEvent*) event
{
    UITouch* touch = [touches anyObject];
    CGPoint location  = [touch locationInView: self];
    //m_renderingEngine->OnFingerDown(ivec2(location.x, location.y));
}

- (void) touchesEnded: (NSSet*) touches withEvent: (UIEvent*) event
{
    UITouch* touch = [touches anyObject];
    CGPoint location  = [touch locationInView: self];
    //m_renderingEngine->OnFingerUp(ivec2(location.x, location.y));
}

- (void) touchesMoved: (NSSet*) touches withEvent: (UIEvent*) event
{
    UITouch* touch = [touches anyObject];
    CGPoint previous  = [touch previousLocationInView: self];
    CGPoint current = [touch locationInView: self];
    //m_renderingEngine->OnFingerMove(ivec2(previous.x, previous.y),
    //                                ivec2(current.x, current.y));
}

@end
