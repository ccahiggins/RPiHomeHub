#include "ThermostatHandler.hpp"

ThermostatHandler::ThermostatHandler(Thermostat& thermostat_) : thermostat(thermostat_) {}

bool ThermostatHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	
	std::string s = "";
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	
	AuthHandler auth = AuthHandler();
	if (auth.authorised(conn)) {
		std::string content = "";
		
		if (CivetServer::getParam(conn, "param", s)) {
			if (s.find("state") != std::string::npos) {
				if (s.find("off") != std::string::npos) {
					thermostat.turn_thermostat_off();
					mg_printf(conn, "Turning thermostat off");
				} else if (s.find("on") != std::string::npos) {
					thermostat.turn_thermostat_on();
					mg_printf(conn, "Turning thermostat on");
				}
			}
		}
		
		//std::string html = boost::str(boost::format(ReadHtml::readHtml("html/ThermostatHandler/html.html")) % content);
		//mg_printf(conn, html.c_str());
		mg_printf(conn, "Thermostat out!");
	} else {
		const struct mg_request_info *req_info = mg_get_request_info(conn);
		std::string uri = std::string(req_info->local_uri);
		std::string html = ReadHtml::readHtml("html/auth/pleaselogin.html");
		std::string s = boost::str(boost::format(html) % uri  );
		mg_printf(conn, s.c_str());
	}
	return true;
}