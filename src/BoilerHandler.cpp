#include "BoilerHandler.hpp"



BoilerHandler::BoilerHandler(Boiler* boiler_){
	boiler = boiler_;
}

bool BoilerHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	using namespace std;
	using boost::format;
	using boost::io::group;
	
	string s = "";
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	
	string content = "";
	
	if (CivetServer::getParam(conn, "param", s)) {
		if (s.find("water") != string::npos) {
			if (s.find("off") != string::npos) {
				if (boiler -> TurnWaterOff()) {
					content.append(ReadHtml::readHtml("html/BoilerHandler/waterOff.html"));
				} else {
					content.append(ReadHtml::readHtml("html/BoilerHandler/cantCommunicate.html"));
				}
			} else if (s.find("on") != string::npos) {
				if (string::npos != s.find_first_of("0123456789")) {
					string durationString = s.substr(7, 10);
					int duration = atoi(durationString.c_str());
					
					if (boiler -> TurnWaterOn(duration)) {
						content.append(str( format(ReadHtml::readHtml("html/BoilerHandler/waterOnFor.html")) % duration));
					} else {
						content.append(ReadHtml::readHtml("html/BoilerHandler/cantCommunicate.html"));
					}
				} else {
					if (boiler->TurnWaterOn()) {
						content.append(ReadHtml::readHtml("html/BoilerHandler/waterOn.html"));
					} else {
						content.append(ReadHtml::readHtml("html/BoilerHandler/cantCommunicate.html"));
					}
				}
			} else {
				content.append(ReadHtml::readHtml("html/BoilerHandler/paramNotSet.html"));
			}
		} else if (s.find("heating") != string::npos) {
			if (s.find("off") != string::npos) {
				if (boiler->TurnHeatingOff()) {
					content.append(ReadHtml::readHtml("html/BoilerHandler/heatingOff.html"));
				} else {
					content.append(ReadHtml::readHtml("html/BoilerHandler/cantCommunicate.html"));
				}
			} else if (s.find("on") != string::npos) {
				if (string::npos != s.find_first_of("0123456789")) {
					string durationString = s.substr(9, 10);
					int duration = atoi(durationString.c_str());
					
					if (boiler->TurnHeatingOn(duration)) {
						content.append(str( format(ReadHtml::readHtml("html/BoilerHandler/heatingOnFor.html")) % duration));
					} else {
						content.append(ReadHtml::readHtml("html/BoilerHandler/cantCommunicate.html"));
					}
				} else {
					if (boiler->TurnHeatingOn()) {
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
	
	string html = str( format(ReadHtml::readHtml("html/BoilerHandler/html.html")) % content);
	mg_printf(conn, html.c_str());

	return true;
}