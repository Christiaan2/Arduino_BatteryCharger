// Timer.h

#ifndef TIMER_H
#define TIMER_H
#include "Arduino.h"

class Timer
{
private:
	unsigned long prevTime;
	int frequency; // in ms


public:
	Timer(int frequency);
	bool fire();
	void setTimer();
	void setTimer(int frequency);
	~Timer();
};

#endif

