#ifndef JSONBOILERHANDLER_HPP
#define JSONBOILERHANDLER_HPP

#include <string>

#include "CivetServer.h"
#include "Boiler.hpp"
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