#ifndef CHARTCREATORJSON_HPP
#define CHARTCREATORJSON_HPP

#include <istream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <ctime>

#include "ReadHtml.hpp"
//#include "boost/format.hpp"

class ChartCreatorJson
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
	std::string getTempGraph(std::vector<std::vector<std::string>> &data);
	
	std::string formatGraphData(std::vector<std::vector<std::string> > &data);
};

#endif