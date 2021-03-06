// A2DD.h
#ifndef IFTTTHANDLER_HPP
#define IFTTTHANDLER_HPP

#include <string>
#include <vector>
#include "CivetServer.h"

#include "DatabaseController.hpp"
#include "Boiler.hpp"
#include "Thermostat.hpp"
#include "Timer.hpp"
#include "TempSensorController.hpp"
#include "RequestValidator.hpp"
#include "TimerEvent.hpp"
#include "BoilerTimerEvent.hpp"

#define ERROR -2
#define OFF 0
#define ON -1

class IftttHandler : public CivetHandler {
	
public:
	IftttHandler(Boiler& boiler_, Thermostat& thermostat_, Timer& timer_, TempSensorController& tempSens_);

	bool handlePost(CivetServer *server, struct mg_connection *conn);
	
private:
	bool apiKeyMatch(std::string requestKey);
	std::string getTemp(std::string id);
	std::string getTimers();
	Boiler& boiler;
	Thermostat& thermostat;
	Timer& timer;
	TempSensorController& tempSens;
};

#endif