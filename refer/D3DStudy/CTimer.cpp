#include "stdafx.h"
#include <time.h>
#include "CTimer.h"


CTimer::CTimer()
{
}

CTimer::~CTimer()
{
}

void CTimer::Start()
{
	startTime = clock();
	lastTime = startTime;
}

void CTimer::Updata()
{
	lastTime = clock();
}

long CTimer::GetPassTime(long *_pSumTime, bool updata)
{
	long nowTime = clock();
	long passTime = nowTime - lastTime;

	if (_pSumTime)
		*_pSumTime = nowTime - startTime;

	if (updata)
		Updata();
	return passTime;
}

long CTimer::WaitPassTime(long _passTime, long *_pSumTime)
{
	while ((clock() - lastTime) < _passTime)
		;

	return GetPassTime(_pSumTime);
}
