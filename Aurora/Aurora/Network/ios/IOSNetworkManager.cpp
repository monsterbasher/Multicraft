#include <Aurora/Network/ios/IOSNetworkManager.h>

namespace Aurora
{
	namespace Network
	{
		IOSNetworkManager::IOSNetworkManager()
		{
			_connectionActive = true;
		}

		bool IOSNetworkManager::Init()
		{
			return true;
		}
	}
}

