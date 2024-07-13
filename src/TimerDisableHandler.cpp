#include "TimerDisableHandler.hpp"

TimerDisableHandler::TimerDisableHandler(Timer& timer_) : timer(timer_) {}

bool TimerDisableHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");

	AuthHandler auth = AuthHandler();
	std::string role = "admin";
    if (auth.authorised(conn, role)) {
		std::string content;
		std::string param = "";
		if (CivetServer::getParam(conn, "id", param)) {
			if (timer.disable_event(atoi(param.c_str()))) {
				content = "Timer disabled";
			}
			else {
				content = "There was an erroreroror";
			}
		}

		std::string html = boost::str(boost::format(ReadHtml::readHtml("html/TimerDisableHandler/get.html")) % content);
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
