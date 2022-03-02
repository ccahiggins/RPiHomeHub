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
	HomeHandler(std::string& api_key_, std::string& api_url_, std::string& ga_key_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);

private:
	std::string& api_key;
	std::string& api_url;
	std::string& ga_key;
};

#endif