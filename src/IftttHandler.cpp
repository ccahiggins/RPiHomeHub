#include "IftttHandler.hpp"

IftttHandler::IftttHandler(Boiler &boiler_, Thermostat &thermostat_, Timer &timer_, TempSensorController &tempSens_, std::vector<Sonoff> _sonoff_list)
    : boiler(boiler_), thermostat(thermostat_), timer(timer_), tempSens(tempSens_) {

    for (auto &s : _sonoff_list) {
        sonoff_list.push_back(s);
    }
}

std::string IftttHandler::getTemp(std::string id) {

    std::string temp = DatabaseController::getTempFromId(id);

    return temp;
}

std::vector<nlohmann::json> IftttHandler::getTimers() {
    std::vector<nlohmann::json> timers_json;

    std::vector<std::shared_ptr<TimerEvent>> timers = timer.get_events();
    for (unsigned x = 0; x < timers.size(); x++) {
        nlohmann::json timer_json_item;
        std::shared_ptr<BoilerTimerEvent> boiler_event;
        std::shared_ptr<ThermostatTimerEvent> thermostat_event;

        if (boiler_event = std::dynamic_pointer_cast<BoilerTimerEvent>(timers[x])) {
            timer_json_item["id"] = x;
            timer_json_item["type"] = "boiler";
            timer_json_item["boiler_item"] = (boiler_event->get_item() == 0) ? "water" : "heating";
            timer_json_item["hours"] = boiler_event->get_hour();
            timer_json_item["minutes"] = boiler_event->get_minute();
            timer_json_item["duration"] = boiler_event->get_duration();
            timer_json_item["onetime"] = boiler_event->is_one_time();
            timer_json_item["enabled"] = boiler_event->is_enabled();

            timers_json.push_back(timer_json_item);

        } else if (thermostat_event = std::dynamic_pointer_cast<ThermostatTimerEvent>(timers[x])) {
            timer_json_item["id"] = x;
            timer_json_item["type"] = "thermostat";
            timer_json_item["room"] = DatabaseController::getSensorName(thermostat_event->get_room());
            timer_json_item["temp"] = thermostat_event->get_temp();
            timer_json_item["on_off"] = thermostat_event->get_on_off();
            timer_json_item["hours"] = thermostat_event->get_hour();
            timer_json_item["minutes"] = thermostat_event->get_minute();
            timer_json_item["onetime"] = thermostat_event->is_one_time();
            timer_json_item["enabled"] = thermostat_event->is_enabled();

            timers_json.push_back(timer_json_item);
        }
    }

    return timers_json;
}

bool IftttHandler::apiKeyMatch(std::string requestKey) {

    std::string key;
    std::string key2;
    std::ifstream file;
    file.open("apikey");
    std::getline(file, key);
    std::getline(file, key2);

    if (key.compare(requestKey) == 0 || key2.compare(requestKey) == 0) {
        return true;
    } else {
        return false;
    }
}

bool IftttHandler::handlePost(CivetServer *server, struct mg_connection *conn) {

    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n");

    std::vector<int> lowBatts = tempSens.lowBattery();

    std::string requestKey = "";
    CivetServer::getParam(conn, "apikey", requestKey);

    nlohmann::json j;

    if (!apiKeyMatch(requestKey)) {
        j["status"] = "invalidkey";
    } else {

        bool change = false;

        const struct mg_request_info *req_info = mg_get_request_info(conn);
        RequestValidator rv(req_info->request_uri);

        if (rv.getType() == RequestType::WaterOn) {
            if (boiler.TurnWaterOn(std::stoi(rv.getDuration()))) {
                j["status"] = "done";
                change = true;
            } else {
                j["status"] = "error";
            }
        } else if (rv.getType() == RequestType::WaterOff) {
            if (boiler.TurnWaterOff()) {
                j["status"] = "done";
                change = true;
            } else {
                j["status"] = "error";
            }
        } else if (rv.getType() == RequestType::HeatingOn) {
            if (boiler.TurnHeatingOn(std::stoi(rv.getDuration()))) {
                j["status"] = "done";
                change = true;
            } else {
                j["status"] = "error";
            }
        } else if (rv.getType() == RequestType::HeatingOff) {
            if (boiler.TurnHeatingOff()) {
                j["status"] = "done";
                change = true;
            } else {
                j["status"] = "error";
            }
        } else if (rv.getType() == RequestType::Status) {
            Boiler::boiler_status boilerStatus = boiler.getBoilerStatus();

            int waterStatus = boilerStatus.waterStatus;
            int heatingStatus = boilerStatus.heatingStatus;

            if (waterStatus == ERROR || heatingStatus == ERROR) {
                j["status"] = "error";
                j["lowbattery"] = lowBatts;
            } else {
                std::string waterJson;
                std::string heatingJson;
                std::string batteryJson;

                if (waterStatus == -1) {
                    // waterJson = "\"water\":{\"status\":\"off\"}";
                    j["water"]["status"] = "off";
                } else {
                    j["water"]["status"] = "on";
                    j["water"]["duration"] = waterStatus;
                }

                if (heatingStatus == -1) {
                    j["heating"]["status"] = "off";
                } else {
                    j["heating"]["status"] = "on";
                    j["heating"]["duration"] = heatingStatus;
                }

                if (lowBatts.size() > 0) {
                    j["lowbattery"] = lowBatts;
                }

                j["status"] = "done";
            }
        } else if (rv.getType() == RequestType::TemperatureAll) {
            std::vector<nlohmann::json> temps;
            for (int i = 1; i <= 5; i++) {
                nlohmann::json temp;
                std::string name = DatabaseController::getSensorName(i);

                temp["name"] = name;
                temp["id"] = i;
                temp["temp"] = getTemp(std::to_string(i));
                temps.push_back(temp);
            }

            j["temperatures"] = temps;
        } else if (rv.getType() == RequestType::TemperatureOne) {
            std::string id;
            if (rv.getRoom().compare("livingroom") == 0) {
                id = "1";
            } else if (rv.getRoom().compare("bedroom") == 0) {
                id = "2";
            } else if (rv.getRoom().compare("outside") == 0) {
                id = "4";
            }
            std::string temp = getTemp(id);
            j["status"] = "done";
            j["temperature"] = temp;
        } else if (rv.getType() == RequestType::Timers) {
            j["timers"] = getTimers();
        } else if (rv.getType() == RequestType::TimersAddWater) {
            bool onetime = true;
            if (rv.getOneTime().compare("f") == 0) {
                onetime = false;
            } else if (rv.getOneTime().compare("t") == 0) {
                onetime = true;
            }

            int hour = stoi(rv.getHour());
            int minute = stoi(rv.getMinute());
            int duration = stoi(rv.getDuration());

            std::shared_ptr<TimerEvent> ev(new BoilerTimerEvent(hour, minute, onetime, 0, duration, boiler));
            if (timer.add_event(ev)) {
                j["status"] = "done";
                change = true;
            } else {
                j["status"] = "error";
            }
        } else if (rv.getType() == RequestType::TimersAddHeating) {
            bool onetime = true;
            if (rv.getOneTime().compare("f") == 0) {
                onetime = false;
            } else if (rv.getOneTime().compare("t") == 0) {
                onetime = true;
            }

            int hour = stoi(rv.getHour());
            int minute = stoi(rv.getMinute());
            int duration = stoi(rv.getDuration());

            std::shared_ptr<TimerEvent> ev(new BoilerTimerEvent(hour, minute, onetime, 1, duration, boiler));
            if (timer.add_event(ev)) {
                j["status"] = "done";
                change = true;
            } else {
                j["status"] = "error";
            }
        } else if (rv.getType() == RequestType::TimersAddThermostat) {
            bool onetime = true;
            if (rv.getOneTime().compare("f") == 0) {
                onetime = false;
            } else if (rv.getOneTime().compare("t") == 0) {
                onetime = true;
            }

            bool on_off = true;
            if (rv.getOnOff().compare("f") == 0) {
                on_off = false;
            } else if (rv.getOnOff().compare("t") == 0) {
                on_off = true;
            }

            int hour = stoi(rv.getHour());
            int minute = stoi(rv.getMinute());
            int room = stoi(rv.getRoom());
            float temp = stof(rv.getTemp());

            std::shared_ptr<TimerEvent> ev(new ThermostatTimerEvent(hour, minute, onetime, on_off, room, temp, thermostat));
            if (timer.add_event(ev)) {
                j["status"] = "done";
                change = true;
            } else {
                j["status"] = "error";
            }
        } else if (rv.getType() == RequestType::TimersDelete) {
            if (timer.delete_event(stoi(rv.getTimerId()))) {
                j["status"] = "done";
                change = true;
            } else {
                j["status"] = "error";
            }
        } else if (rv.getType() == RequestType::TimersEnable) {
            if (timer.enable_event(stoi(rv.getTimerId()))) {
                j["status"] = "done";
                change = true;
            } else {
                j["status"] = "error";
            }
        } else if (rv.getType() == RequestType::TimersDisable) {
            if (timer.disable_event(stoi(rv.getTimerId()))) {
                j["status"] = "done";
                change = true;
            } else {
                j["status"] = "error";
            }
        } else if (rv.getType() == RequestType::SonoffOn) {
            if (sonoff_list[stoi(rv.get_sonoff_switch_num())].turn_on() == 1) {
                j["status"] = "done";
                change = true;
            } else {
                j["status"] = "error";
            }
        } else if (rv.getType() == RequestType::SonoffOff) {
            if (sonoff_list[stoi(rv.get_sonoff_switch_num())].turn_off() == 0) {
                j["status"] = "done";
                change = true;
            } else {
                j["status"] = "error";
            }
        } else if (rv.getType() == RequestType::SonoffAllStatus) {
            std::vector<nlohmann::json> sonoff_statuses;
            for (int i = 0; i < 3; i++) {
                nlohmann::json sonofffff;
                int sonoff_status = sonoff_list[i].status();
                sonofffff["id"] = std::to_string(i);
                if (sonoff_status == 0) {
                    sonofffff["status"] = "off";
                } else if (sonoff_status == 1) {
                    sonofffff["status"] = "on";
                } else {
                    sonofffff["status"] = "error";
                }
                sonoff_statuses.push_back(sonofffff);
            }
            j["sonoffs"] = sonoff_statuses;

        } else if (rv.getType() == RequestType::SonoffSingleStatus) {
            int sonoff_status = sonoff_list[stoi(rv.get_sonoff_switch_num())].status();
            j["id"] = rv.get_sonoff_switch_num();
            if (sonoff_status == 0) {
                j["status"] = "off";
            } else if (sonoff_status == 1) {
                j["status"] = "on";
            } else {
                j["status"] = "error";
            }
        } else {
            j["error"] = "invalidrequest";
        }

        if (change) {
            system("./push");
        }
    }

    std::string response = j.dump();
    mg_printf(conn, response.c_str());
    return true;
}
