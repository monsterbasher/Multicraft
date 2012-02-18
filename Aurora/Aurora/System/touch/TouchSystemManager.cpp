#include <Aurora/System/touch/TouchSystemManager.h>
#include <Aurora/Graphics/opengl/OpenGLES1RenderManager.h>

namespace Aurora
{
	namespace System
	{
			TouchSystemManager::TouchSystemManager() : SystemManager()
			{
				#ifdef AURORA_ANDROID
					setPlatformType(AndroidType);
				#else
					setPlatformType(IosType);
				#endif
                
                movePad = false;
                rotatePad = false;
                virtualPads = true;
			}

			void TouchSystemManager::Update()
			{
                //reset all keys
                for(int i = 0;i < Key::Count;i++)
				{
                    _keyStates[(Key::Code)i] = false;
				}
                
				if (virtualPads)
                {
                    if (movePad)
                    {
                        for (int i = 0; i < _touches.size(); i++)
                        {
                            if (_touches[i].hash == moveHash)
                            {
                                Math::Vector2 lenghtVector = Math::Vector2(_touches[i].positionX,_touches[i].positionY) - movePoint;
                                movePadPos.x = lenghtVector.x / 30.0f;
                                movePadPos.y = -lenghtVector.y / 30.0f;
                            }
                        }
                        
                        if (movePadPos.x > 0.5f)
                        {
                            _keyStates[Key::D] = true;
                        }
                        if (movePadPos.x < -0.5f)
                        {
                            _keyStates[Key::A] = true;
                        }
                        
                        if (movePadPos.y > 0.5f)
                        {
                            _keyStates[Key::W] = true;
                        }
                        if (movePadPos.y < -0.5f)
                        {
                            _keyStates[Key::S] = true;
                        }
                    }
                    
                    if (rotatePad)
                    {
                        for (int i = 0; i < _touches.size(); i++)
                        {
                            if (_touches[i].hash == rotateHash)
                            {
                                Math::Vector2 lenghtVector = Math::Vector2(_touches[i].positionX,_touches[i].positionY) - rotatePoint;
                                rotatePadPos.x = lenghtVector.x / 30.0f;
                                rotatePadPos.y = -lenghtVector.y / 30.0f;
                            }
                        }
                        
                        if (rotatePadPos.x > 0.5f)
                        {
                            _keyStates[Key::Right] = true;
                        }
                        if (rotatePadPos.x < -0.5f)
                        {
                            _keyStates[Key::Left] = true;
                        }
                        
                        if (rotatePadPos.y > 0.5f)
                        {
                            _keyStates[Key::Up] = true;
                        }
                        if (rotatePadPos.y < -0.5f)
                        {
                            _keyStates[Key::Down] = true;
                        }
                    }
                }
			}

			bool TouchSystemManager::keyPressed(Key::Code keyCode)
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

			bool TouchSystemManager::keyHold(Key::Code keyCode)
			{
				return _keyStates[keyCode];
			}

			bool TouchSystemManager::mouseButtonDown(Mouse::Button buttonNumber)
			{
				return 0;
			}

			int TouchSystemManager::getMouseX()
			{
                if (_touches.size() > 0)
                {
                    return _touches[0].positionX;
                }
                
				return 0;
			}

			int TouchSystemManager::getMouseY()
			{
				if (_touches.size() > 0)
                {
                    return _touches[0].positionY;
                }
                
                return 0;
			}

			float TouchSystemManager::getAnalogX()
			{
                if (movePad)
                {
                    return movePadPos.x;
                }
                
				return 0.0f;
			}

			float TouchSystemManager::getAlanogY()
			{
                if (movePad)
                {
                    return movePadPos.y;
                }
                
				return 0.0f;
			}
        
        void TouchSystemManager::TouchesBegan(int hash,float x,float y)
        {
            
            for (int i = 0; i < _touches.size(); i++)
            {
                if (_touches[i].hash == hash)
                {
                    return;
                }
            }
            
            Touch newTouch;
            newTouch.hash = hash;
            
            x -= Graphics::RenderManager::Instance()->getWidth() / 2;
            y -= Graphics::RenderManager::Instance()->getHeight() / 2;
            
            
            float posx = x;
            float posy = y;
            
            x = posx * cos(-Graphics::OpenGLES1RenderManager::deviceRotationAngle / 180 * M_PI) - posy * sin(-Graphics::OpenGLES1RenderManager::deviceRotationAngle / 180 * M_PI);
            y = posx * sin(-Graphics::OpenGLES1RenderManager::deviceRotationAngle / 180 * M_PI) + posy * cos(-Graphics::OpenGLES1RenderManager::deviceRotationAngle / 180 * M_PI);
            
            x += Graphics::RenderManager::Instance()->getWidth() / 2;
            y += Graphics::RenderManager::Instance()->getHeight() / 2;
            
            if (Graphics::OpenGLES1RenderManager::deviceRotationAngle == 90 || Graphics::OpenGLES1RenderManager::deviceRotationAngle == 270)
            {
                float temp = y;
                y = x;
                x = Graphics::RenderManager::Instance()->getHeight() - temp;
                
            }
            
            newTouch.positionX = newTouch.previouspositionX = x;
            newTouch.positionY = newTouch.previouspositionY = y;
            
            _touches.push_back(newTouch);
            
            if (virtualPads)
            {
                int halfWidth;
                if (Graphics::OpenGLES1RenderManager::deviceRotationAngle == 90 || Graphics::OpenGLES1RenderManager::deviceRotationAngle == 270)
                {
                    halfWidth = Graphics::RenderManager::Instance()->getHeight() / 2;
                }else
                {
                    halfWidth = Graphics::RenderManager::Instance()->getWidth() / 2;
                }
                
                //move pad
                if (!movePad)
                {
                    //move pad will be on the left side
                    if (x < halfWidth)
                    {
                        //ok gogo
                        movePad = true;
                        moveHash = hash;
                        movePoint = Math::Vector2(x,y);
                        movePadPos = Math::Vector2(0,0);
                    }
                }
                
                //rotate pad
                if (!rotatePad)
                {
                    //move pad will be on the left side
                    if (x > halfWidth)
                    {
                        //ok gogo
                        rotatePad = true;
                        rotateHash = hash;
                        rotatePoint = Math::Vector2(x,y);
                        rotatePadPos = Math::Vector2(0,0);
                    }
                }
            }
        }
        
        void TouchSystemManager::TouchesMoved(int hash,float x,float y,float lastX,float lastY)
        {
            for (int i = 0; i < _touches.size(); i++)
            {
                if (_touches[i].hash == hash)
                {
                    _touches[i].previouspositionX = _touches[i].positionX;
                    _touches[i].previouspositionY = _touches[i].positionY;
                    
                    x -= Graphics::RenderManager::Instance()->getWidth() / 2;
                    y -= Graphics::RenderManager::Instance()->getHeight() / 2;
                    
                    float posx = x;
                    float posy = y;
                    
                    x = posx * cos(-Graphics::OpenGLES1RenderManager::deviceRotationAngle / 180 * M_PI) - posy * sin(-Graphics::OpenGLES1RenderManager::deviceRotationAngle / 180 * M_PI);
                    y = posx * sin(-Graphics::OpenGLES1RenderManager::deviceRotationAngle / 180 * M_PI) + posy * cos(-Graphics::OpenGLES1RenderManager::deviceRotationAngle / 180 * M_PI);
                    
                    x += Graphics::RenderManager::Instance()->getWidth() / 2;
                    y += Graphics::RenderManager::Instance()->getHeight() / 2;
                    
                     if (Graphics::OpenGLES1RenderManager::deviceRotationAngle == 90 || Graphics::OpenGLES1RenderManager::deviceRotationAngle == 270)
                     {
                         float temp = y;
                         y = x;
                         x = Graphics::RenderManager::Instance()->getHeight() - temp;                         
                     }
                    
                    _touches[i].positionX = x;
                    _touches[i].positionY = y;
                }
            }
        }
        
        void TouchSystemManager::TouchesEnded(int hash,float x,float y)
        {
            int number = 0;
            for (int i = 0; i < _touches.size(); i++)
            {
                if (_touches[i].hash == hash)
                {
                    number = i;
                }
            }
            
            if (_touches.size() > 0)
            {
                _touches.erase(_touches.begin() + number);
            }
            
            
            if (virtualPads)
            {
                if (movePad)
                {
                    if (hash == moveHash)
                    {
                        movePad = false;
                    }
                }
                
                if (rotatePad)
                {
                    if (hash == rotateHash)
                    {
                        rotatePad = false;
                    }
                }
            }
        }
	}
}
