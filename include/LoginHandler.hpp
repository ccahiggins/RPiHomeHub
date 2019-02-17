// A2DD.h
#ifndef LOGINHANDLER_HPP
#define LOGINHANDLER_HPP

#include <string>

#include "CivetServer.h"
#include "AuthHandler.hpp"
#include "ReadHtml.hpp"
#include "boost/format.hpp"
#include <boost/algorithm/string.hpp>

class LoginHandler : public CivetHandler
{
	
public:

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	bool handlePost(CivetServer *server, struct mg_connection *conn);
	
};

#endif