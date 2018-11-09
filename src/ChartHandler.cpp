#include "ChartHandler.hpp"

int startTime=0;
int endTime=0;

ChartCreator chartCreator;

bool ChartHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	using namespace std;
	startTime = clock();

	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	string days;
	string to;
	string from;
	string chart;
	
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
		cout << "File" << flush;
		chart = ReadHtml::readHtml("chartdata");
		} else {
			chart = chartCreator.getChartDays(days);
		}
	}
	else {
		cout << "File" << flush;
		chart = ReadHtml::readHtml("chartdata");
		//days="1";
		//chart = chartCreator.getChartDays(days);
	}
	
	mg_printf(conn, "%s", chart.c_str());
	endTime = clock();
	cout << "T:" << endTime - startTime << flush;
	return true;
}



