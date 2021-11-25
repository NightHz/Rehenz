// Timer.cpp
//
// By NightHz
//

#include "Timer.h"
#include <time.h>

#define _DEBUG_UPDATE

static long nowTime = 0;

Timer::Timer()
{
	startTime = 0;
}

Timer::~Timer()
{
}

void Timer::Start()
{
#ifdef _DEBUG_UPDATE
	UpdateNowTime();
#endif

	startTime = nowTime;
}

long Timer::GetPassTime()
{
#ifdef _DEBUG_UPDATE
	UpdateNowTime();
#endif

	if (CLOCKS_PER_SEC == 1000)
		return nowTime - startTime;
	else
		return (long)((nowTime - startTime) / (CLOCKS_PER_SEC / 1000));
}


void Timer::UpdateNowTime()
{
	nowTime = clock();
}


void Timer::WaitTime(long _time)
{
	long time = clock();
	while (clock() - time < _time);
}

