#include <Aurora/Utils/ios/IOSGameLoader.h>
#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/System/SystemManager.h>

namespace Aurora
{
	namespace Utils
	{
        //Warning - as for now this class should not be really used in IOS project...
		IOSGameLoader::IOSGameLoader(GameManager* gameManager):GameLoader(gameManager)
		{

		}

		void IOSGameLoader::Run()
		{
			_gameManager->Configure();

			Aurora::Graphics::RenderManager::Instance()->Init();

			_gameManager->Init();

			while (true)
			{

				_gameManager->HandleEvents();
				_gameManager->Update();
				_gameManager->Draw();
			}

			_gameManager->CleanUp();
		}

	}
}
