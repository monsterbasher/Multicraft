#ifndef IOSGAMELOADER_H
#define IOSGAMELOADER_H

#include <Aurora/Utils/GameLoader.h>

namespace Aurora
{
	namespace Utils
	{
		class IOSGameLoader: public GameLoader
		{
		private:

			

		public:

			IOSGameLoader(GameManager* gameManager);

			void Run();

		};
	}
}

#endif