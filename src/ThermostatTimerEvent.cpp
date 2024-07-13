#include "ThermostatTimerEvent.hpp"
#include <iostream>

ThermostatTimerEvent::ThermostatTimerEvent(int hour_, int minute_, bool one_time_, bool on_off_, int room_, float temp_, Thermostat& thermostat_) :
	TimerEvent(hour_, minute_, one_time_), thermostat(thermostat_), on_off(on_off_), room(room_), temp(temp_) {
}

void ThermostatTimerEvent::trigger_event() {
	if (on_off) {
		thermostat.turn_thermostat_on();
		thermostat.set_room(room);
		thermostat.set_temperature(temp);
	} else {
		thermostat.turn_thermostat_off();
	}
}

bool ThermostatTimerEvent::get_on_off() {
	return on_off;
}

int ThermostatTimerEvent::get_room() {
	return room;
}

float ThermostatTimerEvent::get_temp() {
	return temp;
}


void ThermostatTimerEvent::to_stringstream(std::ostringstream& oss) {
	oss << "THERMOSTAT" << std::endl;
    oss << get_hour() << std::endl;
    oss << get_minute() << std::endl;
    oss << is_one_time() << std::endl;
    oss << is_enabled() << std::endl;
    oss << get_trig_hour() << std::endl;
    oss << get_trig_min() << std::endl;
    oss << get_trig_day() << std::endl;
    oss << on_off << std::endl;
    oss << room << std::endl;
    oss << temp << std::endl;
}
