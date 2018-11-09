#ifndef JSONHUBHANDLER_HPP
#define JSONHUBHANDLER_HPP

#include <string>

#include "CivetServer.h"
#include "Boiler.hpp"
#include "json.hpp"


class JsonHubHandler : public CivetHandler
{
	
public:
 
	JsonHubHandler(Boiler* boiler_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	
private:
Boiler* boiler;

	std::string getJson(int water, int heating);

};

#endif