#include "EmailerHandler.hpp"

EmailerHandler::EmailerHandler(Emailer& emailer_) : emailer(emailer_) {}

bool EmailerHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	
	std::string s = "";
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	
	AuthHandler auth = AuthHandler();
	if (auth.authorised(conn)) {
		std::string content = "";
		
		if (CivetServer::getParam(conn, "param", s)) {
			if (s.find("state") != std::string::npos) {
				if (s.find("off") != std::string::npos) {
					emailer.turn_emailer_off();
					content.append(ReadHtml::readHtml("html/EmailerHandler/emailer_off.html"));
				} else if (s.find("on") != std::string::npos) {
					emailer.turn_emailer_on();
					content.append(ReadHtml::readHtml("html/EmailerHandler/emailer_on.html"));
				}
			} else if (s.find("temp") != std::string::npos) {
				if (std::string::npos != s.find_first_of("0123456789")) {
					std::string tempString = s.substr(s.find_first_of("0123456789"), s.length());

					float temp = stof(tempString);

					emailer.set_trigger_temp(temp);
					content.append(boost::str(boost::format(ReadHtml::readHtml("html/EmailerHandler/set_temp.html")) % tempString));
				}
			} else {
				mg_printf(conn, "NO VALID PARAM!\r\n");
			}
		} else {
			if (emailer.is_emailer_on()) {
				content.append(ReadHtml::readHtml("html/EmailerHandler/emailerOn.html"));
			} else {
				content.append(ReadHtml::readHtml("html/EmailerHandler/emailerOff.html"));
			}
			std::string temp_string = std::to_string(emailer.get_trigger_temp());

			content.append(boost::str(boost::format(ReadHtml::readHtml("html/EmailerHandler/blah.html")) % temp_string));
		}
		
		std::string html = boost::str(boost::format(ReadHtml::readHtml("html/EmailerHandler/html.html")) % content);
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