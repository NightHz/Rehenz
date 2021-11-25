#pragma once
class CTimer
{
private:
	long startTime;
	long lastTime;
	
public:
	CTimer();
	~CTimer();

	void Start();
	void Updata();
	long GetPassTime(long *_pSumTime = nullptr, bool updata = true);
	long WaitPassTime(long _passTime, long *_pSumTime = nullptr);
};

