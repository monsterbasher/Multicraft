#include <Aurora/System/pc/PcSystemManager.h>


namespace Aurora
{
	namespace System
	{
			PcSystemManager::PcSystemManager() : SystemManager()
			{
				setPlatformType(PC);
			}

			void PcSystemManager::Update()
			{
				for(int i = 0;i < Key::Count;i++)
				{
					if(!_input->IsKeyDown((sf::Key::Code)i))
						_keyStates[(sf::Key::Code)i] = false;
				}
			}

			bool PcSystemManager::keyPressed(Key::Code keyCode)
			{
				bool temp = _input->IsKeyDown((sf::Key::Code)keyCode);
				if(_keyStates[keyCode] == true || temp == false)
				{
					return false;
				}else
				{
					_keyStates[keyCode] = true;
					return true;
				}
			}

			bool PcSystemManager::keyHold(Key::Code keyCode)
			{
				return _input->IsKeyDown((sf::Key::Code)keyCode);
			}

			bool PcSystemManager::mouseButtonDown(Mouse::Button buttonNumber)
			{
				return _input->IsMouseButtonDown((sf::Mouse::Button)buttonNumber);
			}

			int PcSystemManager::getMouseX()
			{
				return _input->GetMouseX();
			}

			int PcSystemManager::getMouseY()
			{
				return _input->GetMouseY();
			}

			float PcSystemManager::getAnalogX()
			{
				return 0.0f;
			}

			float PcSystemManager::getAlanogY()
			{
				return 0.0f;
			}
	}
}
