// BatteryCharger.h

#ifndef BATTERYCHARGER_H
#define BATTERYCHARGER_H
#include "Arduino.h"
#include "Pid.h"
#include "Timer.h"
#include "Oscilloscope.h"

#define MAXTEMP 50.0 //Graden Celsius
#define R_EMITTER 21.8
#define R_BASE 2160.0
#define CHARGE_CURRENT 0.07
#define SUPPLY_VOLTAGE 12.10
#define V_INPUT_PIN A0
#define V_BASE_PIN A1
#define V_BATTERY_PIN A2
#define V_EMITTER_PIN A3
#define V_TEMP_PIN A4
#define PWM_PIN 3
#define MAXTIME 57600000
#define REDLEDPIN 4
#define YELLOWLEDPIN 5
#define GREENLEDPIN 6

class BatteryCharger
{
private:
	Pid pid;
	Timer timer;
	Oscilloscope oscilloscope;
	bool stop;
	unsigned long time;

 public:
	 BatteryCharger();
	 void initialize();

	 void run();
};

#endif

