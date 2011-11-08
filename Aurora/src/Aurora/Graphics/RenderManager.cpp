#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Utils/PlatformConfigurator.h>

#ifdef AURORA_PC
#include <Aurora/Graphics/win/OpenGLRenderManager.h>
#endif

#ifdef AURORA_PSP
#include <Aurora/Graphics/psp/GuRenderManager.h>
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

			}

			return _renderManager;
		}

		RenderManager::RenderManager()
		{
			_currentTexture = -1;
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
	}
}
