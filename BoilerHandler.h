#ifndef BOILERHANDLER_H
#define BOILERHANDLER_H

#include "CivetServer.h"
#include "Boiler.h"
#include "ReadHtml.h"
#include "boost/format.hpp"

#include <string>

class BoilerHandler : public CivetHandler {
	
public:
 
	BoilerHandler(Boiler* boiler_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	
private:
	Boiler* boiler;
};

#endif