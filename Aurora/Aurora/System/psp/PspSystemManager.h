#ifndef PCSYSTEMManager_H
#define PCSYSTEMManager_H

#include <Aurora/System/SystemManager.h>

#include <pspkernel.h>
#include <pspctrl.h>
#include <psputility.h>

namespace Aurora
{
	namespace System
	{
		class PspSystemManager : public SystemManager
		{
		private:
		
			//input
			SceCtrlData oldPadData,newPadData;
			
		private:
		
			PspCtrlButtons keyToPspButton(Key::Code keyCode);

		public:

			PspSystemManager();

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
