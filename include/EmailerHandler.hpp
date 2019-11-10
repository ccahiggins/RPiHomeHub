#ifndef EMAILERHANDLER_HPP
#define EMAILERHANDLER_HPP

#include "Emailer.hpp"

#include "CivetServer.h"
#include "ReadHtml.hpp"
#include "boost/format.hpp"
#include "AuthHandler.hpp"

#include <string>

class EmailerHandler : public CivetHandler {
	
public:
 
	EmailerHandler(Emailer& emailer_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	
private:
	Emailer& emailer;
};

#endif