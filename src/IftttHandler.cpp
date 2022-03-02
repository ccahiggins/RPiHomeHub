#include "IftttHandler.hpp"

IftttHandler::IftttHandler(Boiler& boiler_, Thermostat& thermostat_, Timer& timer_, TempSensorController& tempSens_, std::vector<Sonoff> _sonoff_list)
	: boiler(boiler_), thermostat(thermostat_), timer(timer_), tempSens(tempSens_) {
		
		for (auto &s : _sonoff_list)
		{  			
			sonoff_list.push_back(s);
		}
	}

std::string IftttHandler::getTemp(std::string id) {
	
	std::string temp = DatabaseController::getTempFromId(id);
	
	return temp;
}

std::string IftttHandler::getTimers() {
	
	std::string timerjson = "{\"timers\":[";
	std::vector<std::shared_ptr<TimerEvent>> timers = timer.get_events();
	for (unsigned x=0; x < timers.size(); x++) {
		std::shared_ptr<BoilerTimerEvent> boiler_event;
		std::shared_ptr<ThermostatTimerEvent> thermostat_event;

		if (boiler_event = std::dynamic_pointer_cast<BoilerTimerEvent> (timers[x])) {
			std::string whichone;
			std::string endis;
			std::string onetime;
			if (boiler_event->get_item() == 0) {
				whichone = "water";
			} else {
				whichone = "heating";
			}
			if (boiler_event->is_enabled()) {
				endis = "true";
			} else {
				endis = "false";
			}
			
			if (boiler_event->is_one_time()) {
				onetime = "true";
			} else {
				onetime = "false";
			}
			timerjson.append("{\"id\":\"" + std::to_string(x) + "\", \"type\":\"" + "boiler" + "\", \"boiler_item\":\"" + whichone + "\", \"hours\":\"" + std::to_string(boiler_event->get_hour()) + "\", \"minutes\":\"" +  std::to_string(boiler_event->get_minute()) + "\",\"duration\":\"" + std::to_string(boiler_event->get_duration()) + "\", \"onetime\":\"" + onetime + "\", \"enabled\":\"" + endis + "\"");
			
			//string s = str( format(line) % whichone % timers[x].startHour % timers[x].startMinute% timers[x].duration % enDis % timers[x].id % endis % timers[x].id );
			//content.append(s);
			timerjson.append("}");
			if (!(x == timers.size() - 1)) {
				timerjson.append(",");
			}
		} else if (thermostat_event = std::dynamic_pointer_cast<ThermostatTimerEvent> (timers[x])) {
			std::string endis;
			std::string onetime;
			std::string room;
			std::string temp;
			std::string on_off;

			if (thermostat_event->is_enabled()) {
				endis = "true";
			} else {
				endis = "false";
			}

			if (thermostat_event->get_on_off()) {
				on_off = "on";
			} else {
				on_off = "off";
			}

			if (thermostat_event->get_room() == 1) {
				room = "Bedroom";
			} else if (thermostat_event->get_room() == 2) {
				room = "Office";
			} else if (thermostat_event->get_room() == 3) {
				room = "Kitchen";
			} else if (thermostat_event->get_room() == 4) {
				room = "Living Room";
			}

			temp = std::to_string(thermostat_event->get_temp());

			if (thermostat_event->is_one_time()) {
				onetime = "true";
			} else {
				onetime = "false";
			}

			timerjson.append("{\"id\":\"" + std::to_string(x) + "\", \"type\":\"" + "thermostat" + "\", \"room\":\"" + room + "\", \"temp\":\"" + temp + "\", \"on_off\":\"" + on_off + "\", \"hours\":\"" + std::to_string(thermostat_event->get_hour()) + "\", \"minutes\":\"" +  std::to_string(thermostat_event->get_minute()) + "\", \"onetime\":\"" + onetime + "\", \"enabled\":\"" + endis + "\"");
			
			//string s = str( format(line) % whichone % timers[x].startHour % timers[x].startMinute% timers[x].duration % enDis % timers[x].id % endis % timers[x].id );
			//content.append(s);
			timerjson.append("}");
			if (!(x == timers.size() - 1)) {
				timerjson.append(",");
			}
		}
	}
	timerjson.append("]}");
	//cout << timerjson << endl;
	
	return timerjson;
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
	
	std::string requestKey="";
	CivetServer::getParam(conn, "apikey", requestKey);
	if (!apiKeyMatch(requestKey)) {
		mg_printf(conn, "{\"status\":\"invalidkey\"}");
		return true;
	}
	
	bool change = false;
	
	const struct mg_request_info *req_info = mg_get_request_info(conn);
	RequestValidator rv(req_info->request_uri);
	
	if (rv.getType() == RequestType::WaterOn) {
		if (boiler.TurnWaterOn(std::stoi(rv.getDuration()))) {
			//cout << "google turned the water on" << endl;
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"error\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::WaterOff) {
		if (boiler.TurnWaterOff()) {
			//cout << "google turned the water off" << endl;
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"error\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::HeatingOn) {
		if (boiler.TurnHeatingOn(std::stoi(rv.getDuration()))) {
			//cout << "google turned the heating on" << endl;
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"error\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::HeatingOff) {
		if (boiler.TurnHeatingOff()) {
			//cout << "google turned the heating off" << endl;
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::Status) {
			Boiler::boiler_status boilerStatus = boiler.getBoilerStatus();
	
			int waterStatus = boilerStatus.waterStatus;
			int heatingStatus = boilerStatus.heatingStatus;
			
			if (waterStatus == ERROR || heatingStatus == ERROR) {
				//mg_printf(conn, "{\"status\":\"error\"}");

				std::string batteryJson;
				if (lowBatts.size() > 0) {
					batteryJson = ",\"lowbattery\":[";
					bool first = true;
					for(std::vector<int>::iterator it = lowBatts.begin(); it != lowBatts.end(); ++it) {
						if (first) {
							first = false;
						} else {
							batteryJson.append(",");
						}
						batteryJson.append(std::to_string(*it));
						//content.append(to_string(*it));
					}
					batteryJson.append("]");
				} else {
					batteryJson = "";
				}

				std::string result = "{\"status\":\"error\"" + batteryJson + "}";
				mg_printf(conn, result.c_str());

			} else {
				std::string waterJson;
				std::string heatingJson;
				std::string batteryJson;
				
				if (waterStatus == -1) {
					waterJson = "\"water\":{\"status\":\"off\"}";
				} else {
					waterJson = "\"water\":{\"status\":\"on\",\"duration\":\"" + std::to_string(waterStatus) + "\"}";
				}
				
				if (heatingStatus == -1) {
					heatingJson = "\"heating\":{\"status\":\"off\"}";
				} else {
					heatingJson = "\"heating\":{\"status\":\"on\",\"duration\":\"" + std::to_string(heatingStatus) + "\"}";
				}
				
				if (lowBatts.size() > 0) {
					batteryJson = ",\"lowbattery\":[";
					bool first = true;
					for(std::vector<int>::iterator it = lowBatts.begin(); it != lowBatts.end(); ++it) {
						if (first) {
							first = false;
						} else {
							batteryJson.append(",");
						}
						batteryJson.append(std::to_string(*it));
						//content.append(to_string(*it));
					}
					batteryJson.append("]");
				} else {
					batteryJson = "";
				}
				
				std::string result = "{\"status\":\"done\"," + waterJson + "," + heatingJson + batteryJson + "}";
				
				mg_printf(conn, result.c_str());
			}
			
	} else if (rv.getType() == RequestType::TemperatureAll) {
		//cout << "TempAll" << endl;
		mg_printf(conn, "{\"error\":\"notyetimplemented\"}");
	} else if (rv.getType() == RequestType::TemperatureOne) {
		//cout << "TempOne" << endl;
		std::string id;
		//cout << rv.getRoom() << endl;
		if (rv.getRoom().compare("livingroom") == 0) {
			id = "1";
		} else if (rv.getRoom().compare("bedroom") == 0) {
			id = "2";
		} else if (rv.getRoom().compare("outside") == 0) {
			id = "4";
		}
		std::string temp = getTemp(id);
		std::string result = "{\"status\":\"done\",\"temperature\":\"" + temp + "\"}";
		mg_printf(conn, result.c_str());
	} else if (rv.getType() == RequestType::Timers) {
		std::string result = getTimers();
		mg_printf(conn, result.c_str());
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
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
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
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
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
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::TimersDelete) {
		if (timer.delete_event(stoi(rv.getTimerId()))) {
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::TimersEnable) {
		if (timer.enable_event(stoi(rv.getTimerId()))) {
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::TimersDisable) {
		if (timer.disable_event(stoi(rv.getTimerId()))) {
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::SonoffOn) {
		if (sonoff_list[stoi(rv.get_sonoff_switch_num())].turn_on() == 1) {
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::SonoffOff) {
		if (sonoff_list[stoi(rv.get_sonoff_switch_num())].turn_off() == 0) {
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
		}
	} else {
		mg_printf(conn, "{\"error\":\"invalidrequest\"}");
	}

	if (change) {
		system("./push");
	}
	
	return true;
}
