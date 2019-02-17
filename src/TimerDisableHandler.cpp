#include "TimerDisableHandler.hpp"

TimerDisableHandler::TimerDisableHandler(Timer* timer_) {
	timer = timer_;
}

bool TimerDisableHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	using namespace std;
	using boost::format;
	
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");

	AuthHandler auth = AuthHandler();
	if (auth.authorised(conn)) {
		string content;
		string param = "";
		if (CivetServer::getParam(conn, "id", param))
		{
			if (timer -> disableTimerEvent(atoi(param.c_str())))
			{
				content = "Timer disabled";
			}
			else
			{
				content = "There was an erroreroror";
			}
		}

		string html = str( format(ReadHtml::readHtml("html/TimerDisableHandler/get.html")) % content);
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
