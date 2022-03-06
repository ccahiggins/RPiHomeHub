#ifndef CHARTCREATORJSON_HPP
#define CHARTCREATORJSON_HPP

#include <istream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <map>

#include "Sensors.hpp"
#include "DatabaseController.hpp"

class ChartCreatorJson
{
public:

	std::string get_chart_days(std::string &days);
	std::string get_chart_from_days(std::string &from, std::string &days);
	std::string get_chart_from_to(std::string &from, std::string &to);
	void writeChartToFile();
	
private:

	struct TempData{
		std::string date;
		float temp;
		int id;
	};
	std::string get_chart_json(std::vector<std::vector<std::string>> &data);
};

#endif