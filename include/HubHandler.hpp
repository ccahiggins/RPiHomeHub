// A2DD.h
#ifndef HUBHANDLER_HPP
#define HUBHANDLER_HPP

#include <string>

#include "CivetServer.h"
#include "Boiler.hpp"
#include "TempSensorController.hpp"
#include "ReadHtml.hpp"
#include "boost/format.hpp"
#include "AuthHandler.hpp"

#define ERROR -2
#define OFF 0
#define ON -1

class HubHandler : public CivetHandler
{
	
public:
 
	HubHandler(Boiler& boiler_, TempSensorController& tempSens_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	
private:

	Boiler& boiler;
	TempSensorController& tempSens;
};

#endif