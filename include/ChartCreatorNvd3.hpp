#ifndef CHARTCREATORNVD3_HPP
#define CHARTCREATORNVD3_HPP

#include <istream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <map>

#include "Sensors.hpp"
#include "DatabaseController.hpp"

class ChartCreatorNvd3
{
public:

	std::string get_chart_days(std::string &days);
	std::string get_chart_from_days(std::string &from, std::string &days);
	std::string get_chart_from_to(std::string &from, std::string &to);
	void writeChartToFile();
	
private:
	std::string get_chart_data(std::vector<std::vector<std::string>> &data);
};

#endif