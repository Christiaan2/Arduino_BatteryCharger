// 
// 
// 

#include "BatteryCharger.h"

BatteryCharger::BatteryCharger()
	: timer(FREQUENCY), oscilloscope(BUTTON_PIN), stop(false), time(0), startTime(0), prevSampling_on(false)
{
	pinMode(REDLEDPIN, OUTPUT);
	pinMode(YELLOWLEDPIN, OUTPUT);
	pinMode(GREENLEDPIN, OUTPUT);
	pinMode(PWM_PIN, OUTPUT);
}

void BatteryCharger::initialize()
{
	digitalWrite(YELLOWLEDPIN, HIGH);
	digitalWrite(GREENLEDPIN, LOW);
	digitalWrite(REDLEDPIN, LOW);
	digitalWrite(PWM_PIN, LOW);
	oscilloscope.initializeSerial();
}

void BatteryCharger::run()
{
	oscilloscope.checkButton();

	if (oscilloscope.getSampling_on())
	{
		if (prevSampling_on != oscilloscope.getSampling_on())
		{ // First time since charger is started?
			// Set charge current
			analogWrite(PWM_PIN, int(round(CHARGE_CURRENT*R_CURRENT_SENSING / 5.0*255.0)));
			startTime = millis();
			prevSampling_on = oscilloscope.getSampling_on();
		}
		if (timer.fire())
		{
			int Current_Sensing_Voltage = analogRead(CURRENT_SENSING_PIN);
			int Battery_Voltage = analogRead(BATTERY_PIN);
			int Battery_Temperature = analogRead(TEMP_PIN);
			double Charge_Current = ((Current_Sensing_Voltage / 1023.0 * 5.0) / R_CURRENT_SENSING) + (Battery_Voltage / 1023.0 * 5.0) / BATTERY_VOLTAGE_DIVIDER_1;
			double batVoltage = SUPPLY_VOLTAGE - ((Battery_Voltage / 1023.0 * 5.0) / BATTERY_VOLTAGE_DIVIDER_1 * (BATTERY_VOLTAGE_DIVIDER_1 + BATTERY_VOLTAGE_DIVIDER_2));
			double batTemp = ((Battery_Temperature / 1023.0 * 5.0) - 0.5) * 100.0;

			if (!stop)
			{
				if (millis() - startTime > MAXTIME)
				{
					// Charge cycle completed
					analogWrite(PWM_PIN, 0);
					digitalWrite(GREENLEDPIN, HIGH);
					stop = true;
					time = millis();
				}
				else if (batTemp > MAXTEMP)
				{
					// Battery temperature too high
					analogWrite(PWM_PIN, 0);
					digitalWrite(REDLEDPIN, HIGH);
					stop = true;
					time = millis();
				}
			}

			oscilloscope.setSensorReading(0, int(round(CHARGE_CURRENT*R_CURRENT_SENSING / 5.0*255.0)));
			oscilloscope.setSensorReading(1, Current_Sensing_Voltage);
			oscilloscope.setSensorReading(2, Battery_Voltage);
			oscilloscope.setSensorReading(3, Battery_Temperature);
			oscilloscope.setTime();
			oscilloscope.sendData();
		}
	}
	else
	{
		analogWrite(PWM_PIN, 0);
		prevSampling_on = false;
	}

	if (stop && millis() - time > 40000)
	{
		// Stop monitoring battery after charge is completed
		digitalWrite(YELLOWLEDPIN, LOW);
		prevSampling_on = false;
		oscilloscope.checkButton(Button::longPressed);
		while (true)
		{
		}
	}
}