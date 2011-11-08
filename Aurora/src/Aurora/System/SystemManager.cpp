#include <Aurora/System/SystemManager.h>
#include <Aurora/Utils/PlatformConfigurator.h>

#ifdef AURORA_PC
#include <Aurora/System/pc/PcSystemManager.h>
#endif

#ifdef AURORA_PSP
#include <Aurora/System/psp/PspSystemManager.h>
#endif

namespace Aurora
{
	namespace System
	{
		SystemManager* SystemManager::_systemManager = 0;

		SystemManager* SystemManager::Instance()
		{
			if(_systemManager == 0)
			{
#ifdef AURORA_PC
				_systemManager = new PcSystemManager();
#endif

#ifdef AURORA_PSP
				_systemManager = new PspSystemManager();
#endif

			}

			return _systemManager;
		}

		SystemManager::SystemManager()
		{
			_platformType = Pc;
		}

		bool SystemManager::isPlatformPC()
		{
			return _platformType == Pc;
		}

		bool SystemManager::isPlatformPSP()
		{
			return _platformType == Psp;
		}

		bool SystemManager::isPlatformPS3()
		{
			return _platformType == Ps3;
		}

		PlatformType SystemManager::getPlatformType()
		{
			return _platformType;
		}

		void SystemManager::setPlatformType(PlatformType type)
		{
			_platformType = type;
		}
	}
}
