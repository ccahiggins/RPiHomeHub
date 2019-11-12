#ifndef THERMOSTATTIMEREVENT_HPP
#define THERMOSTATTIMEREVENT_HPP

#include "TimerEvent.hpp"
#include "Thermostat.hpp"

class ThermostatTimerEvent : public TimerEvent {

public:
	ThermostatTimerEvent(int hour_, int minute_, bool one_time_, bool on_off_, int room_, double temp_, Thermostat& thermostat);
	
private:
	Thermostat& Thermostat;

	bool on_off;
	int room;
	float temp;

	void trigger_event();

};


#endif