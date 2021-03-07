#ifndef VOLTAGEHANDLER_HPP
#define VOLTAGEHANDLER_HPP

#include <istream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include <sqlite3.h>
#include "CivetServer.h"
#include "boost/format.hpp"
#include "ReadHtml.hpp"
#include "AuthHandler.hpp"
#include "Sensors.hpp"

class VoltageHandler: public CivetHandler
{
public:

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	
private:

	std::string formatVolts(std::vector<std::vector<std::string> > &data);
	std::string printVolts(int days);
	std::string printVolts();
	static int callback(void *ptr, int argc, char* argv[], char* cols[]);
};

#endif