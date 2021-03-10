#include "Sensors.hpp"

std::string Sensors::getName(int sensor) {
	
	std::string name = DatabaseController::getSensorName(sensor);
	
	return name;
}

std::string Sensors::getShortName(int sensor) {
	
	std::string short_name = DatabaseController::getSensorShortName(sensor);
	
	return short_name;
}

std::string Sensors::getIdByName(std::string name) {
	
	std::string id = DatabaseController::getSensorIdByName(name);
	
	return id;
}

std::vector<std::vector<std::string>> Sensors::getSensors() {
	
	std::vector<std::vector<std::string>> data = DatabaseController::getAllSensors();
	
	return data;
}