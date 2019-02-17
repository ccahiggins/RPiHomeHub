#include "TimerHandler.hpp"

TimerHandler::TimerHandler(Timer* timer_) {
	timer = timer_;
}

bool TimerHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	using namespace std;
	using boost::format;
	
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	
	AuthHandler auth = AuthHandler();
	if (auth.authorised(conn)) {
		//html.append(readHtml.readHtml("html/TimerHandler/head.html"));
			
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
			string line = ReadHtml::readHtml("html/TimerHandler/enableDisable.html");

			string s = str( format(line) % whichone % timers[x].startHour % timers[x].startMinute% timers[x].duration % enDis % timers[x].id % endis % timers[x].id );
			content.append(s);
		}
		
		string html = ReadHtml::readHtml("html/TimerHandler/html.html");
		string s = str( format(html) % content  );
		mg_printf(conn, s.c_str());
	} else {
		const struct mg_request_info *req_info = mg_get_request_info(conn);
		string uri = string(req_info->local_uri);
		string html = ReadHtml::readHtml("html/auth/pleaselogin.html");
		string s = str( format(html) % uri  );
		mg_printf(conn, s.c_str());
	}
	return true;
}