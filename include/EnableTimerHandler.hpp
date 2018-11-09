#ifndef ENABLETIMERHANDLER_HPP
#define ENABLETIMERHANDLER_HPP

#include <iostream>     // std::cout, std::ios
#include <sstream>      // std::ostringstream

#include "CivetServer.h"
#include "ReadHtml.hpp"
#include "Timer.hpp"
#include "boost/format.hpp"

#include <string>

class EnableTimerHandler : public CivetHandler {
	
public:
 
	EnableTimerHandler(Timer* timer_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	
private:
	Timer* timer;

};

#endif