#ifndef DISABLETIMERHANDLER_H
#define DISABLETIMERHANDLER_H

#include <iostream>     // std::cout, std::ios
#include <sstream>      // std::ostringstream

#include "CivetServer.h"
#include "ReadHtml.h"
#include "boost/format.hpp"
#include "Timer.h"

#include <string>

class DisableTimerHandler : public CivetHandler {
	
public:
 
	DisableTimerHandler(Timer* timer_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	
private:
	Timer* timer;

};

#endif