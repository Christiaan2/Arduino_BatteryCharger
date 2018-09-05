// 
// 
// 

#include "Pid.h"

Pid::Pid(double Kp, double Kd, double Ki)
	: Kp(Kp), Kd(Kd), Ki(Ki), prevError(0), prevOutput(0), sumError(0)
{
}

int Pid::calcPidTerm(double setPoint, double curValue)
{
	if (setPoint <= 0.001)
	{
		reset();
		return 0;
	}
	else
	{
		double error = setPoint - curValue;
		sumError = sumError + error;
		double pidTerm = (Kp * error) + (Kd * (error - prevError)) + (Ki * sumError);
		prevError = error;
		int output = constrain(prevOutput + int(pidTerm), 0, 255);
		prevOutput = output;
		return output;
	}
}

void Pid::reset()
{
	prevOutput = 0;
	prevError = 0.0;
	sumError = 0.0;
}