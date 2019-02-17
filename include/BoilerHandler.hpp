#ifndef BOILERHANDLER_HPP
#define BOILERHANDLER_HPP

#include "CivetServer.h"
#include "Boiler.hpp"
#include "ReadHtml.hpp"
#include "boost/format.hpp"
#include "AuthHandler.hpp"

#include <string>

class BoilerHandler : public CivetHandler {
	
public:
 
	BoilerHandler(Boiler* boiler_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	
private:
	Boiler* boiler;
};

#endif