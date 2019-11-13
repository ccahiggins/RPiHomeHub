#include "BoilerTimerEvent.hpp"

#include <iostream>

BoilerTimerEvent::BoilerTimerEvent(int hour_, int minute_, bool one_time_, int item_, int duration_, Boiler& boiler_) :
	TimerEvent(hour_, minute_, one_time_), item(item_), duration(duration_), boiler(boiler_) {
}

void BoilerTimerEvent::trigger_event() {
	if(item == 0) {
		boiler.TurnWaterOn(duration);
		std::cout << "Timer turning heating on" << std::endl;
	} else if(item == 1) {
		boiler.TurnHeatingOn(duration);
		std::cout << "Timer turning water on" << std::endl;
	} else {
		std::cout << "Timer turning... nothing on" << std::endl;
	}
}

int BoilerTimerEvent::get_item() {
	return item;
}
int BoilerTimerEvent::get_duration() {
	return duration;
}