#include "TimerDeleteHandler.hpp"

TimerDeleteHandler::TimerDeleteHandler(Timer& timer_) : timer(timer_) {}

bool TimerDeleteHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");	

	AuthHandler auth = AuthHandler();
	std::string role = "admin";
    if (auth.authorised(conn, role)) {
		std::string s = "";
		std::string content;
		
		if (CivetServer::getParam(conn, "id", s)) {
			std::string idString = s;
			
			int id = atoi(idString.c_str());
			if (timer.delete_event(id)) {
				content = "Timer deleterised";
			} else {
				content = "Error deleterising timer";
			}
		}
		
		std::string html = boost::str(boost::format(ReadHtml::readHtml("html/TimerDeleteHandler/get.html")) % content);
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