#include "Thermostat.hpp"
#include <iostream>

Thermostat::Thermostat(Boiler& boiler_) : boiler(boiler_) {
	selected_temp = 21.5;
	selected_room = 2;
	thermostat_on = false;
}

void Thermostat::update(int room, float temp) {
	std::cout << "Update" << std::endl;
	last_temps[room] = temp;
	std::cout << "Update - last_temps" << std::endl;
	if (thermostat_on) {
		if (room == selected_room) {
			if (temp >= selected_temp) {
				std::cout << "THERMOSTAT OFF:" << temp << std::endl;
				boiler.TurnHeatingOff();
			} else if (temp < selected_temp) {
				std::cout << "THERMOSTAT ON:" << temp << std::endl;
				boiler.TurnHeatingOn();
			}
		}
	}
	std::cout << "Update - bye bye" << std::endl;
}

void Thermostat::turn_thermostat_on() {
	std::cout << "Thrmst_on" << std::endl;
	float last_temp;
	bool got_last_temp = false;
	std::cout << "Thrmst_on - blah" << std::endl;


	if (!(last_temps.find(selected_room) == last_temps.end())) {
		std::cout << "Thrmst_on - found key" << std::endl;

		last_temp = last_temps.at(selected_room);
		got_last_temp = true;
		std::cout << "Thrmst_on - found key - got value" << std::endl;

	} else {
		std::cout << "Thrmst_on - no key" << std::endl;
	}

	if (got_last_temp && last_temp < selected_temp) {
		std::cout << "Thrmst_on - got val, boiler on" << std::endl;
		boiler.TurnHeatingOn();
	} else if (got_last_temp && last_temp >= selected_temp) {
		std::cout << "Thrmst_on - got val, boiler off" << std::endl;
		boiler.TurnHeatingOff();
	}

	thermostat_on = true;
}

void Thermostat::turn_thermostat_off() {
	thermostat_on = false;
	boiler.TurnHeatingOff();
}

void Thermostat::set_temperature(float temp) {
	std::cout << "Thrmst_tmp" << std::endl;

	selected_temp = temp;

	float last_temp;
	bool got_last_temp = false;
	std::cout << "Thrmst_tmp - blah" << std::endl;


	if (!(last_temps.find(selected_room) == last_temps.end())) {
		std::cout << "Thrmst_tmp - found key" << std::endl;

		last_temp = last_temps.at(selected_room);
		got_last_temp = true;
		std::cout << "Thrmst_tmp - found key - got value" << std::endl;

	} else {
		std::cout << "Thrmst_tmp - no key" << std::endl;
	}

	if (got_last_temp && last_temp < selected_temp) {
		std::cout << "Thrmst_tmp - got val, boiler on" << std::endl;
		boiler.TurnHeatingOn();
	} else if (got_last_temp && last_temp >= selected_temp) {
		std::cout << "Thrmst_tmp - got val, boiler off" << std::endl;
		boiler.TurnHeatingOff();
	}
}

void Thermostat::set_room(int room) {
	std::cout << "Thrmst_room" << std::endl;

	selected_room = room;

	float last_temp;
	bool got_last_temp = false;
	std::cout << "Thrmst_room - blah" << std::endl;


	if (!(last_temps.find(selected_room) == last_temps.end())) {
		std::cout << "Thrmst_room - found key" << std::endl;

		last_temp = last_temps.at(selected_room);
		got_last_temp = true;
		std::cout << "Thrmst_room - found key - got value" << std::endl;

	} else {
		std::cout << "Thrmst_room - no key" << std::endl;
	}

	if (got_last_temp && last_temp < selected_temp) {
		std::cout << "Thrmst_room - got val, boiler on" << std::endl;
		boiler.TurnHeatingOn();
	} else if (got_last_temp && last_temp >= selected_temp) {
		std::cout << "Thrmst_room - got val, boiler off" << std::endl;
		boiler.TurnHeatingOff();
	}
}

bool Thermostat::is_thermostat_on() {

	return thermostat_on;
}

float Thermostat::get_selected_temp() {
	return selected_temp;
}

int Thermostat::get_selected_room() {
	return selected_room;
}