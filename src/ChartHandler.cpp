#include "ChartHandler.hpp"

bool ChartHandler::handleGet(CivetServer *server, struct mg_connection *conn) {

    mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	AuthHandler auth = AuthHandler();
	std::string role = "admin";
    if (auth.authorised(conn, role)) {
        std::string days;
        std::string to;
        std::string from;
        std::string chart;
        std::string chart_type;

        bool foundFrom = CivetServer::getParam(conn, "from", from);
        bool foundTo = CivetServer::getParam(conn, "to", to);
        bool foundDays = CivetServer::getParam(conn, "days", days);
        bool chart_type_found = CivetServer::getParam(conn, "type", chart_type);

        if (foundFrom && foundTo) {
            if (!chart_type_found || chart_type.compare("google") == 0) {
                ChartCreator chartCreator;
                chart = chartCreator.getChartFromTo(from, to);
            } else if (chart_type.compare("google_json") == 0) {
                ChartCreatorJson chartCreator;
                chart = chartCreator.get_chart_from_to(from, to);
            } else if (chart_type.compare("nvd3") == 0) {
                ChartCreatorNvd3 chartCreator;
                chart = chartCreator.get_chart_from_to(from, to);
            } else if (chart_type.compare("google_array") == 0) {
                ChartCreatorGoogleArray chartCreator;
                chart = chartCreator.get_chart_from_to(from, to);
            }
        } else if (foundFrom && foundDays) {
            if (!chart_type_found || chart_type.compare("google") == 0) {
                ChartCreator chartCreator;
                chart = chartCreator.getChartFromDays(from, days);
            } else if (chart_type.compare("google_json") == 0) {
                ChartCreatorJson chartCreator;
                chart = chartCreator.get_chart_from_days(from, days);
            } else if (chart_type.compare("nvd3") == 0) {
                ChartCreatorNvd3 chartCreator;
                chart = chartCreator.get_chart_from_days(from, days);
            } else if (chart_type.compare("dygraphs") == 0) {
                ChartCreatorDygraphs chartCreator;
                chart = chartCreator.get_chart_from_days(from, days);
            } else if (chart_type.compare("google_array") == 0) {
                ChartCreatorGoogleArray chartCreator;
                chart = chartCreator.get_chart_from_days(from, days);
            }
        } else if (foundDays) {
            if (!chart_type_found || chart_type.compare("google") == 0) {
                if (days.compare("1") == 0) {
                    chart = ReadHtml::readHtml("chartdata");
                } else {
                    ChartCreator chartCreator;
                    chart = chartCreator.getChartDays(days);
                }
            } else if (chart_type.compare("google_json") == 0) {
                ChartCreatorJson chartCreator;
                chart = chartCreator.get_chart_days(days);
            } else if (chart_type.compare("nvd3") == 0) {
                ChartCreatorJson chartCreator;
                chart = chartCreator.get_chart_days(days);
            } else if (chart_type.compare("dygraphs") == 0) {
                ChartCreatorDygraphs chartCreator;
                chart = chartCreator.get_chart_days(days);
            } else if (chart_type.compare("google_array") == 0) {
                ChartCreatorGoogleArray chartCreator;
                chart = chartCreator.get_chart_days(days);
            }
        } else {
            if (!chart_type_found || chart_type.compare("google") == 0) {
                chart = ReadHtml::readHtml("chartdata");
            } else if (chart_type.compare("google_json") == 0) {
                days = "1";
                ChartCreatorJson chartCreator;
                chart = chartCreator.get_chart_days(days);
            } else if (chart_type.compare("nvd3") == 0) {
                days = "1";
                ChartCreatorNvd3 chartCreator;
                chart = chartCreator.get_chart_days(days);
            } else if (chart_type.compare("dygraphs") == 0) {
                days = "1";
                ChartCreatorDygraphs chartCreator;
                chart = chartCreator.get_chart_days(days);
            } else if (chart_type.compare("google_array") == 0) {
                days = "1";
                ChartCreatorGoogleArray chartCreator;
                chart = chartCreator.get_chart_days(days);
            }
        }

        mg_printf(conn, chart.c_str());
    } else {
        const struct mg_request_info *req_info = mg_get_request_info(conn);
        std::string uri = std::string(req_info->local_uri);
        std::string html = ReadHtml::readHtml("html/auth/pleaselogin.html");
        std::string s = boost::str(boost::format(html) % uri);
        mg_printf(conn, s.c_str());
    }
    return true;
}
