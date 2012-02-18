#include <Aurora/System/Clock.h>

#include <sys/time.h>

namespace Aurora
{
	namespace System
	{
		long lastMilis;
        timeval time;

		Clock::Clock()
		{
			gettimeofday(&time, NULL);
            lastMilis = (time.tv_sec * 1000) + (time.tv_usec / 1000);
		}

		void Clock::Reset()
		{
			//_clock.Reset();
		}

		float Clock::getTime()
		{
            gettimeofday(&time, NULL);
            long miliseconds = (time.tv_sec * 1000) + (time.tv_usec / 1000);
            
            lastTime = (miliseconds - lastMilis)/ 1000.0f;
            
            lastMilis = miliseconds;
            
			return lastTime;//0.015f;//_clock.GetElapsedTime();
		}
	}
}
