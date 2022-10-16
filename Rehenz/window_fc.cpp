#include "window_fc.h"
#define WIN32_LEAN_AND_MEAN
#include <profileapi.h>
#include <utility>

namespace Rehenz
{
	inline ullong QueryPerformanceGetTime()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		return li.QuadPart;
	}

	inline ullong QueryPerformanceGetFrequency()
	{
		LARGE_INTEGER li;
		QueryPerformanceFrequency(&li);
		return li.QuadPart;
	}

	FpsCounterS::FpsCounterS() : FpsCounter(QueryPerformanceGetTime, QueryPerformanceGetFrequency())
	{
	}

	FpsCounterS::~FpsCounterS()
	{
	}

	SimpleWindowWithFC::SimpleWindowWithFC(HINSTANCE _hinstance, int _width, int _height, String _title_base)
		: SimpleWindow(_hinstance, _width, _height, _title_base), title_base(std::move(_title_base)), fps_counter()
	{
		fps_counter.LockFps(100);
		auto updateFps = [this](Rehenz::uint fps)
		{
			this->SetTitle(this->title_base + TEXT(" fps:") + ToString(fps));
		};
		fps_counter.UpdateFpsCallback = updateFps;
	}

	SimpleWindowWithFC::~SimpleWindowWithFC()
	{
	}

	void SimpleWindowWithFC::Present()
	{
		if (GetHwnd())
			fps_counter.Present();
	}

}