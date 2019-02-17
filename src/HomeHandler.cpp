#include "HomeHandler.hpp"

bool HomeHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	using namespace std;
	using boost::format;	
	
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	AuthHandler auth = AuthHandler();
	
	if (auth.authorised(conn)) {
		string html = ReadHtml::readHtml("html/api/index.html");
		mg_printf(conn, html.c_str());
	} else {
		const struct mg_request_info *req_info = mg_get_request_info(conn);
		string uri = string(req_info->local_uri);
		string html = ReadHtml::readHtml("html/auth/pleaselogin.html");
		string s = str( format(html) % uri  );
		mg_printf(conn, s.c_str());
	}
	
	return true;
}