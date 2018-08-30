// 
// 
// 

#include "Timer.h"

Timer::Timer(int frequency)
	: frequency(frequency)
{
	prevTime = millis();
}

bool Timer::fire()
{
	unsigned long curTime = millis();
	if (int(curTime - prevTime) >= frequency)
	{
		prevTime = curTime;
		return true;
	}
	else
	{
		return false;
	}
}

void Timer::setTimer()
{
	prevTime = millis();
}

void Timer::setTimer(int frequency)
{
	this->frequency = frequency;
	prevTime = millis();
}

Timer::~Timer()
{
}

