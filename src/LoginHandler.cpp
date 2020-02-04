#include "LoginHandler.hpp"
#include <iostream>
#include <cstring>

bool LoginHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	
	std::string uri = "";
	CivetServer::getParam(conn, "uri", uri);
	boost::replace_all(uri, "&", "&amp;");
	boost::replace_all(uri, "<", "&lt;");
	boost::replace_all(uri, ">", "&gt;");
	boost::replace_all(uri, "\"","&quot;");
	boost::replace_all(uri, "'", "&#x27;");
	boost::replace_all(uri, "/", "&#x2F;");
	boost::erase_all(uri, "%s");
	boost::erase_all(uri, "%n");
	boost::erase_all(uri, "%d");
	boost::erase_all(uri, "%i");
	boost::erase_all(uri, "%E");
	boost::erase_all(uri, "%f");
	boost::erase_all(uri, "%G");
	boost::erase_all(uri, "%o");
	boost::erase_all(uri, "%u");
	boost::erase_all(uri, "%x");
	boost::erase_all(uri, "%X");
	boost::erase_all(uri, "%P");

	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	std::string html = ReadHtml::readHtml("html/auth/login.html");
	std::string s = boost::str(boost::format(html) % uri  );
	const char* cccc = s.c_str();
	mg_printf(conn, cccc);
	return true;
}

bool LoginHandler::handlePost(CivetServer *server, struct mg_connection *conn) {

	AuthHandler auth = AuthHandler();
	
	//mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	std::string content = "Test POST";
	
	std::string session;
	std::string uri;
	if (auth.login(conn, session, uri)) {
		
		//string session = auth.getSession(conn);

        // Send reply to the client, showing submitted form values.
        mg_printf(conn,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nSet-Cookie: session=");
		mg_printf(conn, session.c_str());
		mg_printf(conn, "; secure; httpOnly;\r\n\r\n");

		//mg_printf(conn,"HTTP/1.1 303 See Other\r\n");
		//mg_printf(conn, "Location: ");
		//mg_printf(conn, uri.c_str());
		//mg_printf(conn, "\r\n\r\n");
		//mg_printf(conn, "Set-Cookie: session=");
		//mg_printf(conn, session.c_str());
		//mg_printf(conn, "; secure; httpOnly;\r\n\r\n");
		
		
		std::string html = ReadHtml::readHtml("html/auth/loggedin.html");
		//std::string s = boost::str(boost::format(html) % uri % uri );
		std::string s = boost::str(boost::format(html) % uri);
		mg_printf(conn, s.c_str());
    } else {
        // Show HTML form.
		mg_printf(conn,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
		std::string html = ReadHtml::readHtml("html/auth/unauthorised.html");
		mg_printf(conn, html.c_str());
    }
	return true;
}