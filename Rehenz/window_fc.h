#pragma once
#include "window.h"
#include "fps_counter.h"

namespace Rehenz
{
	class FpsCounterS : public FpsCounter
	{
	public:
		FpsCounterS();
		FpsCounterS(const FpsCounterS&) = delete;
		FpsCounterS& operator=(const FpsCounterS&) = delete;
		~FpsCounterS();
	};

	class SimpleWindowWithFC : public SimpleWindow
	{
	private:
	public:
		String title_base;
		FpsCounterS fps_counter;

		SimpleWindowWithFC(HINSTANCE _hinstance, int _width, int _height, String _title_base);
		SimpleWindowWithFC(const SimpleWindowWithFC&) = delete;
		SimpleWindowWithFC& operator=(const SimpleWindowWithFC&) = delete;
		~SimpleWindowWithFC();

		void Present();
	};
}