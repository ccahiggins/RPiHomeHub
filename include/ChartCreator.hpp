#ifndef CHARTCREATOR_HPP
#define CHARTCREATOR_HPP

#include <istream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <ctime>

#include "ReadHtml.hpp"
//#include "boost/format.hpp"

#include <sqlite3.h>


class ChartCreator
{
public:

	std::string getChartDays(std::string &days);
	std::string getChartFromDays(std::string &from, std::string &days);
	std::string getChartFromTo(std::string &from, std::string &to);
	void writeChartToFile();
	
private:

	struct TempData{
		std::string date;
		float temp;
		int id;
	};
	std::string getSqlStatementDays(std::string &days);
	std::string getSqlStatementFromTo(std::string &from, std::string &to);
	std::string getSqlStatementFromDays(std::string &from, std::string &days);
	std::string getTempGraph(std::string &sqlStatement);
	std::string getTempData(std::string &sqlStatement);
	
	std::string formatGraphData(std::vector<std::vector<std::string> > &data);
	static int callback(void *ptr, int argc, char* argv[], char* cols[]);
};

#endif