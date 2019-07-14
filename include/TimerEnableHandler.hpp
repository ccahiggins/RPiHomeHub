#ifndef TIMERENABLEHANDLER_HPP
#define TIMERENABLEHANDLER_HPP

#include <iostream>
#include <sstream>

#include "CivetServer.h"
#include "ReadHtml.hpp"
#include "Timer.hpp"
#include "boost/format.hpp"
#include "AuthHandler.hpp"

#include <string>

class TimerEnableHandler : public CivetHandler {
	
public:
 
	TimerEnableHandler(Timer& timer_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	
private:
	Timer& timer;

};

#endif