// 
// 
// 

#include "Pid.h"

Pid::Pid(double Kp, double Kd, double Ki)
	: Kp(Kp), Kd(Kd), Ki(Ki), prevError(0.0), prevPidTerm(0.0), sumError(0.0)
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
		if (abs(error) < 0.0015)
		{
			return int(round(prevPidTerm));
		}
		sumError = sumError + error;
		double pidTerm = (Kp * error) + (Kd * (error - prevError)) + (Ki * sumError);
		prevError = error;
		prevPidTerm = constrain(prevPidTerm + pidTerm, 0.0, 255.0);
		return int(round(prevPidTerm));
	}
}

void Pid::reset()
{
	prevPidTerm = 0.0;
	prevError = 0.0;
	sumError = 0.0;
}