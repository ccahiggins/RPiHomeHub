#ifndef TIMERADDHANDLER_HPP
#define TIMERADDHANDLER_HPP

#include <iostream>     // std::cout, std::ios
#include <sstream>      // std::ostringstream

#include "CivetServer.h"
#include "ReadHtml.hpp"
#include "boost/format.hpp"
#include "Timer.hpp"
#include "AuthHandler.hpp"
#include "BoilerTimerEvent.hpp"
#include "Boiler.hpp"
#include "Thermostat.hpp"
#include "Sensors.hpp"

#include <string>


class TimerAddHandler : public CivetHandler
{
	
public:
 
	TimerAddHandler(Timer& timer_, Boiler& boiler_, Thermostat& thermostat_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	bool handlePost(CivetServer *server, struct mg_connection *conn);
	
private:
	const int BOILER_ITEM_WATER = 0;
	Timer& timer;
	Boiler& boiler;
	Thermostat& thermostat;
	std::string addBoilerTimer(int hour, int minute, int duration, int boilerItem, bool onetime);
	std::string addThermostatTimer(int hour, int minute, bool on_off, int room, float temperature, bool onetime);
};

#endif