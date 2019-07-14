#include "TimerHandler.hpp"

TimerHandler::TimerHandler(Timer& timer_) : timer(timer_) {}

bool TimerHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	
	AuthHandler auth = AuthHandler();
	if (auth.authorised(conn)) {
		//html.append(readHtml.readHtml("html/TimerHandler/head.html"));
			
		std::string content = "";
		
		std::vector<Timer::TimerEvent>& timers = timer.getTimers();
		for (unsigned x=0; x<timers.size(); x++)
		{
			std::string whichone;
			std::string endis;
			std::string enDis;
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
			std::string line = ReadHtml::readHtml("html/TimerHandler/enableDisable.html");

			std::string s = boost::str(boost::format(line) % whichone % timers[x].startHour % timers[x].startMinute% timers[x].duration % enDis % timers[x].id % endis % timers[x].id );
			content.append(s);
		}
		
		std::string html = ReadHtml::readHtml("html/TimerHandler/html.html");
		std::string s = boost::str(boost::format(html) % content  );
		mg_printf(conn, s.c_str());
	} else {
		const struct mg_request_info *req_info = mg_get_request_info(conn);
		std::string uri = std::string(req_info->local_uri);
		std::string html = ReadHtml::readHtml("html/auth/pleaselogin.html");
		std::string s = boost::str(boost::format(html) % uri  );
		mg_printf(conn, s.c_str());
	}
	return true;
}