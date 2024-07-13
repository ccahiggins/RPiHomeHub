#ifndef CHARTCREATORDYGRAPHS_HPP
#define CHARTCREATORDYGRAPHS_HPP

#include <algorithm>
#include <ctime>
#include <istream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "DatabaseController.hpp"
#include "Sensors.hpp"

class ChartCreatorDygraphs {
public:
    std::string get_chart_days(std::string &days);
    std::string get_chart_from_days(std::string &from, std::string &days);
    std::string get_chart_from_to(std::string &from, std::string &to);
    void writeChartToFile();

private:
    std::string get_chart_data(std::vector<std::vector<std::string>> &data);
};

#endif