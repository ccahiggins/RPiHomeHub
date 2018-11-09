#include "JsonBoilerHandler.hpp"

JsonBoilerHandler::JsonBoilerHandler(Boiler* boiler_) {
	boiler = boiler_;
}

bool JsonBoilerHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	using namespace std;
	string param = "";

	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n");
	
	if (CivetServer::getParam(conn, "json", param))
	{
		mg_printf(conn, getJson(param).c_str());
	} else {
		mg_printf(conn, "{\error\":\"true\"}");
	}
	return true;
}
	
std::string JsonBoilerHandler::getJson(std::string js)
{
	/* using namespace std;
	using namespace nlohmann;
	
	const string STATUS = "status";
	const string BOILER = "boiler";
	const string OPTION = "option";
	const string HEATING = "heating";
	const string WATER = "water";
	const string DURATION = "duration";
	const string ON = "on";
	const string OFF = "off";

	string str;
	cout << "1" << endl;
	try {
		cout << "2" << endl;
		json j = json::parse(js);
		string status = j.at(BOILER).at(STATUS);
		string option = j.at(BOILER).at(OPTION);
		int duration = j.at(BOILER).at(DURATION);
		
		bool ok = false;
		
		if (option.compare(WATER) == 0) {
			if (status.compare(ON) == 0) {
				if (duration == 0) {
					ok = boiler->TurnWaterOn();
				} else {
					ok = boiler->TurnWaterOn(duration);
				}
			} else if (status.compare(OFF) == 0) {
				ok = boiler->TurnWaterOff();
			}
		} else if (option.compare(HEATING) == 0) {
			if (status.compare(ON) == 0) {
				if (duration == 0) {
					ok = boiler->TurnHeatingOn();
				} else {
					ok = boiler->TurnHeatingOn(duration);
				}
			} else if (status.compare(OFF) == 0) {
				ok = boiler->TurnHeatingOff();
			}
		}	
	} catch (std::out_of_range e) {
		cout << "Error " << e.what() << endl;
	} catch (std::invalid_argument e) {
		cout << "Error " << e.what() << endl;
	} catch (std::domain_error e) {
		cout << "Error " << e.what() << endl;
	} */
	
	return "{\"status\":\"done\"}";
}
