#include "SonoffHandler.hpp"

SonoffHandler::SonoffHandler(std::vector<Sonoff> _sonoff_list) {
	
	for (auto &s : _sonoff_list)
		{  			
			sonoff_list.push_back(s);
		}
}

bool SonoffHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	
	std::string s = "";
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	
	AuthHandler auth = AuthHandler();
	if (auth.authorised(conn)) {
		std::string content = "";
		std::string param1 = "";
		std::string param2 = "";
		std::string html = "";
		if (CivetServer::getParam(conn, "turn", param1)) {
			if (CivetServer::getParam(conn, "id", param2)) {
				if (param1.compare("on") == 0) {
					int id = atoi(param2.c_str());
					int res = sonoff_list[id].turn_on();
					if (res == 0) {
						content.append(boost::str(boost::format(ReadHtml::readHtml("html/SonoffHandler/switch.html")) % "ON" % "off" % id % "Turn OFF"));
					} else {
						content.append(boost::str(boost::format(ReadHtml::readHtml("html/SonoffHandler/switch.html")) % "ERROR" % "off" % id % "ERROR"));
					}
				} else if (param1.compare("off") == 0) {
					int id = atoi(param2.c_str());
					int res = sonoff_list[id].turn_off();
					if (res == 0) {
						content.append(boost::str(boost::format(ReadHtml::readHtml("html/SonoffHandler/switch.html")) % "OFF" % "on" % id % "Turn ON"));
					} else {
						content.append(boost::str(boost::format(ReadHtml::readHtml("html/SonoffHandler/switch.html")) % "ERROR" % "on" % id % "ERROR"));
					}
				}
			}
			html = boost::str(boost::format(ReadHtml::readHtml("html/SonoffHandler/html.html")) % content  % "sonoff" % "Sonoff");
		} else {
			int cnt = 0;
			for (auto &s : sonoff_list)
			{  
				int status = s.status();
				if (status == 0) {
					//content.append("\nOFF");
					content.append(boost::str(boost::format(ReadHtml::readHtml("html/SonoffHandler/status.html")) % "OFF" % "on" % cnt % "Turn ON"));
				} else if (status == 1) {
					content.append(boost::str(boost::format(ReadHtml::readHtml("html/SonoffHandler/status.html")) % "ON" % "off" % cnt % "Turn OFF"));
				} else {
					content.append(boost::str(boost::format(ReadHtml::readHtml("html/SonoffHandler/status.html")) % "ERROR" % "ERROR" % cnt % "ERROR"));
				}
				cnt++;
			}
			
			html = boost::str(boost::format(ReadHtml::readHtml("html/SonoffHandler/html.html")) % content  % "hub" % "Hub");
		}
		
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

void SonoffHandler::add_sonoff(Sonoff& sonoff) {
	
	sonoff_list.push_back(sonoff);
}