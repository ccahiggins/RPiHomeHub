#ifndef THERMOSTAT_HPP
#define THERMOSTAT_HPP

#include "Observer.hpp"
#include "Boiler.hpp"

class Thermostat : public Observer {

public:
	Thermostat(Boiler& boiler_);

	void update(int room, float temp);

	void turn_thermostat_on();
	void turn_thermostat_off();
	void set_temperature(float temp);
	void set_room(int room);

private:
	Boiler& boiler;

	float selected_temp;
	int selected_room;
	bool thermostat_on;
};

#endif