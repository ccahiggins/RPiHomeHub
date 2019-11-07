#ifndef THERMOSTATHANDLER_HPP
#define THERMOSTATHANDLER_HPP

#include "CivetServer.h"
#include "Thermostat.hpp"
#include "ReadHtml.hpp"
#include "boost/format.hpp"
#include "AuthHandler.hpp"

#include <string>

class ThermostatHandler : public CivetHandler {
	
public:
 
	ThermostatHandler(Thermostat& thermostat_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	
private:
	Thermostat& thermostat;
};

#endif