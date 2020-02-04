#include "ChartCreator.hpp"



std::string ChartCreator::getChartDays(std::string &days) {
	
	std::string sqlStatement = getSqlStatementDays(days);
	std::string tempData = getTempGraph(sqlStatement);
	
	return tempData;
}

std::string ChartCreator::getChartFromDays(std::string &from, std::string &days) {

	std::string sqlStatement = getSqlStatementFromDays(from, days);
	std::string tempData = getTempGraph(sqlStatement);
	
	return tempData;
}

std::string ChartCreator::getChartFromTo(std::string &from, std::string &to) {

	std::string sqlStatement = getSqlStatementFromTo(from, to);
	std::string tempData = getTempGraph(sqlStatement);
	
	return tempData;
}


void ChartCreator::writeChartToFile() {
	
	std::string days = "1";
	std::string sqlStatement = getSqlStatementDays(days);
	std::string tempData = getTempGraph(sqlStatement);
	
	std::ofstream myfile;
	myfile.open ("chartdata");
	myfile << tempData;
	myfile.close();
}



std::string ChartCreator::getSqlStatementFromDays(std::string &from, std::string &days) {

	std::ostringstream ss;
	ss << "SELECT timestamp, temp, id ";
	ss << "FROM temps ";
	ss << "WHERE id IN (1,2,3,4,5) ";
	ss << "AND timestamp BETWEEN date('" << from << "') AND date('" << from << "','+" << days << " days');";
	
	return ss.str();
}

/* std::string ChartCreator::getSqlStatement(std::string &days)
{
	//using namespace std;
	ostringstream ss;
	ss << "SELECT (STRFTIME('%Y', timestamp)) || ',' || (STRFTIME('%m', timestamp) - 1) || ',' || (STRFTIME('%d,%H,%M,%S', timestamp)), temp, id ";
	ss << "FROM temps ";
	ss << "WHERE id IN (1,2,3,4,5) ";
	ss << "AND timestamp > datetime('now', 'localtime', '-" << days << " days');";
	
	return ss.str();
} */

std::string ChartCreator::getSqlStatementDays(std::string &days) {

	std::ostringstream ss;
	ss << "SELECT timestamp, temp, id ";
	ss << "FROM temps ";
	ss << "WHERE id IN (1,2,3,4,5) ";
	ss << "AND timestamp > datetime('now', 'localtime', '-" << days << " days');";
	
	return ss.str();
}

std::string ChartCreator::getSqlStatementFromTo(std::string &from, std::string &to) {

	std::ostringstream ss;
	ss << "SELECT timestamp, temp, id ";
	ss << "FROM temps ";
	ss << "WHERE id IN (1,2,3,4,5) ";
	ss << "AND timestamp BETWEEN date('" << from << "') ";
	ss << "AND date('" << to << "');";
	
	return ss.str();
}

std::string ChartCreator::getTempGraph(std::string &sqlStatement) {

	int numGraphs = 1;
	std::string somestuff = "";

	somestuff.append(ReadHtml::readHtml("html/ChartCreator/graph1.html"));
	//somestuff.append("=========");

	std::string data1=getTempData(sqlStatement);
	somestuff.append(data1);

	somestuff.append(ReadHtml::readHtml("html/ChartCreator/graph2.html"));


	//Do this to make an example graph
	//If there is no data returned form database
	if (numGraphs < 1) {
		std::string emptyGraph = "";
		emptyGraph.append(ReadHtml::readHtml("html/ChartCreator/emptyGraph.html"));
		return emptyGraph;
	}
	return somestuff;

}

std::string ChartCreator::getTempData(std::string &sqlStatement) {

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	
	std::vector<std::vector<std::string> > table;

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
	std::string tempData=formatGraphData(table);

	return tempData;
}

std::string ChartCreator::formatGraphData(std::vector<std::vector<std::string> > &data) {
	
	std::string sensor1Name = "Bed";
	std::string sensor2Name = "Living";
	std::string sensor3Name = "Izzy";
	std::string sensor4Name = "Outside";
	std::string sensor5Name = "Bob";
	
	std::vector<std::string> sensors;
	
	std::ostringstream tempData;
	if (data.size() < 1) {
		return tempData.str();
	}

	tempData << "var data";
	tempData << " = google.visualization.arrayToDataTable([\n";

	for (unsigned int i=0; i < data.size(); i++)
	{
		if (sensors.empty())
			sensors.push_back(data[i][2]);
		else if (!(find(sensors.begin(), sensors.end(), data[i][2]) != sensors.end()))
			sensors.push_back(data[i][2]);
	}

	std::sort(sensors.begin(), sensors.end());

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
	if (find(sensors.begin(), sensors.end(), "5") != sensors.end()) {
		tempData << "'";
		tempData << sensor5Name;
		tempData << "', ";
	}
	tempData << "],\n";
	
	
	for (unsigned int i = 0; i < data.size(); i++) {
		//Add date
		tempData << "[new Date(";
		tempData << data[i][0].substr (0, 4); 	  // year
		tempData << ",";
		
		std::string month = data[i][0].substr (5, 2);  // Month
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

int ChartCreator::callback(void *ptr, int argc, char* argv[], char* cols[]) {

	typedef std::vector<std::vector<std::string> > table_type;
	ChartCreator::TempData td;
	table_type* table = static_cast<table_type*>(ptr);
	std::vector<std::string> row;
	for (int i = 0; i < argc; i++) {
		row.push_back(argv[i] ? argv[i] : "(NULL)");
	}
	table -> push_back(row);
	
	return 0;
}