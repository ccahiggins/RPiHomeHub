#include "TimerDisableHandler.hpp"

TimerDisableHandler::TimerDisableHandler(Timer* timer_) {
	timer = timer_;
}

bool TimerDisableHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	using namespace std;
	using boost::format;
	using boost::io::group;
	
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");

	string content;
	string param = "";
	if (CivetServer::getParam(conn, "id", param))
	{
		if (timer -> disableTimerEvent(atoi(param.c_str())))
		{
			content = "Timer disabled";
		}
		else
		{
			content = "There was an erroreroror";
		}
	}

	string html = str( format(ReadHtml::readHtml("html/TimerDisableHandler/get.html")) % content);
	mg_printf(conn, html.c_str());
	
	return true;
}
