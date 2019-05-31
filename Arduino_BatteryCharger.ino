/*
    Name:       Arduino_BatteryCharger.ino
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
