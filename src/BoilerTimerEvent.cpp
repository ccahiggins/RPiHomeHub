#include "BoilerTimerEvent.hpp"

#include <iostream>
#include <sstream>

BoilerTimerEvent::BoilerTimerEvent(int hour_, int minute_, bool one_time_, int item_, int duration_, Boiler &boiler_) : TimerEvent(hour_, minute_, one_time_), item(item_), duration(duration_), boiler(boiler_) {
}

void BoilerTimerEvent::trigger_event() {
    if (item == 0) {
        boiler.TurnWaterOn(duration);
    } else if (item == 1) {
        boiler.TurnHeatingOn(duration);
    }
}

int BoilerTimerEvent::get_item() {
    return item;
}
int BoilerTimerEvent::get_duration() {
    return duration;
}

void BoilerTimerEvent::to_stringstream(std::ostringstream &oss) {
    oss << "BOILER" << std::endl;
    oss << get_hour() << std::endl;
    oss << get_minute() << std::endl;
    oss << is_one_time() << std::endl;
    oss << is_enabled() << std::endl;
    oss << get_trig_hour() << std::endl;
    oss << get_trig_min() << std::endl;
    oss << get_trig_day() << std::endl;
    oss << get_item() << std::endl;
    oss << get_duration() << std::endl;
}
