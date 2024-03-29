#include <Aurora/Utils/GameLoader.h>
#include <Aurora/Utils/PlatformConfigurator.h>

#ifdef AURORA_PC
#include <Aurora/Utils/pc/WinGameLoader.h>
#endif

#ifdef AURORA_PSP
#include <Aurora/Utils/psp/PspGameLoader.h>
#endif

#ifdef AURORA_IOS
#include <Aurora/Utils/ios/IOSGameLoader.h>
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
            
            #ifdef AURORA_IOS
			_loader = new IOSGameLoader(gameManager);
            #endif

			return _loader;
		}
	}
}

