// Button.h

#ifndef BUTTON_H
#define BUTTON_H
#include "Arduino.h"

#define TIMESHORTPRESS 5 // in ms
#define TIMELONGPRESS 1000 // in ms

class Button
{
private:
	int pin;
	bool state;
	bool prevState;
	unsigned long timePressed;
public:
	enum buttonResult { unchangedPressed, unchangedNotPressed, changedPressed, shortPressed, longPressed, debounce, undefined };
	Button(int pin);

	buttonResult input();
};

#endif