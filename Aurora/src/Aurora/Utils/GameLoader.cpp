#include <Aurora/Utils/GameLoader.h>
#include <Aurora/Utils/PlatformConfigurator.h>

#ifdef AURORA_PC
#include <Aurora/Utils/win/WinGameLoader.h>
#endif

#ifdef AURORA_PSP
#include <Aurora/Utils/psp/PspGameLoader.h>
#endif

namespace Aurora
{
	namespace Utils
	{
		GameLoader::GameLoader(GameManager* gameManager)
		{
			_gameManager = gameManager;
		}

		GameLoader* GameLoader::getGameLoader(GameManager* gameManager)
		{
			GameLoader* _loader = 0;

			#ifdef AURORA_PC
			_loader = new WinGameLoader(gameManager);
			#endif

			#ifdef AURORA_PSP
			_loader = new PspGameLoader(gameManager);
			#endif

			return _loader;
		}
	}
}

