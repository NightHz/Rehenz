#pragma once
#include "type.h"
#include <functional>

namespace Rehenz
{
	class FpsCounter
	{
	private:
		using TimeType = unsigned long;

		uint fps[2];
		TimeType fps_t0;
		TimeType t0;
		TimeType lock_time;

	public:
		std::function<void(uint fps)> UpdateFpsCallback;

		// default lock 60 fps
		FpsCounter();
		~FpsCounter();

		void Present();
		// input 0 to unlock fps
		void LockFps(uint fps);
	};
}