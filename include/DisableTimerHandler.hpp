#ifndef DISABLETIMERHANDLER_HPP
#define DISABLETIMERHANDLER_HPP

#include <iostream>
#include <sstream>

#include "CivetServer.h"
#include "ReadHtml.hpp"
#include "boost/format.hpp"
#include "Timer.hpp"

#include <string>

class DisableTimerHandler : public CivetHandler {
	
public:
 
	DisableTimerHandler(Timer* timer_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	
private:
	Timer* timer;

};

#endif