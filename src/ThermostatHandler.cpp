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
					content.append(ReadHtml::readHtml("html/ThermostatHandler/thermostat_off.html"));
				} else if (s.find("on") != std::string::npos) {
					thermostat.turn_thermostat_on();
					content.append(ReadHtml::readHtml("html/ThermostatHandler/thermostat_on.html"));
				}
			} else if (s.find("temp") != std::string::npos) {
				if (std::string::npos != s.find_first_of("0123456789")) {
					std::string tempString = s.substr(s.find_first_of("0123456789"), s.length());

					float temp = stof(tempString);

					thermostat.set_temperature(temp);
					content.append(boost::str(boost::format(ReadHtml::readHtml("html/ThermostatHandler/set_temp.html")) % tempString));

				}
			} else if (s.find("room") != std::string::npos) {
				if (std::string::npos != s.find_first_of("0123456789")) {
					std::string room_string = s.substr(s.find_first_of("0123456789"), s.length());
					int room = stoi(room_string);
					if (room == 1) {
						room_string = "Bedroom";
					} else if (room == 2) {
						room_string = "Living room";
					}

					thermostat.set_room(room);
					content.append(boost::str(boost::format(ReadHtml::readHtml("html/ThermostatHandler/set_room.html")) % room_string));
				}
			} else {

				mg_printf(conn, "NO VALID PARAM!\r\n");
			}
		} else {
			if (thermostat.is_thermostat_on()) {
				content.append(ReadHtml::readHtml("html/ThermostatHandler/thermostatOn.html"));
			} else {
				content.append(ReadHtml::readHtml("html/ThermostatHandler/thermostatOff.html"));
			}
			int room = thermostat.get_selected_room();
			std::string room_string = "";
			if (room == 1) {
				room_string = "Bedroom";
			} else if (room == 2) {
				room_string = "Living room";
			}

			std::string temp_string = std::to_string(thermostat.get_selected_temp());
			content.append(boost::str(boost::format(ReadHtml::readHtml("html/ThermostatHandler/blah.html")) % temp_string % room_string));
		}
		
		std::string html = boost::str(boost::format(ReadHtml::readHtml("html/ThermostatHandler/html.html")) % content);
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