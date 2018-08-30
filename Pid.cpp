// 
// 
// 

#include "Pid.h"

Pid::Pid(double Kp, double Kd)
	: Kp(Kp), Kd(Kd), prevError(0), prevOutput(0)
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
		double pidTerm = (Kp * error) + (Kd * (error - prevError));
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
}