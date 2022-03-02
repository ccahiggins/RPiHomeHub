#include "HubHandler.hpp"

HubHandler::HubHandler(Boiler& boiler_, TempSensorController& tempSens_) :
	boiler(boiler_), tempSens(tempSens_){}

bool HubHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
		
	std::string content = "";
	
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nStrict-Transport-Security: max-age=31536000\r\n");

	AuthHandler auth = AuthHandler();
	if (auth.authorised(conn)) {
    mg_printf(conn,"\r\n");
		std::vector<int> lowBatts = tempSens.lowBattery();
		if (lowBatts.size() > 0) {
			content.append("Low Battery");
			for(std::vector<int>::iterator it = lowBatts.begin(); it != lowBatts.end(); ++it) {
				content.append(std::to_string(*it));
			}
		}

		//Get heating and water status
		//int waterStatus = boiler->WaterStatus();
		//int heatingStatus = boiler->HeatingStatus();
		Boiler::boiler_status boilerStatus = boiler.getBoilerStatus();
		
		int waterStatus = boilerStatus.waterStatus;
		int heatingStatus = boilerStatus.heatingStatus;
			
		if (waterStatus == ERROR) {
			content.append(ReadHtml::readHtml("html/HubHandler/cantCommunicate.html"));
		}
		else {
			if (waterStatus == OFF) {
				content.append(ReadHtml::readHtml("html/HubHandler/waterOn.html"));
			}
			else if (waterStatus == ON) {
				content.append(ReadHtml::readHtml("html/HubHandler/turnWaterOn.html"));
			}
			else if (waterStatus > 0) {
				content.append(boost::str(boost::format(ReadHtml::readHtml("html/HubHandler/waterOnFor.html")) % waterStatus));
			}
			
		}
		if (heatingStatus == ERROR) {
			content.append(ReadHtml::readHtml("html/HubHandler/cantCommunicate.html"));
		}
		else
		{
			if (heatingStatus == OFF) {
				content.append(ReadHtml::readHtml("html/HubHandler/heatingOn.html"));
			}
			else if (heatingStatus == ON) {
				content.append(ReadHtml::readHtml("html/HubHandler/turnHeatingOn.html"));
			}
			else if (heatingStatus > 0) {
				content.append(boost::str(boost::format(ReadHtml::readHtml("html/HubHandler/heatingOnFor.html")) % heatingStatus));
			}
			
		}
		
		std::string days;
		std::string from;
		std::string to;
		bool foundDays = CivetServer::getParam(conn, "days", days);
		bool foundFrom = CivetServer::getParam(conn, "from", from);
		bool foundTo = CivetServer::getParam(conn, "to", to);
		
		if (foundFrom && foundTo) {
			days = "0";
		} else if (foundFrom && foundDays) {
			to = "";
		} else if (foundDays) {
			from = "";
			to = "";
		} else {
			from = "";
			to = "";
			days = "0";
		}

		std::string html = boost::str(boost::format(ReadHtml::readHtml("html/HubHandler/html.html")) % content % days % from % to);
		
		mg_printf(conn, html.c_str());
	} else {
		mg_printf(conn,"X-XSS-Protection: 0\r\n");
		mg_printf(conn,"X-Frame-Options: DENY\r\n");
		mg_printf(conn,"X-Content-Type-Options: nosniff\r\n");
		mg_printf(conn,"Content-Security-Policy: default-src 'self'; frame-ancestors 'self'; form-action 'self';\r\n");
		mg_printf(conn,"\r\n");
		const struct mg_request_info *req_info = mg_get_request_info(conn);
		std::string uri = std::string(req_info->local_uri);
		std::string html = ReadHtml::readHtml("html/auth/pleaselogin.html");
		std::string s = boost::str(boost::format(html) % uri  );
		mg_printf(conn, s.c_str());
	}
	return true;
}
