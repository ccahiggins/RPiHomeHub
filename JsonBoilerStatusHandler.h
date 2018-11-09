#ifndef JSONBOILERSTATUSHANDLER_H
#define JSONBOILERSTATUSHANDLER_H

#include <string>

#include "CivetServer.h"
#include "Boiler.h"
#include "json.hpp"

#define ERROR -2
#define OFF -1
#define ON 0

class JsonBoilerStatusHandler : public CivetHandler
{
	
public:
 
	JsonBoilerStatusHandler(Boiler* boiler_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	
private:
Boiler* boiler;

	std::string getJson();

};

#endif