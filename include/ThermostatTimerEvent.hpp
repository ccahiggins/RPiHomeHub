#ifndef THERMOSTATTIMEREVENT_HPP
#define THERMOSTATTIMEREVENT_HPP

#include "TimerEvent.hpp"
#include "Thermostat.hpp"

#include <sstream>

class ThermostatTimerEvent : public TimerEvent {

public:
	ThermostatTimerEvent(int hour_, int minute_, bool one_time_, bool on_off_, int room_, float temp_, Thermostat& thermostat);
	bool get_on_off();
	int get_room();
	float get_temp();

	void to_stringstream(std::ostringstream& oss);
	
private:
	Thermostat& thermostat;

	bool on_off;
	int room;
	float temp;

	void trigger_event();

};


#endif