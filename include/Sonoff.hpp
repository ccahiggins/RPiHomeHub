#ifndef SONOFF_HPP
#define SONOFF_HPP

#include <iostream>
#include <string>

#include <cpr/cpr.h>

class Sonoff {

public:

	Sonoff(std::string& ip_adress_);

	int turn_on();
	int turn_off();
	int toggle();
	int status();
	
private:

	std::string ip_adress;
};

#endif