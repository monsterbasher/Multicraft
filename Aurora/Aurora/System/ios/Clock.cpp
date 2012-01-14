#include <Aurora/System/Clock.h>

namespace Aurora
{
	namespace System
	{
		

		Clock::Clock()
		{
			Reset();
		}

		void Clock::Reset()
		{
			//_clock.Reset();
		}

		float Clock::getTime()
		{
			return 0.0f;//_clock.GetElapsedTime();
		}
	}
}
