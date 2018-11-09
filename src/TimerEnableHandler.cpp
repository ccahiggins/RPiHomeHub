#include "TimerEnableHandler.hpp"

TimerEnableHandler::TimerEnableHandler(Timer* timer_) {
	timer = timer_;
}

bool TimerEnableHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	using namespace std;
	using boost::format;
	using boost::io::group;
		
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");

	string content;
	string param = "";
	if (CivetServer::getParam(conn, "id", param)) {
		if (timer->enableTimerEvent(atoi(param.c_str()))) {
			content = "Timer enabled";
		} else {
			content = "There was an erroreroror";
		}
	}

	string html = str( format(ReadHtml::readHtml("html/TimerEnableHandler/get.html")) % content);
	mg_printf(conn, html.c_str());

	return true;
}