// 
// 
// 

#include "BatteryCharger.h"

BatteryCharger::BatteryCharger()
	: timer(FREQUENCY), oscilloscope(BUTTON_PIN), pid(PID_KP, PID_KD, PID_KI), stop(false), time(0), prevPWM_value(0)
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
		if (timer.fire())
		{
			int Current_Sensing_Voltage = analogRead(CURRENT_SENSING_PIN);
			int Battery_Voltage = analogRead(BATTERY_PIN);
			int Battery_Temperature = analogRead(TEMP_PIN);
			double Charge_Current = ((Current_Sensing_Voltage / 1024.0 * 5.0) / R_CURRENT_SENSING) + (Battery_Voltage / 1024.0 * 5.0) / BATTERY_VOLTAGE_DIVIDER_1;
			double batVoltage = SUPPLY_VOLTAGE - ((Battery_Voltage / 1024.0 * 5.0) / BATTERY_VOLTAGE_DIVIDER_1 * (BATTERY_VOLTAGE_DIVIDER_1 + BATTERY_VOLTAGE_DIVIDER_2));
			double batTemp = ((Battery_Temperature / 1024.0 * 5.0) - 0.5) * 100.0;

			int PWM_value = 0;

			if (!stop)
			{
				if (millis() > MAXTIME)
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
				else
				{
					// For Litium ion cells implement maximum voltage
					PWM_value = pid.calcPidTerm(CHARGE_CURRENT, Charge_Current);
				}

				if (PWM_value != prevPWM_value)
				{
					analogWrite(PWM_PIN, PWM_value);
					prevPWM_value = PWM_value;
				}
			}

			oscilloscope.setSensorReading(0, PWM_value);
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
		pid.reset();
	}

	if (stop && millis() - time > 40000)
	{
		// Stop monitoring battery after charge is completed
		digitalWrite(YELLOWLEDPIN, LOW);
		oscilloscope.checkButton(Button::longPressed);
		while (true)
		{
		}
	}
}