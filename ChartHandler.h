#ifndef CHARTHANDLER_H
#define CHARTHANDLER_H

#include <istream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <ctime>

#include "CivetServer.h"
#include "ReadHtml.h"
//#include "boost/format.hpp"

#include <sqlite3.h>


class ChartHandler: public CivetHandler
{
public:

	bool handleGet(CivetServer *server, struct mg_connection *conn);
	
private:

	struct TempData{
		std::string date;
		float temp;
		int id;
	};
	std::string getSqlStatement(std::string &days);
	std::string getSqlStatement(std::string &from, std::string &to);
	std::string getSqlStatementFromDays(std::string from, std::string &days);
	std::string getTempGraph(std::string &sqlStatement);
	std::string getTempData(std::string &sqlStatement);
	
	std::string formatGraphData(std::vector<std::vector<std::string> > &data);
	static int callback(void *ptr, int argc, char* argv[], char* cols[]);
};

#endif