#include "HomeHandler.hpp"

bool HomeHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	AuthHandler auth = AuthHandler();
	
	if (auth.authorised(conn)) {
		std::string html = ReadHtml::readHtml("html/api/index.html");
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