#ifndef SENSORS_HPP
#define SENSORS_HPP

#include <istream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <sstream>

#include <sqlite3.h>

class Sensors {

public:
    Sensors();

    static std::string getName(int sensor);
	static std::string getShortName(int sensor);
	static std::string getIdByName(std::string name);
	static std::vector<std::vector<std::string>> getSensors();

private:
    
	static std::string getSqlStatementForSensorName(int sensor);
	static std::string getSqlStatementForSensorShortName(int sensor);
	static std::string getSqlStatementForIdByName(std::string name);
	static std::string getSqlStatementForSensors();
	//static std::vector<std::vector<std::string>> getSensorNames();
	
	static int sensor_callback(void *ptr, int argc, char* argv[], char* cols[]);
    static int sensors_callback(void *ptr, int argc, char* argv[], char* cols[]);

};

#endif