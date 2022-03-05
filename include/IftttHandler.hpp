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
#include "Sonoff.hpp"
#include "json.hpp"

#define ERROR -2
#define OFF 0
#define ON -1

class IftttHandler : public CivetHandler {
	
public:
	IftttHandler(Boiler& boiler_, Thermostat& thermostat_, Timer& timer_, TempSensorController& tempSens_, std::vector<Sonoff> _sonoff_list);

	bool handlePost(CivetServer *server, struct mg_connection *conn);
	
private:
	bool apiKeyMatch(std::string requestKey);
	std::string getTemp(std::string id);
	std::vector<nlohmann::json> getTimers();
	Boiler& boiler;
	Thermostat& thermostat;
	Timer& timer;
	TempSensorController& tempSens;
	std::vector<Sonoff> sonoff_list;
};

#endif