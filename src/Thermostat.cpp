#include "Thermostat.hpp"
#include <iostream>

Thermostat::Thermostat(Boiler& boiler_) : boiler(boiler_) {
	selected_temp = 21.5;
	selected_room = 2;
	thermostat_on = false;
}

void Thermostat::update(int room, float temp) {
	last_temps[room] = temp;
	if (thermostat_on) {
		if (room == selected_room) {
			if (temp >= selected_temp) {
				boiler.TurnHeatingOff();
			} else if (temp < selected_temp) {
				boiler.TurnHeatingOn();
			}
		}
	}
}

void Thermostat::turn_thermostat_on() {
	float last_temp;
	bool got_last_temp = false;


	if (!(last_temps.find(selected_room) == last_temps.end())) {

		last_temp = last_temps.at(selected_room);
		got_last_temp = true;
	}

	if (got_last_temp && last_temp < selected_temp) {
		boiler.TurnHeatingOn();
	} else if (got_last_temp && last_temp >= selected_temp) {
		boiler.TurnHeatingOff();
	}

	thermostat_on = true;
}

void Thermostat::turn_thermostat_off() {
	thermostat_on = false;
	boiler.TurnHeatingOff();
}

void Thermostat::set_temperature(float temp) {

	selected_temp = temp;

	float last_temp;
	bool got_last_temp = false;


	if (!(last_temps.find(selected_room) == last_temps.end())) {

		last_temp = last_temps.at(selected_room);
		got_last_temp = true;
	}

	if (thermostat_on && got_last_temp && last_temp < selected_temp) {
		boiler.TurnHeatingOn();
	} else if (got_last_temp && last_temp >= selected_temp) {
		boiler.TurnHeatingOff();
	}
}

void Thermostat::set_room(int room) {

	selected_room = room;

	float last_temp;
	bool got_last_temp = false;


	if (!(last_temps.find(selected_room) == last_temps.end())) {

		last_temp = last_temps.at(selected_room);
		got_last_temp = true;
	}

	if (thermostat_on && got_last_temp && last_temp < selected_temp) {
		boiler.TurnHeatingOn();
	} else if (got_last_temp && last_temp >= selected_temp) {
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
