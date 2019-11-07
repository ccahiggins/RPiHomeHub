#include "Thermostat.hpp"
#include <iostream>

Thermostat::Thermostat(Boiler& boiler_) : boiler(boiler_) {
	selected_temp = 21.5;
	selected_room = 2;
	thermostat_on = false;
}

void Thermostat::update(int room, float temp) {

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
}

void Thermostat::turn_thermostat_on() {
	thermostat_on = true;
}

void Thermostat::turn_thermostat_off() {
	thermostat_on = false;
	boiler.TurnHeatingOff();
}

void Thermostat::set_temperature(float temp) {
	selected_temp = temp;
}

void Thermostat::set_room(int room) {
	selected_room = room;
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