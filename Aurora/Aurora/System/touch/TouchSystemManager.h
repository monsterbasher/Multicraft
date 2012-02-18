#ifndef TOUCHSYSTEMMANAGER_H
#define TOUCHSYSTEMMANAGER_H

#include <Aurora/System/SystemManager.h>
#include <Aurora/Math/Vector2.h>
#include <vector>

using namespace Key;

namespace Aurora
{
	namespace System
	{
        class Touch
        {
        public:
            
            int hash;
            float positionX,positionY,previouspositionX,previouspositionY;
            
        };
        
		class TouchSystemManager : public SystemManager
		{
		private:

			bool _keyStates[Key::Count];
            std::vector<Touch> _touches;
            
            bool virtualPads;
            
            bool movePad;
            int moveHash;
            Math::Vector2 movePoint;
            Math::Vector2 movePadPos;
            
            bool rotatePad;
            int rotateHash;
            Math::Vector2 rotatePoint;
            Math::Vector2 rotatePadPos;

		public:


			TouchSystemManager();

			void Update();

			bool keyPressed(Key::Code keyCode);
			bool keyHold(Key::Code keyCode);

			bool mouseButtonDown(Mouse::Button buttonNumber);

			int getMouseX();
			int getMouseY();

			float getAnalogX();
			float getAlanogY();
            
        public:
            
            void TouchesBegan(int hash,float x,float y);
            void TouchesMoved(int hash,float newX,float newY,float lastX,float lastY);
            void TouchesEnded(int hash,float x,float y);

		};

	}
}

#endif
