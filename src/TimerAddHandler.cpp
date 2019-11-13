#include "TimerAddHandler.hpp"

TimerAddHandler::TimerAddHandler(Timer& timer_, Boiler& boiler_) : timer(timer_), boiler(boiler_) {}

bool TimerAddHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	
	AuthHandler auth = AuthHandler();
		if (auth.authorised(conn)) {
		std::string html = ReadHtml::readHtml("html/TimerAddHandler/get.html");
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
	if (auth.authorised(conn)) {

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
			}
			else if (s.compare("0") == 0) {
				oneTime=false;
			}
		}
		
		std::string message = addTimer(startHour, startMinute, duration, boilerItem, oneTime);

		std::string html = boost::str(boost::format(ReadHtml::readHtml("html/TimerAddHandler/post.html")) % message);
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

std::string TimerAddHandler::addTimer(int hour, int minute, int duration, int boilerItem, bool onetime) {

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