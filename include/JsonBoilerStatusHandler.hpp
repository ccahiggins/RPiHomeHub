#ifndef JSONBOILERSTATUSHANDLER_HPP
#define JSONBOILERSTATUSHANDLER_HPP

#include <string>

#include "CivetServer.h"
#include "Boiler.hpp"
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