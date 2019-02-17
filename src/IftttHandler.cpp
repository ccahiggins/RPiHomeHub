#include "IftttHandler.hpp"

IftttHandler::IftttHandler(Boiler* boiler_, Timer* timer_, TempSensorController* tempSens_){
	boiler = boiler_;
	timer = timer_;
	tempSens = tempSens_;
}

/**#define ERROR -2
#define OFF 0
#define ON -1*/

int IftttHandler::callback(void *ptr, int argc, char* argv[], char* cols[])
{
	using namespace std;
	typedef vector<vector<string> > table_type;
	table_type* table = static_cast<table_type*>(ptr);
	vector<string> row;
	for (int i = 0; i < argc; i++)
	{
		row.push_back(argv[i] ? argv[i] : "(NULL)");
	}
	table -> push_back(row);
	return 0;
}

std::string IftttHandler::getTemp(std::string id) {
	using namespace std;
	
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	
	vector<vector<string> > table;

	rc = sqlite3_open("/home/pi/h/db/sqlTemplog.db", &db);
	//rc = sqlite3_open("/media/ramdisk/sqlTemplog.db", &db);
	if( rc ){
		fprintf(stderr, "C:ERRDB: %s\n", sqlite3_errmsg(db));
		return 0;
	}
	
	string sqlbuilder = "select printf(\"%.1f\", temp) as temp from temps where id = " + id + " order by timestamp desc limit 1";
	
	const char *sql = sqlbuilder.c_str();
	
	//int startT=clock();
	rc = sqlite3_exec(db, sql, callback, &table, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "C:SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	
	sqlite3_close(db);
	

	for ( auto &i : table ) {
		for ( auto &j : i ) {
			//cout << "got db:" << j << endl;
			return j;
		}
	}
	
	return "";
}

std::string IftttHandler::getTimers() {
	using namespace std;
	
	string timerjson = "{\"timers\":[";
	vector<Timer::TimerEvent>& timers = timer->getTimers();
	for (unsigned x=0; x < timers.size(); x++)
	{
		string whichone;
		string endis;
		string onetime;
		if (timers[x].boilerItem == 0) {
			whichone="water";
		} else {
			whichone="heating";
		}
		if (timers[x].enabled) {
			endis = "true";
		} else {
			endis = "false";
		}
		
		if (timers[x].oneTime) {
			onetime = "true";
		} else {
			onetime = "false";
		}
		timerjson.append("{\"id\":\"" + to_string(timers[x].id) + "\", \"boiler_item\":\"" + whichone + "\", \"hours\":\"" + to_string(timers[x].startHour) + "\", \"minutes\":\"" +  to_string(timers[x].startMinute) + "\",\"duration\":\"" + to_string(timers[x].duration) + "\", \"onetime\":\"" + onetime + "\", \"enabled\":\"" + endis + "\"");
		
		//string s = str( format(line) % whichone % timers[x].startHour % timers[x].startMinute% timers[x].duration % enDis % timers[x].id % endis % timers[x].id );
		//content.append(s);
		timerjson.append("}");
		if (!(x == timers.size() - 1)) {
			timerjson.append(",");
		}
	}
	timerjson.append("]}");
	//cout << timerjson << endl;
	
	return timerjson;
}
/*
bool IftttHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	using namespace std;
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n");
	
	vector<int> lowBatts = tempSens -> lowBattery();
	
	const struct mg_request_info *req_info = mg_get_request_info(conn);
	RequestValidator rv(req_info->request_uri);
	
	if (rv.getType() == RequestType::WaterOn) {
		if (boiler->TurnWaterOn(stoi(rv.getDuration()))) {
			//cout << "google turned the water on" << endl;
			mg_printf(conn, "{\"status\":\"done\"}");
		} else {
			mg_printf(conn, "{\"error\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::WaterOff) {
		if (boiler->TurnWaterOff()) {
			//cout << "google turned the water off" << endl;
			mg_printf(conn, "{\"status\":\"done\"}");
		} else {
			mg_printf(conn, "{\"error\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::HeatingOn) {
		if (boiler->TurnHeatingOn(stoi(rv.getDuration()))) {
			//cout << "google turned the heating on" << endl;
			mg_printf(conn, "{\"status\":\"done\"}");
		} else {
			mg_printf(conn, "{\"error\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::HeatingOff) {
		if (boiler->TurnHeatingOff()) {
			//cout << "google turned the heating off" << endl;
			mg_printf(conn, "{\"status\":\"done\"}");
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::Status) {
			Boiler::boiler_status boilerStatus = boiler->getBoilerStatus();
	
			int waterStatus = boilerStatus.waterStatus;
			int heatingStatus = boilerStatus.heatingStatus;
			
			if (waterStatus == -2 || heatingStatus == -2) {
				mg_printf(conn, "{\"status\":\"error\"}");
			} else {
				string waterJson;
				string heatingJson;
				string batteryJson;
				
				if (waterStatus == -1) {
					waterJson = "\"water\":{\"status\":\"off\"}";
				} else {
					waterJson = "\"water\":{\"status\":\"on\",\"duration\":\"" + to_string(waterStatus) + "\"}";
				}
				
				if (heatingStatus == -1) {
					heatingJson = "\"heating\":{\"status\":\"off\"}";
				} else {
					heatingJson = "\"heating\":{\"status\":\"on\",\"duration\":\"" + to_string(heatingStatus) + "\"}";
				}
				
				if (lowBatts.size() > 0) {
					batteryJson = "\"lowbattery\":[";
					for(std::vector<int>::iterator it = lowBatts.begin(); it != lowBatts.end(); ++it) {
						batteryJson.append(to_string(*it) + ",");
						//content.append(to_string(*it));
					}
					batteryJson.append("],");
				} else {
					batteryJson = "";
				}
				
				string result = "{\"status\":\"done\"," + waterJson + "," + heatingJson + heatingJson + "}";
				
				mg_printf(conn, result.c_str());
			}
			
	} else if (rv.getType() == RequestType::TemperatureAll) {
		cout << "TempAll" << endl;
		mg_printf(conn, "{\"error\":\"notyetimplemented\"}");
	} else if (rv.getType() == RequestType::TemperatureOne) {
		cout << "TempOne" << endl;
		string id;
		cout << rv.getRoom() << endl;
		if (rv.getRoom().compare("livingroom") == 0) {
			id = "1";
		} else if (rv.getRoom().compare("bedroom") == 0) {
			id = "2";
		} else if (rv.getRoom().compare("outside") == 0) {
			id = "4";
		}
		string temp = getTemp(id);
		string result = "{\"status\":\"done\",\"temperature\":\"" + temp + "\"}";
		mg_printf(conn, result.c_str());
	} else if (rv.getType() == RequestType::Timers) {
		string result = getTimers();
		mg_printf(conn, result.c_str());
	} else if (rv.getType() == RequestType::TimersAddWater) {		
		bool onetime = true;
		if (rv.getOneTime().compare("f") == 0) {
			onetime = false;
		} else if (rv.getOneTime().compare("f") == 0) {
			onetime = true;
		}
		
		int hour = stoi(rv.getHour());
		int minute = stoi(rv.getMinute());
		int duration = stoi(rv.getDuration());
		
		if (timer->addTimerEvent(hour, minute, duration, 0, true, onetime)) {
			mg_printf(conn, "{\"status\":\"done\"}");
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::TimersAddHeating) {
		bool onetime = true;
		if (rv.getOneTime().compare("f") == 0) {
			onetime = false;
		} else if (rv.getOneTime().compare("f") == 0) {
			onetime = true;
		}
		
		int hour = stoi(rv.getHour());
		int minute = stoi(rv.getMinute());
		int duration = stoi(rv.getDuration());
		
		if (timer->addTimerEvent(hour, minute, duration, 1, true, onetime)) {
			mg_printf(conn, "{\"status\":\"done\"}");
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::TimersDelete) {
		if (timer->removeTimerEvent(stoi(rv.getTimerId()))) {
			mg_printf(conn, "{\"status\":\"done\"}");
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::TimersEnable) {
		if (timer->enableTimerEvent(stoi(rv.getTimerId()))) {
			mg_printf(conn, "{\"status\":\"done\"}");
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::TimersDisable) {
		if (timer->disableTimerEvent(stoi(rv.getTimerId()))) {
			mg_printf(conn, "{\"status\":\"done\"}");
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
		}
	} else {
		mg_printf(conn, "{\"error\":\"invalidrequest\"}");
	}

	return true;
}
*/
bool IftttHandler::apiKeyMatch(std::string requestKey) {
	using namespace std;
	string key;
	ifstream file;
	file.open("apikey");
	getline(file, key);
	
	if (key.compare(requestKey) == 0) {
		return true; 
	} else {
		return false;
	}
}

bool IftttHandler::handlePost(CivetServer *server, struct mg_connection *conn) {
	using namespace std;
	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n");
	
	vector<int> lowBatts = tempSens -> lowBattery();
	
	string requestKey="";
	CivetServer::getParam(conn, "apikey", requestKey);
	if (!apiKeyMatch(requestKey)) {
		mg_printf(conn, "{\"status\":\"invalidkey\"}");
	}
	
	bool change = false;
	
	const struct mg_request_info *req_info = mg_get_request_info(conn);
	RequestValidator rv(req_info->request_uri);
	
	if (rv.getType() == RequestType::WaterOn) {
		if (boiler->TurnWaterOn(stoi(rv.getDuration()))) {
			//cout << "google turned the water on" << endl;
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"error\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::WaterOff) {
		if (boiler->TurnWaterOff()) {
			//cout << "google turned the water off" << endl;
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"error\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::HeatingOn) {
		if (boiler->TurnHeatingOn(stoi(rv.getDuration()))) {
			//cout << "google turned the heating on" << endl;
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"error\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::HeatingOff) {
		if (boiler->TurnHeatingOff()) {
			//cout << "google turned the heating off" << endl;
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::Status) {
			Boiler::boiler_status boilerStatus = boiler->getBoilerStatus();
	
			int waterStatus = boilerStatus.waterStatus;
			int heatingStatus = boilerStatus.heatingStatus;
			
			if (waterStatus == -2 || heatingStatus == -2) {
				mg_printf(conn, "{\"status\":\"error\"}");
			} else {
				string waterJson;
				string heatingJson;
				string batteryJson;
				
				if (waterStatus == -1) {
					waterJson = "\"water\":{\"status\":\"off\"}";
				} else {
					waterJson = "\"water\":{\"status\":\"on\",\"duration\":\"" + to_string(waterStatus) + "\"}";
				}
				
				if (heatingStatus == -1) {
					heatingJson = "\"heating\":{\"status\":\"off\"}";
				} else {
					heatingJson = "\"heating\":{\"status\":\"on\",\"duration\":\"" + to_string(heatingStatus) + "\"}";
				}
				
				if (lowBatts.size() > 0) {
					batteryJson = ",\"lowbattery\":[";
					bool first = true;
					for(std::vector<int>::iterator it = lowBatts.begin(); it != lowBatts.end(); ++it) {
	if (first) {
		first = false;
	} else {
		batteryJson.append(",");
	}
						batteryJson.append(to_string(*it));
						//content.append(to_string(*it));
					}
					batteryJson.append("]");
				} else {
					batteryJson = "";
				}
				
				string result = "{\"status\":\"done\"," + waterJson + "," + heatingJson + batteryJson + "}";
				
				mg_printf(conn, result.c_str());
			}
			
	} else if (rv.getType() == RequestType::TemperatureAll) {
		//cout << "TempAll" << endl;
		mg_printf(conn, "{\"error\":\"notyetimplemented\"}");
	} else if (rv.getType() == RequestType::TemperatureOne) {
		//cout << "TempOne" << endl;
		string id;
		//cout << rv.getRoom() << endl;
		if (rv.getRoom().compare("livingroom") == 0) {
			id = "1";
		} else if (rv.getRoom().compare("bedroom") == 0) {
			id = "2";
		} else if (rv.getRoom().compare("outside") == 0) {
			id = "4";
		}
		string temp = getTemp(id);
		string result = "{\"status\":\"done\",\"temperature\":\"" + temp + "\"}";
		mg_printf(conn, result.c_str());
	} else if (rv.getType() == RequestType::Timers) {
		string result = getTimers();
		mg_printf(conn, result.c_str());
	} else if (rv.getType() == RequestType::TimersAddWater) {		
		bool onetime = true;
		if (rv.getOneTime().compare("f") == 0) {
			onetime = false;
		} else if (rv.getOneTime().compare("f") == 0) {
			onetime = true;
		}
		
		int hour = stoi(rv.getHour());
		int minute = stoi(rv.getMinute());
		int duration = stoi(rv.getDuration());
		
		if (timer->addTimerEvent(hour, minute, duration, 0, true, onetime)) {
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::TimersAddHeating) {
		bool onetime = true;
		if (rv.getOneTime().compare("f") == 0) {
			onetime = false;
		} else if (rv.getOneTime().compare("f") == 0) {
			onetime = true;
		}
		
		int hour = stoi(rv.getHour());
		int minute = stoi(rv.getMinute());
		int duration = stoi(rv.getDuration());
		
		if (timer->addTimerEvent(hour, minute, duration, 1, true, onetime)) {
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::TimersDelete) {
		if (timer->removeTimerEvent(stoi(rv.getTimerId()))) {
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::TimersEnable) {
		if (timer->enableTimerEvent(stoi(rv.getTimerId()))) {
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
		}
	} else if (rv.getType() == RequestType::TimersDisable) {
		if (timer->disableTimerEvent(stoi(rv.getTimerId()))) {
			mg_printf(conn, "{\"status\":\"done\"}");
			change = true;
		} else {
			mg_printf(conn, "{\"status\":\"error\"}");
		}
	} else {
		mg_printf(conn, "{\"error\":\"invalidrequest\"}");
	}

	if (change) {
		system("./push");
	}
	
	return true;
}
