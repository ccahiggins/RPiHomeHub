// A2DD.h
#ifndef HUBHANDLER_HPP
#define HBUHANDLER_HPP

#include <string>

#include "CivetServer.h"
#include "Boiler.hpp"
#include "TempsensorController.hpp"
#include "ReadHtml.hpp"
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