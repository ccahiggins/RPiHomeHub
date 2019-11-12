#ifndef TIMER_HPP
#define TIMER_HPP

#include "TimerEvent.hpp"

#include <thread>
#include <chrono>
#include <vector>

class Timer {

public:
    Timer();

    void start(const std::chrono::milliseconds &interval, const std::function<void(void)> &timeout);
    void stop();

	void check_timer();

    void add_event(std::shared_ptr<TimerEvent> &event);
    void delete_event(int idx);
    void enable_event(int idx);
    void disable_event(int idx);
    std::shared_ptr<TimerEvent> get_event(int idx);
    
    std::vector<std::shared_ptr<TimerEvent>> events;

private:
    typedef std::chrono::milliseconds Interval;
    typedef std::function<void(void)> Timeout;
    int time_diff(int hour1, int minute1, int hour2, int minute2);

    void get_next_event();

    int next_event = 0;

};


#endif
