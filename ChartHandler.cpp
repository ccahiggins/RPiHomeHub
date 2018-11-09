#include "ChartHandler.h"

int startTime=0;
int endTime=0;

bool ChartHandler::handleGet(CivetServer *server, struct mg_connection *conn) {
	using namespace std;
	startTime = clock();

	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	string days;
	string to;
	string from;
	string tempData;
	string sqlStatement;
	
	bool foundFrom = CivetServer::getParam(conn, "from", from);
	bool foundTo = CivetServer::getParam(conn, "to", to);
	bool foundDays = CivetServer::getParam(conn, "days", days);
	
	if (foundFrom && foundTo) {
		sqlStatement = getSqlStatement(from, to);
	}
	else if (foundFrom && foundDays) {
		sqlStatement = getSqlStatementFromDays(from, days);
	} else if (foundDays) {
		sqlStatement = getSqlStatement(days);
	}
	else {
		days="1";
		sqlStatement = getSqlStatement(days);
	}
	tempData=getTempGraph(sqlStatement);
	cout << "Fin:" << clock() - startTime << endl;
	mg_printf(conn, "%s", tempData.c_str());

	return true;
}

std::string ChartHandler::getSqlStatementFromDays(std::string from, std::string &days) {
	using namespace std;
	ostringstream ss;
	ss << "SELECT timestamp, temp, id ";
	ss << "FROM temps ";
	ss << "WHERE id IN (1,2,3,4) ";
	ss << "AND timestamp BETWEEN date('" << from << "') AND date('" << from << "','+" << days << " days');";
	
	return ss.str();
}

/* std::string ChartHandler::getSqlStatement(std::string &days)
{
	using namespace std;
	ostringstream ss;
	ss << "SELECT (STRFTIME('%Y', timestamp)) || ',' || (STRFTIME('%m', timestamp) - 1) || ',' || (STRFTIME('%d,%H,%M,%S', timestamp)), temp, id ";
	ss << "FROM temps ";
	ss << "WHERE id IN (1,2,3,4) ";
	ss << "AND timestamp > datetime('now', 'localtime', '-" << days << " days');";
	
	return ss.str();
} */

std::string ChartHandler::getSqlStatement(std::string &days)
{
	using namespace std;
	ostringstream ss;
	ss << "SELECT timestamp, temp, id ";
	ss << "FROM temps ";
	ss << "WHERE id IN (1,2,3,4) ";
	ss << "AND timestamp > datetime('now', 'localtime', '-" << days << " days');";
	
	return ss.str();
}

std::string ChartHandler::getSqlStatement(std::string &from, std::string &to)
{
	using namespace std;
	ostringstream ss;
	ss << "SELECT timestamp, temp, id ";
	ss << "FROM temps ";
	ss << "WHERE id IN (1,2,3,4) ";
	ss << "AND timestamp BETWEEN date('" << from << "') ";
	ss << "AND date('" << to << "');";
	
	return ss.str();
}

std::string ChartHandler::getTempGraph(std::string &sqlStatement)
{
	using namespace std;
	int numGraphs = 1;
	string somestuff = "";

	somestuff.append(ReadHtml::readHtml("html/ChartHandler/graph1.html"));

	string data1=getTempData(sqlStatement);
	cout << "GetData:" << clock() - startTime << endl;
	somestuff.append(data1);

	somestuff.append(ReadHtml::readHtml("html/ChartHandler/graph2.html"));


	//Do this to make an example graph
	//If there is no data returned form database
	if (numGraphs < 1) {
		string emptyGraph = "";
		emptyGraph.append(ReadHtml::readHtml("html/ChartHandler/emptyGraph.html"));
		return emptyGraph;
	}
	return somestuff;

}

std::string ChartHandler::getTempData(std::string &sqlStatement)
{
	using namespace std;

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	
	vector<vector<string> > table;

	cout << "C:OD=" << flush;
	rc = sqlite3_open("db/sqlTemplog.db", &db);
	//rc = sqlite3_open("/media/ramdisk/sqlTemplog.db", &db);
	if( rc ){
		fprintf(stderr, "C:ERRDB: %s\n", sqlite3_errmsg(db));
		return "";
	}
	
	const char *sql = sqlStatement.c_str();
	
	//int startT=clock();
	rc = sqlite3_exec(db, sql, callback, &table, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "C:SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	
	sqlite3_close(db);
	cout << "DBClose:" << clock() - startTime << endl;
	string tempData=formatGraphData(table);
	cout << "Graph:" << clock() - startTime << endl;
	cout << "C:CD/" << flush;

	
	return tempData;
}

/* std::string ChartHandler::formatGraphData(std::vector<std::vector<std::string> > &data)
{
	using namespace std;
	
	string sensor1Name = "Bed";
	string sensor2Name = "Living";
	string sensor3Name = "Kids";
	string sensor4Name = "Outside";
	
	vector<string> sensors;
	
	ostringstream tempData;
	if (data.size() < 1) {
		return tempData.str();
	}

	tempData << "var data";
	tempData << " = google.visualization.arrayToDataTable([\n";

	for (unsigned int i=0; i<data.size(); i++)
	{
		if (sensors.empty())
			sensors.push_back(data[i][2]);
		else if (!(find(sensors.begin(), sensors.end(), data[i][2]) != sensors.end()))
			sensors.push_back(data[i][2]);
	}

	sort(sensors.begin(), sensors.end());

	cout << "Sort:" << clock() - startTime << endl;
	//Make titles for graph data
	tempData << "['Date', ";
	if (find(sensors.begin(), sensors.end(), "1") != sensors.end()) {
		tempData << "'";
		tempData << sensor1Name;
		tempData << "', ";
	}
	if (find(sensors.begin(), sensors.end(), "2") != sensors.end()) {
		tempData << "'";
		tempData << sensor2Name;
		tempData << "', ";
	}
	if (find(sensors.begin(), sensors.end(), "3") != sensors.end()) {
		tempData << "'";
		tempData << sensor3Name;
		tempData << "', ";
	}
	if (find(sensors.begin(), sensors.end(), "4") != sensors.end()) {
		tempData << "'";
		tempData << sensor4Name;
		tempData << "', ";
	}
	tempData << "],\n";
	
	
	for (unsigned int i = 0; i < data.size(); i++) {
		//Add date
		tempData << "[new Date(";
		tempData << data[i][0]; 	  // year
		//tempData << ",";
		
		//string month = data[i][0].substr (5, 2);  // Month
		//int value = atoi(month.c_str()) - 1;	  // Get month and subtract 1 for graph (Google months are 0-11)
		
		//tempData << value; 						  // month - 1
		//tempData << ",";
		//tempData << data[i][0].substr (8, 2);	  // day
		//tempData << ",";
		//tempData << data[i][0].substr (11, 2);	  // hour
		//tempData << ",";
		//tempData << data[i][0].substr (14, 2);	  // minute
		tempData << "),";
		
		//Add voltage data and nulls depending on how many sensors there are
		for (unsigned int j = 0; j < sensors.size(); j++) {
			if (!sensors[j].compare(data[i][2])) {
				tempData << data[i][1];
				tempData << ",";
			}
			else {
				tempData << "null, ";
			}
		}
		tempData << "],\n";
	}
	tempData << "]);\n";
	
	return tempData.str();
}	 */

std::string ChartHandler::formatGraphData(std::vector<std::vector<std::string> > &data)
{
	using namespace std;
	
	string sensor1Name = "Bed";
	string sensor2Name = "Living";
	string sensor3Name = "Kids";
	string sensor4Name = "Outside";
	
	vector<string> sensors;
	
	ostringstream tempData;
	if (data.size() < 1) {
		return tempData.str();
	}

	tempData << "var data";
	tempData << " = google.visualization.arrayToDataTable([\n";

	cout << "Size:" << data.size() << endl;
	for (unsigned int i=0; i < data.size(); i++)
	{
		if (sensors.empty())
			sensors.push_back(data[i][2]);
		else if (!(find(sensors.begin(), sensors.end(), data[i][2]) != sensors.end()))
			sensors.push_back(data[i][2]);
	}

	sort(sensors.begin(), sensors.end());

	cout << "Sort:" << clock() - startTime  << endl;
	//Make titles for graph data
	tempData << "['Date', ";
	if (find(sensors.begin(), sensors.end(), "1") != sensors.end()) {
		tempData << "'";
		tempData << sensor1Name;
		tempData << "', ";
	}
	if (find(sensors.begin(), sensors.end(), "2") != sensors.end()) {
		tempData << "'";
		tempData << sensor2Name;
		tempData << "', ";
	}
	if (find(sensors.begin(), sensors.end(), "3") != sensors.end()) {
		tempData << "'";
		tempData << sensor3Name;
		tempData << "', ";
	}
	if (find(sensors.begin(), sensors.end(), "4") != sensors.end()) {
		tempData << "'";
		tempData << sensor4Name;
		tempData << "', ";
	}
	tempData << "],\n";
	cout << "FindS:" << clock() - startTime << endl;
	
	
	for (unsigned int i = 0; i < data.size(); i++) {
		//Add date
		tempData << "[new Date(";
		tempData << data[i][0].substr (0, 4); 	  // year
		tempData << ",";
		
		string month = data[i][0].substr (5, 2);  // Month
		int value = atoi(month.c_str()) - 1;	  // Get month and subtract 1 for graph (Google months are 0-11)
		
		tempData << value; 						  // month - 1
		tempData << ",";
		tempData << data[i][0].substr (8, 2);	  // day
		tempData << ",";
		tempData << data[i][0].substr (11, 2);	  // hour
		tempData << ",";
		tempData << data[i][0].substr (14, 2);	  // minute
		tempData << "),";
		
		//Add voltage data and nulls depending on how many sensors there are
		for (unsigned int j = 0; j < sensors.size(); j++) {
			if (!sensors[j].compare(data[i][2])) {
				tempData << data[i][1];
				tempData << ",";
			}
			else {
				tempData << "null, ";
			}
		}
		tempData << "],\n";
	}
	tempData << "]);\n";
	
	return tempData.str();
}	

int ChartHandler::callback(void *ptr, int argc, char* argv[], char* cols[])
{
	using namespace std;
	typedef vector<vector<string> > table_type;
	ChartHandler::TempData td;
	table_type* table = static_cast<table_type*>(ptr);
	vector<string> row;
	for (int i = 0; i < argc; i++)
	{
		row.push_back(argv[i] ? argv[i] : "(NULL)");
	}
	table -> push_back(row);
	return 0;
}
