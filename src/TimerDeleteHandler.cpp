#include "TimerDeleteHandler.hpp"

TimerDeleteHandler::TimerDeleteHandler(Timer* timer_) {
	timer = timer_;
}

bool TimerDeleteHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	using namespace std;
	using boost::format;
	using boost::io::group;
	
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");	

	string s = "";
	string content;
	
	if (CivetServer::getParam(conn, "id", s))
	{
		string idString = s;
		
		int id = atoi(idString.c_str());
		if (timer->removeTimerEvent(id)) {
			content = "Timer deleterised";
		} else {
			content = "Error deleterising timer";
		}
	}
	
	string html = str( format(ReadHtml::readHtml("html/TimerDeleteHandler/get.html")) % content);
	mg_printf(conn, html.c_str());
	
	return true;
}