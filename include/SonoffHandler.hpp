#ifndef SONOFFHANDLER_HPP
#define SONOFFHANDLER_HPP

#include "CivetServer.h"
#include "ReadHtml.hpp"
#include "boost/format.hpp"
#include "AuthHandler.hpp"
#include "Sonoff.hpp"

#include <string>

class SonoffHandler : public CivetHandler {
	
public:
	SonoffHandler(std::vector<Sonoff> _sonoff_list);
	bool handleGet(CivetServer *server, struct mg_connection *conn);
	void add_sonoff(Sonoff& sonoff);
	
private:
	std::vector<Sonoff> sonoff_list;
};

#endif