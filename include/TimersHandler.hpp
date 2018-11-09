#ifndef TIMERSHANDLER_HPP
#define TIMERSHANDLER_HPP

#include "CivetServer.h"
#include "Timer.hpp"
#include "ReadHtml.hpp"
#include <istream>
#include <string>
#include "boost/format.hpp"

class TimersHandler : public CivetHandler
{
	
public:
 
	TimersHandler(Timer* timer_);

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	
private:
	Timer* timer;
};

#endif