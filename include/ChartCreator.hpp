#ifndef CHARTCREATOR_HPP
#define CHARTCREATOR_HPP

#include <algorithm>
#include <ctime>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

#include "ReadHtml.hpp"
#include "Sensors.hpp"
//#include "boost/format.hpp"

class ChartCreator {
public:
    std::string getChartDays(std::string &days);
    std::string getChartFromDays(std::string &from, std::string &days);
    std::string getChartFromTo(std::string &from, std::string &to);
    void writeChartToFile();

private:
    std::string getTempGraph(std::vector<std::vector<std::string>> &data);
    std::string formatGraphData(std::vector<std::vector<std::string>> &data);
};

#endif