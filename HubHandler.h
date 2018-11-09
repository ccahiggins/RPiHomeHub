// A2DD.h
#ifndef HUBHANDLER_H
#define HBUHANDLER_H

#include <string>

#include "CivetServer.h"
#include "Boiler.h"
#include "TempsensorController.h"
#include "ReadHtml.h"
#include "boost/format.hpp"

#define ERROR -2
#define OFF 0
#define ON -1

class HubHandler : public CivetHandler
{
	
public:
 
	HubHandler(Boiler* boiler_, TempsensorController* tempSens_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	
private:

	Boiler* boiler;
	TempsensorController* tempSens;
};

#endif