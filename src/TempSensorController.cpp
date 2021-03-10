#include "TempSensorController.hpp"

TempSensorController::TempSensorController(RadioController& radio_) : radio(radio_) {
	std::cout << "TC" << std::endl;
}

void TempSensorController::set_low_voltage_trigger(float voltage) {

    low_voltage_threshold = voltage;
}

float TempSensorController::get_low_voltage_trigger() {

    return low_voltage_threshold;
}

void TempSensorController::checkSensors() {
	
	RadioController::payload_temp payload = radio.getTempPayload();
	
	if (payload.deviceNum != 65535) {
		if (payload.deviceNum > 5) {
			std::cout << "Debug-> ID: " << payload.deviceNum << "  ==  " << std::flush;
		} else {
			saveTempData(payload.deviceNum, payload.temp, payload.voltage);
			system("./push");
			printSensorData(payload.deviceNum, payload.temp, payload.voltage);
			notify(payload.deviceNum, payload.temp);
			last_volts[payload.deviceNum] = payload.voltage * 0.001;
		}
	}
}

void TempSensorController::saveTempData(uint16_t deviceNum, float temp, uint16_t voltage) {
	
	DatabaseController::insertTempData(deviceNum, temp, voltage);
	
	ChartCreator chartCreator;
	chartCreator.writeChartToFile();
	//std::cout << " " << rc << " " << std::flush;
}

std::vector<int> TempSensorController::lowBattery() {
//	float low = 3.0;
	std::vector<int> lowBatts;

	for (std::pair<int, float> element : last_volts) {
		if (element.second <= low_voltage_threshold) {
			lowBatts.push_back(element.first);
		}
	}

	return lowBatts;
}

void TempSensorController::printSensorData(uint16_t deviceNum, float temp, uint16_t voltage) {
	
	std::stringstream s; 
	s << std::fixed << std::setprecision(1) << temp;

	std::cout << "-\033[31mS" << deviceNum << ":\033[36m" << s.str() << "°C,\033[32m";

	printCurrentTime();
	std::cout << ",\033[35m";
	
	float battVolts = voltage * 0.001;
	
	if (battVolts > 0) {	
		std::cout << battVolts << "V\033[0m";
	}
	std::cout << " = " << std::flush;
}

void TempSensorController::printCurrentTime() {

	time_t t = time(0);   // get time now
	struct tm* now = localtime( & t );
	int hour = now->tm_hour;
	int minute = now->tm_min;

	if (hour < 10) {
		std::cout << "0";
	}
	std::cout << hour << ":";
	
	if (minute < 10) {
		std::cout << "0";
	}
	std::cout << minute;
}
