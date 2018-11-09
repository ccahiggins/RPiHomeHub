#ifndef ENABLETIMERHANDLER_H
#define ENABLETIMERHANDLER_H

#include <iostream>     // std::cout, std::ios
#include <sstream>      // std::ostringstream

#include "CivetServer.h"
#include "ReadHtml.h"
#include "Timer.h"
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