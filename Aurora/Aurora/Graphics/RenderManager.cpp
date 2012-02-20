#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Utils/PlatformConfigurator.h>

#ifdef AURORA_PC
#include <Aurora/Graphics/opengl/OpenGLRenderManager.h>
#endif

#ifdef AURORA_PSP
#include <Aurora/Graphics/gu/GuRenderManager.h>
#endif

#ifdef AURORA_IOS
#include <Aurora/Graphics/opengl/OpenGLES1RenderManager.h>
#endif

#ifdef AURORA_ANDROID
#include <Aurora/Graphics/opengl/OpenGLES1RenderManager.h>
#endif

#ifdef AURORA_PS3
#include <Aurora/Graphics/opengl/PSGLRenderManager.h>
#endif


namespace Aurora
{
	namespace Graphics
	{
		RenderManager* RenderManager::_renderManager = 0;

		RenderManager* RenderManager::Instance()
		{
			if(_renderManager == 0)
			{
#ifdef AURORA_PC
				_renderManager = new OpengGLRenderManager();
#endif

#ifdef AURORA_PSP
				_renderManager = new GuRenderManager();
#endif
                
#ifdef AURORA_IOS
				_renderManager = new OpenGLES1RenderManager();
#endif

#ifdef AURORA_ANDROID
				_renderManager = new OpenGLES1RenderManager();
#endif

#ifdef AURORA_PS3
				_renderManager = new PSGLRenderManager();
#endif

			}

			return _renderManager;
		}

		RenderManager::RenderManager()
		{
			_currentTexture = -1;
			_currentCam = 0;
		}

		void RenderManager::setPov(float pov)
		{
			_pov = pov;
		}

		void RenderManager::setZminMax(float zmin,float zmax)
		{
			_zMin = zmin;
			_zMax = zmax;
		}

		void RenderManager::setOrthoZminMax(float zmin,float zmax)
		{
			_zOtrhoMin = zmin;
			_zOtrhoMax = zmax;
		}

		void RenderManager::setSesize(int width,int height)
		{
			_width = width;
			_height = height;
		}

		void RenderManager::setFulscreen(bool state)
		{
			_fullScreen = state;
		}

		void RenderManager::setVSync(bool state)
		{
			_vSync = state;
		}

		void RenderManager::setCurrentCam(Camera *cam)
		{
			_currentCam = cam;
		}

		void RenderManager::drawSpriteAnimation(SpriteAnimation* spriteAnimation)
		{
			if(spriteAnimation->_framesCount > 0)
			{
				if(spriteAnimation->_currentFrame < spriteAnimation->_framesCount)
					drawSprite(spriteAnimation->_spriteFrames[spriteAnimation->_currentFrame],spriteAnimation->_position.x,spriteAnimation->_position.x);
				
			}
		}
	}
}
