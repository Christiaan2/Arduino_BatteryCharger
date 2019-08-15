// BatteryCharger.h

#ifndef BATTERYCHARGER_H
#define BATTERYCHARGER_H
#include "Arduino.h"
#include "Timer.h"
#include "Oscilloscope.h"

// Connections
#define CURRENT_SENSING_PIN A0
#define TEMP_PIN A1
#define BATTERY_PIN A2
#define BUTTON_PIN 2
#define PWM_PIN 6
#define GREENLEDPIN 3
#define REDLEDPIN 4
#define YELLOWLEDPIN 5

// Settings
#define MAXTEMP 40.0 // in graden Celsius
#define R_CURRENT_SENSING 6.8 // in Ohm
#define BATTERY_VOLTAGE_DIVIDER_1 21000.0
#define BATTERY_VOLTAGE_DIVIDER_2 98000.0
#define CHARGE_CURRENT 0.32 //in A
#define SUPPLY_VOLTAGE 24.0
#define MAXTIME 10800000 // in ms
#define FREQUENCY 2000 // frequency run method is called in ms

class BatteryCharger
{
private:
	Timer timer;
	Oscilloscope oscilloscope;
	bool stop;
	unsigned long time;
	unsigned long startTime;
	bool prevSampling_on;

 public:
	 BatteryCharger();
	 void initialize();

	 void run();
};

#endif