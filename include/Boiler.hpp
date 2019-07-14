#ifndef BOILER_HPP
#define BOILER_HPP
#include <unistd.h>
#include <iostream>

#include "RadioController.hpp"

class Boiler
{

public:

	Boiler(RadioController& rcont_);

	struct boiler_status
	{
		int heatingStatus;
		int waterStatus;
	};
	
	boiler_status getBoilerStatus();
	bool TurnHeatingOn();
	bool TurnWaterOn();
	bool TurnWaterOn(int duration);
	bool TurnWaterOff();
	bool TurnHeatingOn(int duration);
	bool TurnHeatingOff();
/* 	int WaterStatus();
	int HeatingStatus(); */

	
private:
	const int WATERPIN = 4;
	const int HEATINGPIN = 5;
	const int PINON = 0;
	const int PINOFF = 1;
	const int PINNODURATION = 0;
	const int SENDON = -1;
	const int ON = 1;
	const int OFF = 0;
	const int ERROR = -2;
	const int ERRORPAYLOAD = 65535;
	
	// int getStatus(int pin);
	RadioController& radio;
	bool switchPin(int pin, int state, int duration, unsigned char type);
};

#endif
