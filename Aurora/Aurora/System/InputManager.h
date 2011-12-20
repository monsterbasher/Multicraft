#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <Aurora/System/Keyboard.h>
#include <Aurora/System/Mouse.h>
#include <Aurora/System/Joypad.h>

namespace Aurora
{
	namespace System
	{
		class InputManager
		{
		private:

		static InputManager *_inputManager;
		static Mouse* _mouse;
		static Keyboard* _keyboard;
		static Joypad _joyPads[4];

		public:

			static InputManager* Instance();

			InputManager();

			Mouse* getMouse();
			Keyboard* getKeyboard();


			void createJoypad(int number);
			Joypad* getJoyPad(int number);

			int getJoyPadsNumber();
		};
	}
}

#endif

