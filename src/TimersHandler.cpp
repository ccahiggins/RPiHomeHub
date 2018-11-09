#include "TimersHandler.hpp"

TimersHandler::TimersHandler(Timer* timer_) {
	timer = timer_;
}

bool TimersHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	using namespace std;
	using boost::format;
	using boost::io::group;
	
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	
	//html.append(readHtml.readHtml("html/TimersHandler/head.html"));
		
	string content = "";
	
	vector<Timer::TimerEvent>& timers = timer->getTimers();
	for (unsigned x=0; x<timers.size(); x++)
	{
		string whichone;
		string endis;
		string enDis;
		if (timers[x].boilerItem == 0) {
			whichone="Water";
		} else {
			whichone="Heating";
		}
		if (timers[x].enabled) {
			endis = "disable";
			enDis = "Disable";
		} else {
			endis = "enable";
			enDis = "Enable";
		}
		string line = ReadHtml::readHtml("html/TimersHandler/enableDisable.html");

		string s = str( format(line) % whichone % timers[x].startHour % timers[x].startMinute% timers[x].duration % enDis % timers[x].id % endis % timers[x].id );
		content.append(s);
	}
	
	string html = ReadHtml::readHtml("html/TimersHandler/html.html");
	string s = str( format(html) % content  );
	mg_printf(conn, s.c_str());

	return true;
}