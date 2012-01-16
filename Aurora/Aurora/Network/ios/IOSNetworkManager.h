#ifndef IOSNetworkManager_H
#define IOSNetworkManager_H

#include <Aurora/Network/NetworkManager.h>

namespace Aurora
{
	namespace Network
	{
		class  IOSNetworkManager : public NetworkManager
		{
		public:

			IOSNetworkManager();

			bool Init();
		};
	}
}

#endif