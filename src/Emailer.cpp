#include "Emailer.hpp"
#include <stdio.h>
#include <stdlib.h>

void Emailer::update (int room, float temp) {

	std::string cmd = "/home/pi/m 'Temperature alert " + std::to_string(room) + " " + std::to_string(temp) + "'";

	if (room != 4 && temp >= trigger_temp) {
		system(cmd.c_str());
	}
}

void Emailer::turn_emailer_on() {
	emailer_on = true;
}

void Emailer::turn_emailer_off() {
	emailer_on = false;
}

void Emailer::set_trigger_temp(float temp) {
	trigger_temp = temp;
}
float Emailer::get_trigger_temp() {
	return trigger_temp;
}

bool Emailer::is_emailer_on() {
	return emailer_on;
}