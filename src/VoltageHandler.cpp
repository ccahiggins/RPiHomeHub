#include "VoltageHandler.hpp"


bool VoltageHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	using namespace std;
	using boost::format;
	using boost::io::group;

	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	string param;
	string voltageData="";
	if (CivetServer::getParam(conn, "days", param))
	{
		int days = atoi(param.c_str()); 
		voltageData=printVolts(days);
	}
	else
	{
		voltageData = printVolts();

	}
	
	//printHtml("html/VoltageHandler/html.html", voltageData.c_str(), conn);
	string html = str( format(ReadHtml::readHtml("html/VoltageHandler/html.html")) % voltageData);
	mg_printf(conn, html.c_str());

	return true;
}

std::string VoltageHandler::formatVolts(std::vector<std::vector<std::string> > &data){
	using namespace std;
	//String for the name the title for each sensor
	string sensor1Name = "Bed";
	string sensor2Name = "Living";
	string sensor3Name = "Kids";
	string sensor4Name = "Outside";
	

	string voltsData="";		// String which will contain temp data for graph
	if (data.size() < 1) 		// Return empty string if no values in vector: data
		return voltsData;
	
   vector<string> sensors;		// Vector of strings for each ID that exists in vector: data 
   
   for (unsigned int i=0; i<data.size(); i++)	// Get each unique ID from vector: data
	{
		if (sensors.empty())
			sensors.push_back(data[i][2]);
		else if (!(find(sensors.begin(), sensors.end(), data[i][2]) != sensors.end()))
			sensors.push_back(data[i][2]);
	}
   
   sort(sensors.begin(), sensors.end());
   
   
	//voltsData.append("var data");
	//voltsData.append("   = google.visualization.arrayToDataTable([\n");

	
	
	//Make titles for graph data
	voltsData.append("['Date', ");
	if (find(sensors.begin(), sensors.end(), "1") != sensors.end())			//Add title if data for sensor exists in Vector: data
	{
		voltsData.append("'");
		voltsData.append(sensor1Name);
		voltsData.append("', ");
	}
	if (find(sensors.begin(), sensors.end(), "2") != sensors.end())			//Add title if data for sensor exists in Vector: data
	{
		voltsData.append("'");
		voltsData.append(sensor2Name);
		voltsData.append("', ");
	}
	if (find(sensors.begin(), sensors.end(), "3") != sensors.end())			//Add title if data for sensor exists in Vector: data
	{
		voltsData.append("'");
		voltsData.append(sensor3Name);
		voltsData.append("', ");
	}
	if (find(sensors.begin(), sensors.end(), "4") != sensors.end())			//Add title if data for sensor exists in Vector: data
	{
		voltsData.append("'");
		voltsData.append(sensor4Name);
		voltsData.append("', ");
	}
	voltsData.append("],\n");

	
	//Record last voltage to skip if voltage the same as last
	string lastV1 = "";
	string lastV2 = "";
	string lastV3 = "";
	string lastV4 = "";
	
	for (unsigned int i=0; i<data.size(); i++)
	{
		bool skipData=false;					//Skip if voltage unchanged bool
		string idString = data[i][2];			//Get ID string from Vector: data
		
		if (!idString.compare("1"))
		{
			if (!lastV1.compare(data[i][1]))	//Compare last voltage to current
			{
			 skipData=true;
			}
			else
			{
				lastV1=data[i][1];
			}
		}
		else if (!idString.compare("2"))
		{

			if (!lastV2.compare(data[i][1]))	//Compare last voltage to current
			{
				skipData=true;
			}
			else
			{
				lastV2=data[i][1];
			}
		}
		else if (!idString.compare("3"))
		{
			if (!lastV3.compare(data[i][1]))	//Compare last voltage to current
			{
				skipData=true;
			}
			else
			{
				lastV3=data[i][1];
			}
		}
		else if (!idString.compare("4"))
		{
			if (!lastV4.compare(data[i][1]))	//Compare last voltage to current
			{
				skipData=true;
			}
			else
			{
				lastV4=data[i][1];
			}
		}
		
		if (!skipData)							//Skip if voltage unchanged otherwise add data
		{
			//Add date
			voltsData.append("[new Date(");
			voltsData.append(data[i][0].substr (0, 4)); 	// year
			voltsData.append(",");
			
			string month=data[i][0].substr (5, 2);			// Month
			int value = atoi(month.c_str()) - 1;    		// Get month and subtract 1 for graph (Google months are 0-11)
			
			string newMonth;          						// string which will contain the changed month
			ostringstream convert;   						// stream used for the conversion
		
			convert << value;      							// insert the textual representation of 'month-1' in the characters in the stream

			newMonth = convert.str(); 
				
			voltsData.append(newMonth); 					// month - 1
			voltsData.append(",");
			voltsData.append(data[i][0].substr (8, 2));		// day
			voltsData.append(",");
			voltsData.append(data[i][0].substr (11, 2));	// hour
			voltsData.append(",");
			voltsData.append(data[i][0].substr (14, 2));	// minute
			voltsData.append("),");
			
			//Add voltage data and nulls depending on how many sensors there are
			for (unsigned int j=0; j<sensors.size(); j++)
			{
				if (!sensors[j].compare(data[i][2]))
				{
					voltsData.append(data[i][1]);
					voltsData.append(",");
				}
				else 
					voltsData.append("null, ");
			}
			voltsData.append("],\n");
		}
	}
	
	return voltsData;
}

std::string VoltageHandler::printVolts(int days)
{
	using namespace std;
	//cout << "Num days for Voltage graph: " << days << endl;
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	const char *sql;
	vector<vector<string> > table;
	
	cout << "V:OD=" << endl;
	rc = sqlite3_open("db/sqlTemplog.db", &db);
	if( rc ){
		fprintf(stderr, "V:ERRDB: %s\n", sqlite3_errmsg(db));
		//exit(0);
		return "";
	}else{
		//fprintf(stderr, "Opened database successfully\n");
	}

	//string sqlText = "SELECT timestamp, temp FROM temps WHERE id=";
	//sqlText.append(Result2);
	//sqlText.append(" AND timestamp>datetime('now', 'localtime', '-168 hours')");
	//sqlText.append(" AND timestamp>datetime('now', 'localtime', '-24 hours')");
	
	stringstream ss;

	ss << "SELECT timestamp, voltage, id ";
	ss << "FROM temps ";
	ss << "WHERE id in (1,2,3,4) ";
	ss << "AND timestamp>datetime('now', 'localtime', '-";
	ss << days;
	ss << " days') AND voltage IS NOT NULL;";

	auto x = ss.str();
	sql = x.c_str();

  
   rc = sqlite3_exec(db, sql, callback, &table, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "V: SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      //fprintf(stdout, "Operation done successfully\n");
   }
   cout << "V:CD/" << endl;
   sqlite3_close(db);
  
   string voltsData=formatVolts(table);

   
	return voltsData;
}

std::string VoltageHandler::printVolts()
{
	using namespace std;
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	const char *sql;
	vector<vector<string> > table;
	
	cout << "V:OD=" << endl;
	rc = sqlite3_open("db/sqlTemplog.db", &db);
	if( rc ){
		fprintf(stderr, "V:ERRDB: %s\n", sqlite3_errmsg(db));
		//exit(0);
		return "";
	}else{
	//fprintf(stderr, "Opened database successfully\n");
	}

	//string sqlText = "SELECT timestamp, temp FROM temps WHERE id=";
	//sqlText.append(Result2);
	//sqlText.append(" AND timestamp>datetime('now', 'localtime', '-168 hours')");
	//sqlText.append(" AND timestamp>datetime('now', 'localtime', '-24 hours')");
	
	//string sqlText = "SELECT timestamp, voltage, id FROM temps WHERE timestamp>datetime('now', 'localtime', '-24 hours')";
	stringstream ss;
	ss << "SELECT timestamp, voltage, id ";
	ss << "FROM temps ";
	ss << "WHERE voltage IS NOT NULL ";
	ss << "AND id IN (1,2,3,4);";
	
	auto x = ss.str();
	sql = x.c_str();

  
   rc = sqlite3_exec(db, sql, callback, &table, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      //fprintf(stdout, "Operation done successfully\n");
   }
   cout << "V:CD/" << endl;
   sqlite3_close(db);
  
   string voltsData=formatVolts(table);

   
	return voltsData;
}

int VoltageHandler::callback(void *ptr, int argc, char* argv[], char* cols[])
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
