#include "TimerAddHandler.hpp"

TimerAddHandler::TimerAddHandler(Timer* timer_) {
	timer = timer_;
}

bool TimerAddHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	using namespace std;
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	
	string html = ReadHtml::readHtml("html/TimerAddHandler/get.html");
	mg_printf(conn, html.c_str());
	
	return true;
}
	
bool TimerAddHandler::handlePost(CivetServer *server, struct mg_connection *conn) {
	using namespace std;
	using boost::format;
	using boost::io::group;

	string s = "";
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");

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
	
	string message = addTimer(startHour, startMinute, duration, boilerItem, oneTime);

	string html = str( format(ReadHtml::readHtml("html/TimerAddHandler/post.html")) % message);
	mg_printf(conn, html.c_str());
	
	return true;
}

std::string TimerAddHandler::addTimer(int hour, int minute, int duration, int boilerItem, bool onetime) {
	using namespace std;
	if (timer -> addTimerEvent(hour, minute, duration, boilerItem, true, onetime)) {
		std::ostringstream oss;
		oss << "Added timer: ";
		if (boilerItem == BOILER_ITEM_WATER) {
			oss << "water on for ";
		} else {
			oss << "heating on for ";
		}
		oss << duration << " minutes " << " at " << hour << ":" << minute;
		
		return oss.str();
	}
	else {
		return "Error";
	}
}