#ifndef EMAILER_HPP
#define EMAILER_HPP

#include "Observer.hpp"
#include <string>

class Emailer : public Observer {

private:
	float trigger_temp = 40;
	bool emailer_on = true;

public:
	void turn_emailer_on();
	void turn_emailer_off();
	void update (int room, float temp);
	void set_trigger_temp(float temp);
	float get_trigger_temp();
	bool is_emailer_on();
};

#endif