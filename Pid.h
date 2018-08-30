// Pid.h

#ifndef PID_H
#define PID_H
#include "Arduino.h"

class Pid
{
private:
	double Kp;
	double Kd;
	double prevError;
	int prevOutput;
public:
	Pid(double Kp, double Kd);
	int calcPidTerm(double setPoint, double curValue);
	void reset();
};


#endif

