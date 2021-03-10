#ifndef TEMPSENSORCONTROLLER_HPP
#define TEMPSENSORCONTROLLER_HPP

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <unordered_map>

#include <unistd.h>

#include "ChartCreator.hpp"
#include "RadioController.hpp"
#include "Subject.hpp"
#include "DatabaseController.hpp"

class TempSensorController : public Subject {

public:
	TempSensorController(RadioController& radio_);

	std::vector<int> lowBattery();
	void checkSensors();

    void set_low_voltage_trigger(float voltage);
    float get_low_voltage_trigger();
	
private:

	RadioController& radio;
	void saveTempData(uint16_t deviceNum, float temp, uint16_t voltage);
	void printSensorData(uint16_t deviceNum, float temp, uint16_t voltage);
	void printCurrentTime();
	std::unordered_map<int, float> last_volts;

    float low_voltage_threshold = 3.0;
};

#endif
