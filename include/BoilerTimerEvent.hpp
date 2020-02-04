#ifndef BOILERTIMEREVENT_HPP
#define BOILERTIMEREVENT_HPP

#include "TimerEvent.hpp"
#include "Boiler.hpp"

class BoilerTimerEvent : public TimerEvent {

public:
	BoilerTimerEvent(int hour_, int minute_, bool one_time_, int item_, int duration_, Boiler& boiler_);
	int get_item();
	int get_duration();
	
private:
	Boiler& boiler;

	int item;
	int duration;

	void trigger_event();

	void to_stringstream(std::ostringstream& oss);

};


#endif