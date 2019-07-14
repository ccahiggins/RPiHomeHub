#ifndef TIMERHANDLER_HPP
#define TIMERHANDLER_HPP

#include "CivetServer.h"
#include "Timer.hpp"
#include "ReadHtml.hpp"
#include <istream>
#include <string>
#include "boost/format.hpp"
#include "AuthHandler.hpp"

class TimerHandler : public CivetHandler {
	
public:
	TimerHandler(Timer& timer_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	
private:
	Timer& timer;
};

#endif