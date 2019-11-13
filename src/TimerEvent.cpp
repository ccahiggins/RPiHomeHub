#include "TimerEvent.hpp"

#include <ctime>
#include <iostream>

TimerEvent::TimerEvent(int hour_, int minute_, bool one_time_) : hour(hour_), minute(minute_), one_time(one_time_) {
	enabled = true;

	trig_hour = -1;
	trig_min = -1;
	trig_day = -1;
}

void TimerEvent::trigger() {
	time_t t = time(0);
	struct tm * now = localtime( & t );
	int h = now->tm_hour;
	int m = now->tm_min;
	int d = now->tm_wday;

	trig_hour = h;
	trig_min = m;
	trig_day = d;

	trigger_event();
}

int TimerEvent::get_hour() {
	return hour;
}
int TimerEvent::get_minute() {
	return minute;
}
int TimerEvent::get_trig_hour() {
	return trig_hour;
}
int TimerEvent::get_trig_min() {
	return trig_min;
}
int TimerEvent::get_trig_day() {
	return trig_day;
}
bool TimerEvent::is_enabled() {
	return enabled;
}
void TimerEvent::enable() {
	enabled = true;
}

void TimerEvent::disable() {
	enabled = false;
}

bool TimerEvent::is_one_time() {
	return one_time;
}