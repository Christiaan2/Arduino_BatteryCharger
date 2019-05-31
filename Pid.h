// Pid.h

#ifndef PID_H
#define PID_H
#include "Arduino.h"

class Pid
{
private:
	double Kp;
	double Kd;
	double Ki;
	double prevError;
	int prevOutput;
	double sumError;
public:
	Pid(double Kp, double Kd, double Ki);
	int calcPidTerm(double setPoint, double curValue);
	void reset();
};


#endif