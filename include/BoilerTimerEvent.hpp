#ifndef BOILERTIMEREVENT_HPP
#define BOILERTIMEREVENT_HPP

#include "TimerEvent.hpp"

class BoilerTimerEvent : public TimerEvent {

public:
	BoilerTimerEvent(int hour_, int minute_, bool one_time_, int item_, int duration_, Boiler& boiler_);
	
private:
	Boiler& boiler;

	int item;
	int duration;

	void trigger_event();

};


#endif