#ifndef THERMOSTAT_HPP
#define THERMOSTAT_HPP

#include <unordered_map>

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

	float get_selected_temp();
	int get_selected_room();

	bool is_thermostat_on();

private:
	Boiler& boiler;

	float selected_temp;
	int selected_room;
	bool thermostat_on;

	std::unordered_map<int, float> last_temps;
};

#endif