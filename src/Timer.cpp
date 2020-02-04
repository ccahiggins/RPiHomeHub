#include "Timer.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <ostream>

std::thread th;
bool running = false;

Timer::Timer() {}

void Timer::start(const Timer::Interval &interval, const Timeout &timeout) {
    running = true;

    th = std::thread([=]() {
        while (running) {
            std::this_thread::sleep_for(interval);
            timeout();
        }
    });
    th.detach();
}

void Timer::check_timer() {
    if (events.size() > 0) {
        time_t t = time(0);
        struct tm * now = localtime( & t );
        int h = now->tm_hour;
        int m = now->tm_min;
        int d = now->tm_wday;

        if (next_event >= 0) {
            const std::shared_ptr<TimerEvent> e = events[next_event];
            if (h == e->get_hour()) {
                if (m == e->get_minute()) {
                    if (d != e->get_trig_day()) {
                        if (e->is_enabled()) {
                            e->trigger();
                            get_next_event();
                        }
                    }
                }
            }
        }
    }  
}

void Timer::stop() {
    running = false;
}

bool Timer::add_event(std::shared_ptr<TimerEvent> &event) {
    events.push_back(event);
    get_next_event();
    return true;
}

bool Timer::delete_event(int idx) {
    events.erase(events.begin() + idx);
    get_next_event();
    return true;
}

bool Timer::disable_event(int idx) {
    events[idx]->disable();
    get_next_event();
    return true;
}

bool Timer::enable_event(int idx) {
    events[idx]->enable();
    get_next_event();
    return true;
}

std::shared_ptr<TimerEvent> Timer::get_event(int idx) {
    return events[idx];
}

void Timer::get_next_event() {
    if (events.size() == 0) {
        next_event = -1;
        std::cout << "No events" << std::endl;
    } else {
        next_event = -1;
        time_t t = time(0);
        struct tm * now = localtime( & t );
        int h = now->tm_hour;
        int m = now->tm_min;
        int last_diff = 9999;

        int counter = 0;
        for (auto &ev : events) {
            if (ev->is_enabled()) {
                int diff = time_diff(h, m, ev->get_hour(), ev->get_minute());

                if (diff > 0 && diff < last_diff) {
                    last_diff = diff;
                    next_event = counter;

                }
            }
            counter++;
        }
    }

    save_events();
}

int Timer::time_diff(int hour1, int minute1, int hour2, int minute2) {
    int mins1 = (hour1 * 60) + minute1;
    int mins2 = (hour2 * 60) + minute2;
    
    if (mins2 > mins1) {
        return mins2 - mins1;
    }
    else {
        return ((mins2 + (60 * 24)) - mins1);
    }
}

const std::vector<std::shared_ptr<TimerEvent>> Timer::get_events() const{
    return events;
}

void Timer::save_events() {
    std::ofstream f;
    f.open("timers.txt");
    
    std::ostringstream oss;

    for (unsigned x = 0; x < events.size(); x++)
    {

        events[x]->to_stringstream(oss);
    }

    f << oss.str();
    f.close();
}
