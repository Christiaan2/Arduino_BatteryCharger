/*
    Name:       Arduino_BatteryCharger.ino
    Created:	30-Aug-18 11:49:03 AM
    Author:     HPELITEBOOK\C. Reurslag
*/

#include "BatteryCharger.h"

BatteryCharger batteryCharger;

void setup()
{
	batteryCharger.initialize();
}

void loop()
{
	batteryCharger.run();
}
