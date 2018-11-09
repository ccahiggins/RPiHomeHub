#include "JsonHubHandler.hpp"

JsonHubHandler::JsonHubHandler(Boiler* boiler_) {
	boiler = boiler_;
}

bool JsonHubHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n");
	
	int waterStatus = boiler->WaterStatus();
	int heatingStatus = boiler->HeatingStatus();
	
	mg_printf(conn, "%s", getJson(waterStatus, heatingStatus).c_str());

	return true;
}

std::string JsonHubHandler::getJson(int water, int heating)
{
	using namespace nlohmann;
	using namespace std;
	
	const string BOILER = "boiler";
	const string STATUS = "status";
	const string WATER = "water";
	const string HEATING = "heating";
	const string DURATION = "duration";
	const string OFF = "off";
	const string ON = "on";
	const string ERROR = "error";
	
	json j;
	
	if (water == -2) {
		j[BOILER][WATER][STATUS] = ERROR;
	} else if (water == -1) {
		j[BOILER][WATER][STATUS] = OFF;
	} else if (water >=0 ){
		j[BOILER][WATER][STATUS] = ON;
		j[BOILER][WATER][DURATION] = water;
	}
	
	if (heating == -2) {
		j[BOILER][HEATING][STATUS] = ERROR;
	} else if (heating == -1) {
		j[BOILER][HEATING][STATUS] = OFF;
	} else if (heating >= 0) {
		j[BOILER][HEATING][STATUS] = ON;
		j[BOILER][HEATING][DURATION] = heating;
	}
	
	return j.dump();
}
