#include "TimerHandler.hpp"

TimerHandler::TimerHandler(Timer& timer_) : timer(timer_) {}

bool TimerHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	
	AuthHandler auth = AuthHandler();
	if (auth.authorised(conn)) {
		//html.append(readHtml.readHtml("html/TimerHandler/head.html"));
			
		std::string content = "";
		
		std::vector<std::shared_ptr<TimerEvent>> timers = timer.get_events();
		for (unsigned x = 0; x < timers.size(); x++)
		{
			std::shared_ptr<BoilerTimerEvent> boiler_event;
			std::shared_ptr<ThermostatTimerEvent> thermostat_event;

			if (boiler_event = std::dynamic_pointer_cast<BoilerTimerEvent> (timers[x])) {
				std::string whichone;
				std::string endis;
				std::string enDis;
				if (boiler_event->get_item() == 0) {
					whichone="Water";
				} else {
					whichone="Heating";
				}
				if (boiler_event->is_enabled()) {
					endis = "disable";
					enDis = "Disable";
				} else {
					endis = "enable";
					enDis = "Enable";
				}
				std::string line = ReadHtml::readHtml("html/TimerHandler/enableDisable.html");

				std::string s = boost::str(boost::format(line) % whichone % boiler_event->get_hour() % boiler_event->get_minute() % boiler_event->get_duration() % enDis % x % endis % x );
				content.append(s);
			} else if (thermostat_event = std::dynamic_pointer_cast<ThermostatTimerEvent> (timers[x])) {
				std::string whichone;
				std::string endis;
				std::string enDis;
				if (thermostat_event->get_on_off() == 0) {
					whichone="ON";
				} else {
					whichone="OFF";
				}
				if (thermostat_event->is_enabled()) {
					endis = "disable";
					enDis = "Disable";
				} else {
					endis = "enable";
					enDis = "Enable";
				}
				std::string line = ReadHtml::readHtml("html/TimerHandler/enableDisable.html");

				std::string s = boost::str(boost::format(line) % whichone % thermostat_event->get_hour() % thermostat_event->get_minute() % thermostat_event->get_room() % enDis % x % endis % x );
				content.append(s);
			}
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