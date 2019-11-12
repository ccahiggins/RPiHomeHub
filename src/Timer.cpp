#include "Timer.hpp"

#include <iostream>
#include <thread>
#include <chrono>

std::thread th;
bool running = false;


Timer::Timer() {
}

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

        const std::shared_ptr<TimerEvent> e = events.at(next_event);

        if (next_event >= 0) {
            if (h == e->get_hour()) {
                if (m == e->get_minute()) {
                    if (d > e->get_trig_day()) {
                        e->trigger();
                    }
                }
            }
        }
    }  
}

void Timer::stop() {
    running = false;
}

void Timer::add_event(std::shared_ptr<TimerEvent> &event) {
    events.push_back(event);
    get_next_event();
}

void Timer::delete_event(int idx) {
    get_next_event();
}

std::shared_ptr<TimerEvent> Timer::get_event(int idx) {
    return events[idx];
}

void Timer::get_next_event() {
    if (events.size() == 0) {
        next_event = 0;
    } else {
        next_event = -1;
        time_t t = time(0);
        struct tm * now = localtime( & t );
        int h = now->tm_hour;
        int m = now->tm_min;
        int last_diff = 9999;

        int counter = 0;
        for (auto &ev : events) {
            int diff = time_diff(h, m, ev->get_hour(), ev->get_minute());

            if (diff > 0 && diff < last_diff) {
                last_diff = diff;
                next_event = counter;

                counter++;
            }
        }
    }
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