#ifndef VOLTAGEHANDLER_H
#define VOLTAGEHANDLER_H

#include <istream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include <sqlite3.h>
#include "CivetServer.h"
#include "ReadHtml.h"
#include "boost/format.hpp"

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