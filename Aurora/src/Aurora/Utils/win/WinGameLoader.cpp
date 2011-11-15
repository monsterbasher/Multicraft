#include <Aurora/Utils/win/WinGameLoader.h>
#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/System/SystemManager.h>
#include <Aurora/System/pc/PcSystemManager.h>

namespace Aurora
{
	namespace Utils
	{
		WinGameLoader::WinGameLoader(GameManager* gameManager):GameLoader(gameManager)
		{

		}

		void WinGameLoader::Run()
		{
			_gameManager->Configure();

			_window.Create(sf::VideoMode(Aurora::Graphics::RenderManager::Instance()->getWidth(), Aurora::Graphics::RenderManager::Instance()->getHeight(), 32), "Starter");
			_window.UseVerticalSync(Aurora::Graphics::RenderManager::Instance()->getVsync());

			Aurora::Graphics::RenderManager::Instance()->Init();
			((Aurora::System::PcSystemManager*)Aurora::System::SystemManager::Instance())->_input = (sf::Input*)&_window.GetInput();

			_gameManager->Init();

			while (_window.IsOpened())
			{
				sf::Event Event;
				while (_window.GetEvent(Event))
				{
					if (Event.Type == sf::Event::Closed)
						_window.Close();
				}

				_gameManager->HandleEvents();
				_gameManager->Update();
				_gameManager->Draw();

				_window.Display();
			}

			_gameManager->CleanUp();
		}

	}
}
