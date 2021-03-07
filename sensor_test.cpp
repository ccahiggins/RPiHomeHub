#include "include/Sensors.hpp"

#include <iostream>
int main () {
	
	
	
	std::cout << "HELLO" << std::endl;
	
	std::cout << Sensors::getName(1) << std::endl;
	std::cout << Sensors::getShortName(1) << std::endl;
	std::cout << Sensors::getName(2) << std::endl;
	std::cout << Sensors::getShortName(2) << std::endl;
	std::cout << Sensors::getName(3) << std::endl;
	std::cout << Sensors::getShortName(3) << std::endl;
	std::cout << Sensors::getName(5) << std::endl;
	std::cout << Sensors::getShortName(5) << std::endl;
	std::cout << Sensors::getIdByName("Living Room") << std::endl;
	
	return 0;
}