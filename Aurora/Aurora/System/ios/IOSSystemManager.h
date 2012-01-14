#ifndef IOSSYSTEMManager_H
#define IOSSYSTEMManager_H

#include <Aurora/System/SystemManager.h>

using namespace Key;

namespace Aurora
{
	namespace System
	{
		class IOSSystemManager : public SystemManager
		{
		private:

			bool _keyStates[Key::Count];

		public:


			IOSSystemManager();

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
