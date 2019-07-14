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
	} else if (second.compare("heating") == 0) {
		if (third.compare("on") == 0) {
			if (validNum(fourth, 1, 1000)) {
				this->duration=fourth;
				requestType = RequestType::HeatingOn;
				return true;
			}
		} else if (third.compare("off") == 0) {
			requestType = RequestType::HeatingOff;
			return true;
		}
	} else if (second.compare("status") == 0) {
		requestType = RequestType::Status;
		return true;
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
	} else if (second.compare("temperature") == 0) {
		if (third.compare("bedroom") == 0 || third.compare("livingroom") == 0 || third.compare("outside") == 0) {
			requestType = RequestType::TemperatureOne;
			this->room = third;
			return true;
		} else if (third.empty()) {
			requestType = RequestType::TemperatureAll;
			return true;
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

