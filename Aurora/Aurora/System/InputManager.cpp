#include <Aurora/System/InputManager.h>

namespace Aurora
{
	namespace System
	{
		InputManager* InputManager::_inputManager = 0;

		InputManager* InputManager::Instance()
		{
			if(_inputManager == 0)
			{

			}

			return _inputManager;
		}
	}
}
