#include <Aurora/System/ios/IOSSystemManager.h>


namespace Aurora
{
	namespace System
	{
			IOSSystemManager::IOSSystemManager() : SystemManager()
			{
				setPlatformType(Pc);
			}

			void IOSSystemManager::Update()
			{
				
			}

			bool IOSSystemManager::keyPressed(Key::Code keyCode)
			{
				bool temp = false;
				if(_keyStates[keyCode] == true || temp == false)
				{
					return false;
				}else
				{
					_keyStates[keyCode] = true;
					return true;
				}
			}

			bool IOSSystemManager::keyHold(Key::Code keyCode)
			{
				return 0;
			}

			bool IOSSystemManager::mouseButtonDown(Mouse::Button buttonNumber)
			{
				return 0;
			}

			int IOSSystemManager::getMouseX()
			{
				return 0;
			}

			int IOSSystemManager::getMouseY()
			{
				return 0;
			}

			float IOSSystemManager::getAnalogX()
			{
				return 0.0f;
			}

			float IOSSystemManager::getAlanogY()
			{
				return 0.0f;
			}
	}
}
