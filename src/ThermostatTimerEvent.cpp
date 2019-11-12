#include "ThermostatTimerEvent.hpp"
#include <iostream>

ThermostatTimerEvent::ThermostatTimerEvent(int hour_, int minute_, bool one_time_, bool on_off_, int room_, double temp_, Thermostat& thermostat_) :
	TimerEvent(hour_, minute_, one_time_), on_off(on_off_), room(room_), temp(temp_), thermostat(thermostat_) {
}

void ThermostatTimerEvent::trigger_event() {
	if (on_off) {
		thermostat.turn_thermostat_on();
		thermostat.set_room(room);
		thermostat.set_temp(temp);
	} else {
		thermostat.turn_thermostat_off();
	}
}