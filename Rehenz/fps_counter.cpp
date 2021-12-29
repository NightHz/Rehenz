#include "fps_counter.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <timeapi.h>

namespace Rehenz
{
	FpsCounter::FpsCounter() : fps{ 0,0 }
	{
		fps_t0 = 0;
		t0 = 0;
		lock_time = 16; // lock 60 fps
		UpdateFpsCallback = nullptr;
	}

	FpsCounter::~FpsCounter()
	{
	}

	void FpsCounter::Present()
	{
		// update fps
		fps[1]++;
		unsigned long t1 = timeGetTime();
		if (t1 - fps_t0 >= 500)
		{
			if (UpdateFpsCallback != nullptr)
				UpdateFpsCallback(fps[0] + fps[1]);
			fps[0] = fps[1];
			fps[1] = 0;
			fps_t0 = t1;
		}
		// lock fps
		while (lock_time >= (timeGetTime() - t0))
			;
		t0 = timeGetTime();
	}

	void FpsCounter::LockFps(uint _fps)
	{
		if (_fps == 0)
			lock_time = 0;
		else
			lock_time = 1000 / _fps;
	}
}