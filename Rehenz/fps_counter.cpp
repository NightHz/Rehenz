#include "fps_counter.h"

namespace Rehenz
{
	FpsCounter::FpsCounter(std::function<TimeType(void)> GetTimeFunc, TimeType _frequency)
		: fps{ 0,0,0 }, GetTime(GetTimeFunc), frequency(_frequency)
	{
		fps_t0 = 0;
		t0 = GetTime();
		t1 = t0;
		lock_time = frequency / 60; // lock 60 fps
		UpdateFpsCallback = nullptr;
	}

	FpsCounter::~FpsCounter()
	{
	}

	void FpsCounter::Present()
	{
		// update fps
		fps[2]++;
		TimeType fps_t1 = GetTime();
		if (fps_t1 - fps_t0 >= frequency / 2)
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
		while (lock_time > ((t2 = GetTime()) - t1))
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
		return static_cast<uint>((t1 - t0) * 1000 / frequency);
	}

	float FpsCounter::GetLastDeltatime2()
	{
		return static_cast<float>(static_cast<double>(t1 - t0) / static_cast<double>(frequency));
	}

	double FpsCounter::GetLastDeltatime3()
	{
		return static_cast<double>(t1 - t0) / static_cast<double>(frequency);
	}

	void FpsCounter::LockFps(uint _fps)
	{
		if (_fps == 0)
			lock_time = 0;
		else
			lock_time = frequency / _fps;
	}
}