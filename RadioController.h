// A2DD.h
#ifndef RADIOCONTROLLER_H
#define RADIOCONTROLLER_H

#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>

class RadioController
{
	RF24 radio;
	RF24Network network;
	
public:
 
	RadioController();
	
	struct payload_boiler_status
	{
		uint16_t deviceNum;
		uint16_t heatingState;
		uint16_t heatingDuration;
		uint16_t waterState;
		uint16_t waterDuration;
	};
	
	struct payload_boiler_control
	{
		uint16_t deviceNum;
		uint16_t pinState;
		uint16_t duration;
	};

	struct payload_temp {
		float temp;
		uint16_t voltage;
		uint16_t deviceNum;
	};

	struct payload_debug {
		uint16_t deviceNum;
		uint16_t message;
	};

	struct payload_time{
		uint16_t hours;
		uint16_t minutes;
		uint16_t seconds;
	};

	struct payload_lights{
		uint16_t onOff;
	};

	void writeToFile(std::string text);

	bool sendPayload(RF24NetworkHeader header, const void * payload, uint16_t size);
	// payload_boiler getBoilerPayload();
	payload_temp getTempPayload();
	payload_boiler_status sendGetBoilerPayload(RF24NetworkHeader header, const void * payload, uint16_t size);

};

#endif
