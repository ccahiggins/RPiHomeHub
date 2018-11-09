#include "JsonBoilerStatusHandler.hpp"

JsonBoilerStatusHandler::JsonBoilerStatusHandler(Boiler* boiler_) {
	boiler = boiler_;
}

bool JsonBoilerStatusHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	using namespace std;
	cout << "1|" << flush;
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n");
	cout << "2|" << flush;
	mg_printf(conn, "%s", getJson().c_str());
cout << "3|" << flush;
	return true;
}

std::string JsonBoilerStatusHandler::getJson()
{
	using namespace nlohmann;
	using namespace std;
	
	Boiler::boiler_status boilerStatus = boiler->getBoilerStatus();
	
	int waterStatus = boilerStatus.waterStatus;
	int heatingStatus = boilerStatus.heatingStatus;
		
	//Json strings
	const string JSON_BOILER = "boiler";
	const string JSON_STATUS = "status";
	const string JSON_WATER = "water";
	const string JSON_HEATING = "heating";
	const string JSON_DURATION = "duration";
	const string JSON_OFF = "off";
	const string JSON_ON = "on";
	const string JSON_ERROR = "error";
	
	json j;
	
	if (waterStatus == ERROR) {
		j[JSON_BOILER][JSON_WATER][JSON_STATUS] = JSON_ERROR;
	}
	else
	{
		if (waterStatus == OFF)
		{
			j[JSON_BOILER][JSON_WATER][JSON_STATUS] = JSON_OFF;
		}
		else if (waterStatus == ON)
		{
			j[JSON_BOILER][JSON_WATER][JSON_STATUS] = JSON_ON;
		}
		else if (waterStatus > 0)
		{
			j[JSON_BOILER][JSON_WATER][JSON_STATUS] = JSON_ON;
			j[JSON_BOILER][JSON_WATER][JSON_DURATION] = waterStatus;
		}
		
	}
	if (heatingStatus == ERROR) {
		j[JSON_BOILER][JSON_HEATING][JSON_STATUS] = JSON_ERROR;
	}
	else
	{
		if (heatingStatus == OFF)
		{
			j[JSON_BOILER][JSON_HEATING][JSON_STATUS] = JSON_OFF;
		}
		else if (heatingStatus == ON)
		{
			j[JSON_BOILER][JSON_HEATING][JSON_STATUS] = JSON_ON;
		}
		else if (heatingStatus > 0)
		{
			j[JSON_BOILER][JSON_HEATING][JSON_STATUS] = JSON_ON;
			j[JSON_BOILER][JSON_HEATING][JSON_DURATION] = heatingStatus;
		}
	}
	

	
/* 	if (water == -2) {
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
	} */
	
	return j.dump();
}
