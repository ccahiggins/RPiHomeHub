// A2DD.h
#ifndef TIMER_H
#define TIMER_H
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "Timer.h"

class Timer
{

public:

	struct TimerEvent {
		int startHour;
		int startMinute;
		int duration;
		int boilerItem;
		bool enabled;
		bool oneTime;
		int id;
	};

	Timer();
	
	bool enableTimerEvent(int id);
	bool disableTimerEvent(int id);
	//bool removeTimerEvent(unsigned position);
	bool removeTimerEvent(int id);
	TimerEvent* checkTimer(int interval);
	bool addTimerEvent(int startHour, int startMinute, int duration, int boilerItem, bool enabled, bool oneTime);
	std::vector<TimerEvent>& getTimers();
	bool addTimerEvent(TimerEvent event);
	void getNextTimerEvent();
	void loadTimers();

private:
	int getSecondsToNextEvent(TimerEvent& event);
	int minsBetweenTimes(int hour1, int minute1, int hour2, int minute2);
	void saveTimers();
	void writeToFile(std::string message);

};

#endif