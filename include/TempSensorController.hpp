#ifndef TEMPSENSORCONTROLLER_HPP
#define TEMPSENSORCONTROLLER_HPP
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

#include "ChartCreator.hpp"
#include "RadioController.hpp"
#include <sqlite3.h>

class TempSensorController {

public:
	TempSensorController(RadioController* radio_);

	std::vector<int> lowBattery();
	void checkSensors();
	
private:

	RadioController* radio;
	void saveTempData(uint16_t deviceNum, float temp, uint16_t voltage);
	void printSensorData(uint16_t deviceNum, float temp, uint16_t voltage);
	void printCurrentTime();
};

#endif
