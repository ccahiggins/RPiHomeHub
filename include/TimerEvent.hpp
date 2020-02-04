#ifndef TIMEREVENT_HPP
#define TIMEREVENT_HPP

#include <ctime>
#include <ostream>

class TimerEvent {
	
private:
	int hour;
	int minute;
	int trig_hour;
	int trig_min;
	int trig_day;
	bool enabled;
	bool one_time;
	virtual void trigger_event() = 0;

public:
	TimerEvent(int hour_, int minute_, bool one_time_);
	void trigger();
	bool has_triggered();

	int get_hour();
	int get_minute();
	int get_trig_hour();
	int get_trig_min();
	int get_trig_day();
	bool is_enabled();
	bool is_one_time();
	void enable();
	void disable();

	virtual void to_stringstream(std::ostringstream& oss) = 0;

};

#endif