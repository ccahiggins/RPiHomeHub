#ifndef JSONBOILERHANDLER_H
#define JSONBOILERHANDLER_H

#include <string>

#include "CivetServer.h"
#include "Boiler.h"
#include "json.hpp"


class JsonBoilerHandler : public CivetHandler
{
	
public:
 
	JsonBoilerHandler(Boiler* boiler_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	
private:
	Boiler* boiler;
	std::string getJson(std::string js);

};

#endif