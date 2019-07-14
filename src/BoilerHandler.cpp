#include "BoilerHandler.hpp"

BoilerHandler::BoilerHandler(Boiler& boiler_) : boiler(boiler_) {}

bool BoilerHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	
	std::string s = "";
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	
	AuthHandler auth = AuthHandler();
	if (auth.authorised(conn)) {
		std::string content = "";
		
		if (CivetServer::getParam(conn, "param", s)) {
			if (s.find("water") != std::string::npos) {
				if (s.find("off") != std::string::npos) {
					if (boiler.TurnWaterOff()) {
						content.append(ReadHtml::readHtml("html/BoilerHandler/waterOff.html"));
					} else {
						content.append(ReadHtml::readHtml("html/BoilerHandler/cantCommunicate.html"));
					}
				} else if (s.find("on") != std::string::npos) {
					if (std::string::npos != s.find_first_of("0123456789")) {
						std::string durationString = s.substr(7, 10);
						int duration = atoi(durationString.c_str());
						
						if (boiler.TurnWaterOn(duration)) {
							content.append(boost::str(boost::format(ReadHtml::readHtml("html/BoilerHandler/waterOnFor.html")) % duration));
						} else {
							content.append(ReadHtml::readHtml("html/BoilerHandler/cantCommunicate.html"));
						}
					} else {
						if (boiler.TurnWaterOn()) {
							content.append(ReadHtml::readHtml("html/BoilerHandler/waterOn.html"));
						} else {
							content.append(ReadHtml::readHtml("html/BoilerHandler/cantCommunicate.html"));
						}
					}
				} else {
					content.append(ReadHtml::readHtml("html/BoilerHandler/paramNotSet.html"));
				}
			} else if (s.find("heating") != std::string::npos) {
				if (s.find("off") != std::string::npos) {
					if (boiler.TurnHeatingOff()) {
						content.append(ReadHtml::readHtml("html/BoilerHandler/heatingOff.html"));
					} else {
						content.append(ReadHtml::readHtml("html/BoilerHandler/cantCommunicate.html"));
					}
				} else if (s.find("on") != std::string::npos) {
					if (std::string::npos != s.find_first_of("0123456789")) {
						std::string durationString = s.substr(9, 10);
						int duration = atoi(durationString.c_str());
						
						if (boiler.TurnHeatingOn(duration)) {
							content.append(boost::str(boost::format(ReadHtml::readHtml("html/BoilerHandler/heatingOnFor.html")) % duration));
						} else {
							content.append(ReadHtml::readHtml("html/BoilerHandler/cantCommunicate.html"));
						}
					} else {
						if (boiler.TurnHeatingOn()) {
							content.append(ReadHtml::readHtml("html/BoilerHandler/heatingOn.html"));
						} else {
							content.append(ReadHtml::readHtml("html/BoilerHandler/cantCommunicate.html"));
						}
					}
				} else {
					content.append(ReadHtml::readHtml("html/BoilerHandler/paramNotSet.html"));
				}
			} else {
				content.append(ReadHtml::readHtml("html/BoilerHandler/paramNotSet.html"));
			}
		} else {
			content.append(ReadHtml::readHtml("html/BoilerHandler/paramNotSet.html"));
		}
		
		std::string html = boost::str(boost::format(ReadHtml::readHtml("html/BoilerHandler/html.html")) % content);
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