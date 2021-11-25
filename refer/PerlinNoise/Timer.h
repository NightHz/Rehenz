// Timer.h
//
// By NightHz
//
// timer, and you need use UpdateNowTime first, then you can rightly use GetPassTime() and Start().
//

#ifndef _TIMER_H_
#define _TIMER_H_


class Timer
{
private:
	long startTime;
	
public:
	Timer();
	~Timer();

	void Start();
	long GetPassTime();

	static void UpdateNowTime();

	static void WaitTime(long _time);
};


#endif // !_TIMER_H_

