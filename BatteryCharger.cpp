// 
// 
// 

#include "BatteryCharger.h"

BatteryCharger::BatteryCharger()
	: timer(5000), oscilloscope(2), pid(650.0, 320.0), stop(false), time(0)
{
	pinMode(REDLEDPIN, OUTPUT);
	pinMode(YELLOWLEDPIN, OUTPUT);
	pinMode(GREENLEDPIN, OUTPUT);
	pinMode(PWM_PIN, OUTPUT);
}

void BatteryCharger::initialize()
{
	digitalWrite(YELLOWLEDPIN, HIGH);
	digitalWrite(PWM_PIN, LOW);
	oscilloscope.initializeSerial();
}

void BatteryCharger::run()
{
	oscilloscope.checkButton();

	if (timer.fire() && oscilloscope.getSampling_on())
	{
		int V_INPUTbin = analogRead(V_INPUT_PIN);
		int V_BASEbin = analogRead(V_BASE_PIN);
		int V_EMITTERbin = analogRead(V_EMITTER_PIN);
		int V_BATTERYbin = analogRead(V_BATTERY_PIN);
		int V_batTemp = analogRead(V_TEMP_PIN);
		double inputVoltage = (V_INPUTbin / 1024.0) * 5.0;
		//double baseCurrent = (inputVoltage - ((V_BASEbin / 1024.0) * 5.0)) / R_BASE; //Not baseCurrent but current through 2.2k resistor
		double collectorCurrent = (((V_EMITTERbin / 1024.0) * 5.0) / R_EMITTER) + ((V_BATTERYbin * 5.55) / (98000 + 21000));
		//double batVoltage = SUPPLY_VOLTAGE - (((V_BATTERYbin / 1024.0) * 5.0) * 5.55);
		double batTemp = (((V_batTemp / 1024.0) * 5.0) - 0.5) * 100.0;

		int PWM_value = 0;

		if (!stop)
		{
			if (millis() > MAXTIME)
			{
				digitalWrite(GREENLEDPIN, HIGH);
				stop = true;
				time = millis();
			}
			else if (batTemp > MAXTEMP)
			{
				digitalWrite(REDLEDPIN, HIGH);
				stop = true;
				time = millis();
			}
			else
			{
				PWM_value = pid.calcPidTerm(CHARGE_CURRENT, collectorCurrent);
			}
			analogWrite(PWM_PIN, PWM_value);
		}

		oscilloscope.setSensorReading(0, V_INPUTbin);
		oscilloscope.setSensorReading(1, V_BASEbin);
		oscilloscope.setSensorReading(2, PWM_value);
		oscilloscope.setSensorReading(3, V_EMITTERbin);
		oscilloscope.setSensorReading(4, V_BATTERYbin);
		oscilloscope.setSensorReading(5, V_batTemp);
		oscilloscope.setTime();
		oscilloscope.sendData();
	}

	if (stop && time > 0 && millis() - time > 40000)
	{
		digitalWrite(YELLOWLEDPIN, LOW);
		oscilloscope.checkButton(Button::longPressed);
		while (true)
		{
		}
	}
}