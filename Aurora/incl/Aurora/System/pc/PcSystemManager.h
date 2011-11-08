#ifndef PCSYSTEMManager_H
#define PCSYSTEMManager_H

#include <Aurora/System/SystemManager.h>
#include <SFML/Window/Input.hpp>

namespace Aurora
{
	namespace System
	{
		class PcSystemManager : public SystemManager
		{
		private:

			bool _keyStates[Key::Code::Count];

		public:

			sf::Input* _input;

			PcSystemManager();

			void Update();

			bool keyPressed(Key::Code keyCode);
			bool keyHold(Key::Code keyCode);

			bool mouseButtonDown(Mouse::Button buttonNumber);

			int getMouseX();
			int getMouseY();

			float getAnalogX();
			float getAlanogY();

		};

	}
}

#endif
