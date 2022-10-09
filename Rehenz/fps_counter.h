#pragma once
#include "type.h"

namespace Rehenz
{
	class FpsCounter
	{
	private:
		using TimeType = ullong;

		// save frames per 0.5s
		uint fps[3];
		TimeType fps_t0; // fps[2] start time
		TimeType t0, t1; // present time
		TimeType lock_time;
		
		std::function<TimeType(void)> GetTime;
		TimeType frequency;

	public:
		// update per 0.5s
		std::function<void(uint fps)> UpdateFpsCallback;

		// default lock 60 fps
		FpsCounter(std::function<TimeType(void)> GetTimeFunc, TimeType _frequency = 1000);
		~FpsCounter();

		void Present();
		uint GetLastFps();
		// ms unit
		uint GetLastDeltatime();
		// s unit
		float GetLastDeltatime2();
		// input 0 to unlock fps
		void LockFps(uint fps);
	};
}