#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

namespace Aurora
{
	namespace System
	{
		class InputManager
		{
		private:

		static InputManager *_inputManager;

		public:

			static InputManager* Instance();

			InputManager();

		};
	}
}

#endif

