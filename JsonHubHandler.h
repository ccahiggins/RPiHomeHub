#ifndef JSONHUBHANDLER_H
#define JSONHUBHANDLER_H

#include <string>

#include "CivetServer.h"
#include "Boiler.h"
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