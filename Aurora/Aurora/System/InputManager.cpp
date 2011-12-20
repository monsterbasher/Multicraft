#include <Aurora/System/InputManager.h>

namespace Aurora
{
	namespace System
	{
		InputManager* InputManager::_inputManager = 0;
		Mouse* InputManager::_mouse = 0;
		Keyboard* InputManager::_keyboard = 0;

		InputManager::InputManager()
		{

		}

		InputManager* InputManager::Instance()
		{
			if(_inputManager == 0)
			{
				_inputManager = new InputManager();
			}

			return _inputManager;
		}


		Mouse* InputManager::getMouse()
		{
			if(_mouse == 0)
			{
				_mouse = new Mouse();
			}

			return _mouse;
		}

		Keyboard* InputManager::getKeyboard()
		{
			if(_keyboard == 0)
			{
				_keyboard = new Keyboard();
			}

			return _keyboard;
		}

	}
}
