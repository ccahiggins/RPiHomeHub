#ifndef TIMERDISABLEHANDLER_HPP
#define TIMERDISABLEHANDLER_HPP

#include <iostream>
#include <sstream>

#include "CivetServer.h"
#include "ReadHtml.hpp"
#include "boost/format.hpp"
#include "Timer.hpp"

#include <string>

class TimerDisableHandler : public CivetHandler {
	
public:
 
	TimerDisableHandler(Timer* timer_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	
private:
	Timer* timer;

};

#endif