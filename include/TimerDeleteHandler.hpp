#ifndef TIMERDELETEHANDLER_HPP
#define TIMERDELETEHANDLER_HPP

#include <iostream>     // std::cout, std::ios
#include <sstream>      // std::ostringstream

#include "CivetServer.h"
#include "ReadHtml.hpp"
#include "boost/format.hpp"
#include "Timer.hpp"
#include "AuthHandler.hpp"

#include <string>

class TimerDeleteHandler : public CivetHandler {
	
public:
 
	TimerDeleteHandler(Timer* timer_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	
private:
	Timer* timer;

};

#endif