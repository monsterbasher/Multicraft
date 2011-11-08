#ifndef WINGAMELOADER_H
#define WINGAMELOADER_H

#include <Aurora/Utils/GameLoader.h>

#include <SFML/Window.hpp>

namespace Aurora
{
	namespace Utils
	{
		class WinGameLoader: public GameLoader
		{
		private:

			sf::Window _window;

		public:

			WinGameLoader(GameManager* gameManager);

			void Run();

		};
	}
}

#endif