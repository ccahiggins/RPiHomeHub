#ifndef SENSORS_HPP
#define SENSORS_HPP

#include <string>
#include <vector>

#include "DatabaseController.hpp"

class Sensors {

public:
    Sensors();

    static std::string getName(int sensor);
	static std::string getShortName(int sensor);
	static std::string getIdByName(std::string name);
	static std::vector<std::vector<std::string>> getSensors();
};

#endif