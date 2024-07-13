#include "RequestValidator.hpp"

RequestValidator::RequestValidator(const char* _request) {
	request = _request;
	requestType = RequestType::None;
	this->valid = validateRequest(request);
}
RequestType RequestValidator::getType() {
	return this->requestType;
}

std::string RequestValidator::getDuration() {
	return this->duration;
}

std::string RequestValidator::getRoom() {
	return this->room;
}
std::string RequestValidator::getHour() {
	return this->hour;
}
std::string RequestValidator::getMinute() {
	return this->minute;
}
std::string RequestValidator::getTimerId() {
	return this->timerid;
}
std::string RequestValidator::getOneTime() {
	return this->onetime;
}
std::string RequestValidator::getOnOff() {
	return this->onoff;
}
std::string RequestValidator::getTemp() {
	return this->temp;
}
std::string RequestValidator::get_sonoff_switch_num() {
	return this->sonoff_switch_num;
}


bool RequestValidator::isValid() {
	return this->valid;
}
	
bool RequestValidator::validNum(std::string numTxt, int minnum, int maxnum) {
		
	if (validNum(numTxt)) {
		int num = std::stoi(numTxt);
		
		if (num >= minnum && num <= maxnum) {
			return true;
		}
	}
	
	return false;
}
	
bool RequestValidator::validNum(std::string numTxt) {

	try {
		std::stoi(numTxt);
	} catch (const std::invalid_argument &ex) {
		return false;
	} catch (const std::out_of_range &ex) {
		return false;
	}

  return true;
}

bool RequestValidator::valid_float(std::string num_text, int min_num, int max_num) {

	if (valid_float(num_text)) {
		int num = std::stof(num_text);
		
		if (num >= min_num && num <= max_num) {
			return true;
		}
	}
	
	return false;
}

bool RequestValidator::valid_float(std::string num_text) {

  try {
    std::stof(num_text);
  } catch(const std::invalid_argument &ex) {
    return false;
  }

  return true;
}

//RequestValidator::RequestType requestType;

bool RequestValidator::validateRequest(std::string request) {

	
	//cout << request << endl;
	std::string path = request;
	std::string first = pathPart(path, 1);
	std::string second = pathPart(path, 2);
	std::string third = pathPart(path, 3);
	std::string fourth = pathPart(path, 4);
	
	//cout << "1:" << first << ",2:"<< second << ",3:"<< third << ",4:"<< fourth << endl;
	
	//Water - on / off
	if (second.compare("water") == 0) {
		if (third.compare("on") == 0) {
			if (validNum(fourth, 1, 1000)) {
				this->duration=fourth;
				requestType = RequestType::WaterOn;
				return true;
			}
		} else if (third.compare("off") == 0) {
			requestType = RequestType::WaterOff;
			//cout << "Setting off" << endl;
			return true;
		}

	//Heating - on / off
	} else if (second.compare("heating") == 0) {
		if (third.compare("on") == 0) {
			if (validNum(fourth, 1, 1000)) {
				requestType = RequestType::HeatingOn;
				this->duration=fourth;
				return true;
			}
		} else if (third.compare("off") == 0) {
			requestType = RequestType::HeatingOff;
			return true;
		}

	//Thermostat - on/off/room/temp
	} else if (second.compare("thermostat") == 0) {
		if (third.compare("on") == 0) {
			requestType = RequestType::ThermostatOn;
			return true;
		} else if (third.compare("off") == 0) {
			requestType = RequestType::ThermostatOff;
			return true;
		} else if (third.compare("room") == 0) {
			if (validNum(third, MIN_ROOM, MAX_ROOM)) {
				requestType = RequestType::ThermostatRoom;
				this->room = third;
				return true;
			}
		} else if (third.compare("temp") == 0) {
			if (valid_float(temp, MIN_THERMOSTAT_TEMP, MAX_THERMOSTAT_TEMP)) {
				requestType = RequestType::ThermostatTemp;
				this->temp = third;
				return true;
			}
		}

	//Status
	} else if (second.compare("status") == 0) {
		requestType = RequestType::Status;
		return true;

	//Timers - status/addwater/addheating/enable/disable/delete,addthermostat
	} else if (second.compare("timers") == 0) {
		if (third.compare("status") == 0) {
			requestType = RequestType::Timers;
			return true;
		} else if (third.compare("addwater") == 0) {
			//hh-mm-onetime-duration
			//1259
			std::string hour = fourth.substr(0,2);
			std::string minute = fourth.substr(2,2);
			std::string onetime = fourth.substr(4,1);
			std::string duration = fourth.substr(5,fourth.length() - 1);
			if (validNum(hour, 0, 23)) {
				if (validNum(minute, 0, 59)) {
					if (validNum(duration, 1, 1000)) {
						if (onetime.compare("f") == 0 || onetime.compare("t") == 0) {
							requestType = RequestType::TimersAddWater;
							this->hour = hour;
							this->minute = minute;
							this->onetime = onetime;
							this->duration = duration;
							return true;
						}
					}
				}
			}
		} else if (third.compare("addheating") == 0) {
			std::string hour = fourth.substr(0,2);
			std::string minute = fourth.substr(2,2);
			std::string onetime = fourth.substr(4,1);
			std::string duration = fourth.substr(5,fourth.length() - 1);
			if (validNum(hour, 0, 23)) {
				if (validNum(minute, 0, 59)) {
					if (validNum(duration, 1, 1000)) {
						if (onetime.compare("f") == 0 || onetime.compare("t") == 0) {
							requestType = RequestType::TimersAddHeating;
							this->hour = hour;
							this->minute = minute;
							this->onetime = onetime;
							this->duration = duration;
							return true;
						}
					}
				}
			}
		} else if (third.compare("addthermostat") == 0) {
			std::string hour = fourth.substr(0,2);
			std::string minute = fourth.substr(2,2);
			std::string onetime = fourth.substr(4,1);
			std::string onoff = fourth.substr(5,1);
			std::string room = fourth.substr(6,1);
			std::string temp = fourth.substr(7,fourth.length() - 1);
			if (validNum(hour, 0, 23)) {
				if (validNum(minute, 0, 59)) {
					if (validNum(room, 1, 5)) {
						if (valid_float(temp, 0, 100)) {
							if (onetime.compare("f") == 0 || onetime.compare("t") == 0) {
								if (onoff.compare("f") == 0 || onoff.compare("t") == 0) {
									requestType = RequestType::TimersAddThermostat;
									this->hour = hour;
									this->minute = minute;
									this->onetime = onetime;
									this->onoff = onoff;
									this->room = room;
									this->temp = temp;
									return true;
								}
							}
						}
					}
				}
			}
		} else if (third.compare("enable") == 0) {
			if (validNum(fourth, 0, 10000)) {
				requestType = RequestType::TimersEnable;
				this->timerid = fourth;
				return true;
			}
			
		} else if (third.compare("disable") == 0) {
			if (validNum(fourth, 0, 10000)) {
				requestType = RequestType::TimersDisable;
				this->timerid = fourth;
				return true;
			}
			
		} else if (third.compare("delete") == 0) {
			if (validNum(fourth, 0, 10000)) {
				requestType = RequestType::TimersDelete;
				this->timerid = fourth;
				return true;
			}
			
		}

	//Temperature
	} else if (second.compare("temperature") == 0) {
		if (third.compare("bedroom") == 0 || third.compare("livingroom") == 0 || third.compare("outside") == 0) {
			requestType = RequestType::TemperatureOne;
			this->room = third;
			return true;
		} else if (third.empty()) {
			requestType = RequestType::TemperatureAll;
			return true;
		}
		
	//Sonoff sockets - all/single status - switch on/off/toggle
	} else if (second.compare("sonoff") == 0) {
		if (third.empty()) {
			requestType = RequestType::SonoffAllStatus;
			return true;
		} else if (fourth.empty()) {
			if (validNum(third, 0, 2)) {
				requestType = RequestType::SonoffSingleStatus;
				this->sonoff_switch_num = third;
				return true;
			}
		}
		if (validNum(third, 0, 2)) {
			if (fourth.compare("on") == 0) {
				requestType = RequestType::SonoffOn;
				this->sonoff_switch_num = third;
				return true;
			}
			if (fourth.compare("off") == 0) {
				requestType = RequestType::SonoffOff;
				this->sonoff_switch_num = third;
				return true;
			}
			if (fourth.compare("toggle") == 0) {
				requestType = RequestType::SonoffToggle;
				this->sonoff_switch_num = third;
				return true;
			}
		}
	}
	
	return false;
}

std::string RequestValidator::pathPart(std::string str, int num) {
	
	std::string delimeter = "/";

	std::string start;
	std::string end = str.substr(1, str.length());
	
	if (str.find(delimeter) == std::string::npos) {
		return "";
	} else {
		for (int i = 0; i < num; i++) {
			size_t pos = end.find(delimeter);
			if (pos > end.length() || pos == std::string::npos) {
				if (!end.empty()) {
					start = end.substr(0, end.length());
					end = "";
				} else {
					start = "";
				}
			} else {
				start = end.substr(0, pos);
				end = end.substr(pos +1 , end.length());
			}
		}
	}
	return start;
}

