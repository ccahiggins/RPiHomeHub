#include "ChartHandler.hpp"

ChartCreator chartCreator;

bool ChartHandler::handleGet(CivetServer *server, struct mg_connection *conn) {

	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	AuthHandler auth = AuthHandler();
	if (auth.authorised(conn)) {
		std::string days;
		std::string to;
		std::string from;
		std::string chart;
		
		bool foundFrom = CivetServer::getParam(conn, "from", from);
		bool foundTo = CivetServer::getParam(conn, "to", to);
		bool foundDays = CivetServer::getParam(conn, "days", days);
		
		if (foundFrom && foundTo) {
			chart = chartCreator.getChartFromTo(from, to);
		}
		else if (foundFrom && foundDays) {
			chart = chartCreator.getChartFromDays(from, days);
		} else if (foundDays) {
			if (days.compare("1") == 0) {
				chart = ReadHtml::readHtml("chartdata");
			} else {
				chart = chartCreator.getChartDays(days);
			}
		}
		else {
			chart = ReadHtml::readHtml("chartdata");
			//days="1";
			//chart = chartCreator.getChartDays(days);
		}
		
		mg_printf(conn, "%s", chart.c_str());
	} else {
		const struct mg_request_info *req_info = mg_get_request_info(conn);
		std::string uri = std::string(req_info->local_uri);
		std::string html = ReadHtml::readHtml("html/auth/pleaselogin.html");
		std::string s = boost::str(boost::format(html) % uri  );
		mg_printf(conn, s.c_str());
	}
	return true;
}



