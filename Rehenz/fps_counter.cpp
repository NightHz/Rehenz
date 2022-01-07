#include "fps_counter.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <timeapi.h>

namespace Rehenz
{
	FpsCounter::FpsCounter() : fps{ 0,0,0 }
	{
		fps_t0 = 0;
		t0 = 0;
		t1 = 0;
		lock_time = 16; // lock 60 fps
		UpdateFpsCallback = nullptr;
	}

	FpsCounter::~FpsCounter()
	{
	}

	void FpsCounter::Present()
	{
		// update fps
		fps[2]++;
		TimeType fps_t1 = timeGetTime();
		if (fps_t1 - fps_t0 >= 500)
		{
			fps[0] = fps[1];
			fps[1] = fps[2];
			fps[2] = 0;
			fps_t0 = fps_t1;
			if (UpdateFpsCallback != nullptr)
				UpdateFpsCallback(GetLastFps());
		}
		// lock fps
		TimeType t2;
		while (lock_time > ((t2 = timeGetTime()) - t1))
			;
		t0 = t1;
		t1 = t2;
	}

	uint FpsCounter::GetLastFps()
	{
		return fps[0] + fps[1];
	}

	uint FpsCounter::GetLastDeltatime()
	{
		return static_cast<uint>(t1 - t0);
	}

	void FpsCounter::LockFps(uint _fps)
	{
		if (_fps == 0)
			lock_time = 0;
		else
			lock_time = 1000 / _fps;
	}
}