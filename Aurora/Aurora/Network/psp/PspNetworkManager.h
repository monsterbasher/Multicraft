#ifndef NetworkManager_H
#define NetworkManager_H

namespace Aurora
{
	namespace Network
	{
		class NetworkManager
		{
		private:

			bool _connectionActive;

			static NetworkManager *_networkManager;

			NetworkManager();

		public:

			static NetworkManager* Instance();

			bool isConnectionActive();

		public:

			virtual bool Init() = 0;
		};
	}
}

#endif