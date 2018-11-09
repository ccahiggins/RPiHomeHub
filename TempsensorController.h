#ifndef TEMPSENSORCONTROLLER_H
#define TEMPSENSORCONTROLLER_H
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

#include "ChartCreator.h"
#include "RadioController.h"
#include <sqlite3.h>

class TempsensorController {

public:
	TempsensorController(RadioController* radio_);

	std::vector<int> lowBattery();
	void checkSensors();
	
private:

	RadioController* radio;
	void saveTempData(uint16_t deviceNum, float temp, uint16_t voltage);
	void printSensorData(uint16_t deviceNum, float temp, uint16_t voltage);
	void printCurrentTime();
};

#endif
