// A2DD.h
#ifndef TIMERSHANDLER_H
#define TIMERSHANDLER_H

#include "CivetServer.h"
#include "Timer.h"
#include "ReadHtml.h"
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