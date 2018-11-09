#ifndef ADDTIMERHANDLER_HPP
#define ADDTIMERHANDLER_HPP

#include <iostream>     // std::cout, std::ios
#include <sstream>      // std::ostringstream

#include "CivetServer.h"
#include "ReadHtml.hpp"
#include "boost/format.hpp"
#include "Timer.hpp"

#include <string>


class AddTimerHandler : public CivetHandler
{
	
public:
 
	AddTimerHandler(Timer* timer_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	bool handlePost(CivetServer *server, struct mg_connection *conn);
	
private:
	const int BOILER_ITEM_WATER = 0;
	Timer* timer;
	std::string addTimer(int hour, int minute, int duration, int boilerItem, bool onetime);
};

#endif