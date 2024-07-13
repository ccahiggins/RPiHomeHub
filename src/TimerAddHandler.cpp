#include "TimerAddHandler.hpp"

TimerAddHandler::TimerAddHandler(Timer& timer_, Boiler& boiler_, Thermostat& thermostat_) : timer(timer_), boiler(boiler_), thermostat(thermostat_) {}

bool TimerAddHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	
	AuthHandler auth = AuthHandler();
	std::string role = "admin";
    if (auth.authorised(conn, role)) {
		std::string html = ReadHtml::readHtml("html/TimerAddHandler/get.html");
		std::vector<std::vector<std::string>> sensors = Sensors::getSensors();
		bool first = true;
		for ( auto &i : sensors ) {
			if (i[3].compare("1") == 0 && first) {
				html.append(boost::str(boost::format(ReadHtml::readHtml("html/TimerAddHandler/first_room.html")) % i[0] % i[1]));
				first = false;
			} else if (i[3].compare("1") == 0 && !first) {
				html.append(boost::str(boost::format(ReadHtml::readHtml("html/TimerAddHandler/other_rooms.html")) % i[0] % i[1]));
			}
		}
		html.append(ReadHtml::readHtml("html/TimerAddHandler/get2.html"));
		
		
		mg_printf(conn, html.c_str());
	} else {
		const struct mg_request_info *req_info = mg_get_request_info(conn);
		std::string uri = std::string(req_info->local_uri);
		std::string html = ReadHtml::readHtml("html/auth/pleaselogin.html");
		std::string s = boost::str(boost::format(html) % uri  );
		mg_printf(conn, s.c_str());
	}
	return true;
}
	
bool TimerAddHandler::handlePost(CivetServer *server, struct mg_connection *conn) {

	std::string s = "";
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	AuthHandler auth = AuthHandler();
	std::string role = "admin";
    if (auth.authorised(conn, role)) {
		if (CivetServer::getParam(conn, "timer_type", s) && s.compare("boiler") == 0) {
			int startHour = 0;
			int startMinute = 0;
			int duration = 0;
			int boilerItem = 0;
			bool oneTime = true;
			if (CivetServer::getParam(conn, "starttime", s)) {
				startHour = atoi(s.substr(0, 2).c_str());
				startMinute = atoi(s.substr(3, 2).c_str());
			}
			if (CivetServer::getParam(conn, "duration", s)) {
				duration = atoi(s.c_str());
			}
			if (CivetServer::getParam(conn, "boileritem", s)) {
				boilerItem = atoi(s.c_str());
			}
			if (CivetServer::getParam(conn, "onetime", s)) {
				if (s.compare("1") == 0) {
					oneTime=true;
				} else if (s.compare("0") == 0) {
					oneTime=false;
				}
			}

			std::string message = addBoilerTimer(startHour, startMinute, duration, boilerItem, oneTime);
			std::string html = boost::str(boost::format(ReadHtml::readHtml("html/TimerAddHandler/post.html")) % message);
			mg_printf(conn, html.c_str());
		} else if (CivetServer::getParam(conn, "timer_type", s) && s.compare("thermostat") == 0) {
			int startHour = 0;
			int startMinute = 0;
			int room = 0;
			float temp = 0;
			bool on_off = true;
			bool oneTime = true;
			if (CivetServer::getParam(conn, "starttime_thermostat", s)) {
				startHour = stoi(s.substr(0, 2));
				startMinute = stoi(s.substr(3, 2));
			}
			if (CivetServer::getParam(conn, "room", s)) {
				room = stoi(s);
			}
			if (CivetServer::getParam(conn, "temperature", s)) {
				temp = stof(s);
			}
			if (CivetServer::getParam(conn, "on_off", s)) {
				if (s.compare("on") == 0) {
					on_off = true;
				} else {
					on_off = false;
				}
			}
			if (CivetServer::getParam(conn, "onetime_thermostat", s)) {
				if (s.compare("1") == 0) {
					oneTime=true;
				} else if (s.compare("0") == 0) {
					oneTime=false;
				}
			}

			std::string message = addThermostatTimer(startHour, startMinute, on_off, room, temp, oneTime);
			std::string html = boost::str(boost::format(ReadHtml::readHtml("html/TimerAddHandler/post.html")) % message);
			mg_printf(conn, html.c_str());
		}
	} else {
		const struct mg_request_info *req_info = mg_get_request_info(conn);
		std::string uri = std::string(req_info->local_uri);
		std::string html = ReadHtml::readHtml("html/auth/pleaselogin.html");
		std::string s = boost::str(boost::format(html) % uri  );
		mg_printf(conn, s.c_str());
	}
	return true;
}

std::string TimerAddHandler::addBoilerTimer(int hour, int minute, int duration, int boilerItem, bool onetime) {

	std::shared_ptr<TimerEvent> event(new BoilerTimerEvent(hour, minute, onetime, boilerItem, duration, boiler));
	if (timer.add_event(event)) {
		std::ostringstream oss;
		oss << "Added timer: ";
		if (boilerItem == BOILER_ITEM_WATER) {
			oss << "water on for ";
		} else {
			oss << "heating on for ";
		}
		oss << duration << " minutes " << " at " << hour << ":" << minute;
		
		return oss.str();
	} else {
		return "Error";
	}
}

std::string TimerAddHandler::addThermostatTimer(int hour, int minute, bool on_off, int room, float temperature, bool onetime) {

	std::shared_ptr<TimerEvent> event(new ThermostatTimerEvent(hour, minute, onetime, on_off, room, temperature, thermostat));
	if (timer.add_event(event)) {
		std::ostringstream oss;
		oss << "Added timer: ";
		if (on_off) {
			oss << "Thermostat on in ";
			oss << Sensors::getName(room);
			oss << " at " << temperature << "C";
		} else {
			oss << "Thermostat off ";
		}

		oss << " at " << hour << ":" << minute;
		return oss.str();
	} else {
		return "Error";
	}
}
