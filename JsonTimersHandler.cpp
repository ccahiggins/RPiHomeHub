class JsonTimersHandler: public CivetHandler
{
public:
	bool handleGet(CivetServer *server, struct mg_connection *conn) {
		
		//cout << "JsonTimers" << endl;
		string jsonString = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n";
		jsonString.append("{\"Timers\":[\n");
		
		vector<Timer::timerEvent>& timers = timer.getTimers();
		if (timers.size()>0)
		{			
			for (unsigned x=0; x<timers.size(); x++)
			{
				std::ostringstream hour;
				std::ostringstream minute;
				std::ostringstream duration;
				std::ostringstream boileritem;
				std::ostringstream enabled;
				std::ostringstream onetime;
				std::ostringstream id;
				
				jsonString.append("{\"StartHour\":");
				hour << timers[x].startHour;
				jsonString.append(hour.str());
				jsonString.append(",");
				
				jsonString.append("\"StartMinute\":");
				minute << timers[x].startMinute;
				jsonString.append(minute.str());
				jsonString.append(",");
				
				jsonString.append("\"Duration\":");
				duration << timers[x].duration;
				jsonString.append(duration.str());
				jsonString.append(",");
				
				jsonString.append("\"BoilerItem\":");
				boileritem << timers[x].boilerItem;
				jsonString.append(boileritem.str());
				jsonString.append(",");
				
				jsonString.append("\"Enabled\":");
				enabled << timers[x].enabled;
				jsonString.append(enabled.str());
				jsonString.append(",");
				
				jsonString.append("\"OneTime\":");
				onetime << timers[x].oneTime;
				jsonString.append(onetime.str());
				jsonString.append(",");
				
				jsonString.append("\"Id\":");
				id << timers[x].id;
				jsonString.append(id.str());
				jsonString.append("}");
				
				if (x<timers.size()-1)
					jsonString.append(",");
			}
		}
		jsonString.append("]\n}\n");
		mg_printf(conn, "%s", jsonString.c_str());
		return true;
		}
};