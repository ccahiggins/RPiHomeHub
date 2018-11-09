#ifndef DeleteTimerHandler_HPP
#define DeleteTimerHandler_HPP

#include <iostream>     // std::cout, std::ios
#include <sstream>      // std::ostringstream

#include "CivetServer.h"
#include "ReadHtml.hpp"
#include "boost/format.hpp"
#include "Timer.hpp"

#include <string>

class DeleteTimerHandler : public CivetHandler {
	
public:
 
	DeleteTimerHandler(Timer* timer_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	
private:
	Timer* timer;

};

#endif