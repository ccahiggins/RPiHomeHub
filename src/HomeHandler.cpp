#include "HomeHandler.hpp"

HomeHandler::HomeHandler(std::string& api_key_, std::string& api_url_, std::string& ga_key_) : api_key(api_key_), api_url(api_url_), ga_key(ga_key_) {}

bool HomeHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	
    mg_printf(conn,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nStrict-Transport-Security: max-age=31536000\r\n");

	AuthHandler auth = AuthHandler();
	
	if (auth.authorised(conn)) {
    	mg_printf(conn,"\r\n");
		//content.append(boost::str(boost::format(ReadHtml::readHtml("html/HubHandler/waterOnFor.html")) % waterStatus));
		//std::string html = ReadHtml::readHtml("html/api/index.html");
		std::string html = boost::str(boost::format(ReadHtml::readHtml("html/api/index.html")) % ga_key % api_url % api_url % api_key %api_url % api_key % api_url % api_key % api_url % api_key % api_url % api_key % api_url % api_key % api_url % api_key % api_url % api_key % api_url % api_key);
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
