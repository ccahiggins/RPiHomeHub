#ifndef TIMER_HPP
#define TIMER_HPP

#include "TimerEvent.hpp"
#include "BoilerTimerEvent.hpp"
#include "ThermostatTimerEvent.hpp"

#include <thread>
#include <chrono>
#include <vector>

class Timer {

public:
    Timer();

    void start(const std::chrono::milliseconds &interval, const std::function<void(void)> &timeout);
    void stop();

	void check_timer();

    bool add_event(std::shared_ptr<TimerEvent> &event);
    bool delete_event(int idx);
    bool enable_event(int idx);
    bool disable_event(int idx);
    std::shared_ptr<TimerEvent> get_event(int idx);
    const std::vector<std::shared_ptr<TimerEvent>> get_events() const;

    

private:
    typedef std::chrono::milliseconds Interval;
    typedef std::function<void(void)> Timeout;
    std::vector<std::shared_ptr<TimerEvent>> events;

    int time_diff(int hour1, int minute1, int hour2, int minute2);

    void get_next_event();
    void save_events();

    int next_event = 0;

};


#endif
