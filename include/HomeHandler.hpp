#ifndef HOMEHANDLER_HPP
#define HOMEHANDLER_HPP

#include "CivetServer.h"
#include "Timer.hpp"
#include "ReadHtml.hpp"
#include <istream>
#include <string>
#include "AuthHandler.hpp"
#include "boost/format.hpp"

class HomeHandler : public CivetHandler
{
	
public:
	bool handleGet(CivetServer *server, struct mg_connection *conn);
};

#endif