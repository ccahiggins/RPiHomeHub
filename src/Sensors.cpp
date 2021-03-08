#include "Sensors.hpp"

std::string Sensors::getName(int sensor) {
	
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	
	std::string sensorName;

	rc = sqlite3_open("db/sensors.db", &db);
	if( rc ){
		fprintf(stderr, "C:ERRDB: %s\n", sqlite3_errmsg(db));
		return "";
	}
	
	std::string sqlStatement = getSqlStatementForSensorName(sensor);
	const char *sql = sqlStatement.c_str();
	
	rc = sqlite3_exec(db, sql, sensor_callback, &sensorName, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "C:SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	
	sqlite3_close(db);

	return sensorName;
}

std::string Sensors::getShortName(int sensor) {
	
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	
	std::string sensorName;

	rc = sqlite3_open("db/sensors.db", &db);
	if( rc ){
		fprintf(stderr, "C:ERRDB: %s\n", sqlite3_errmsg(db));
		return "";
	}
	
	std::string sqlStatement = getSqlStatementForSensorShortName(sensor);
	const char *sql = sqlStatement.c_str();
	
	rc = sqlite3_exec(db, sql, sensor_callback, &sensorName, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "C:SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	
	sqlite3_close(db);

	return sensorName;
}

std::string Sensors::getIdByName(std::string name) {
	
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	
	std::string sensorId;

	rc = sqlite3_open("db/sensors.db", &db);
	if( rc ){
		fprintf(stderr, "C:ERRDB: %s\n", sqlite3_errmsg(db));
		return "";
	}
	
	std::string sqlStatement = getSqlStatementForIdByName(name);
	const char *sql = sqlStatement.c_str();
	
	rc = sqlite3_exec(db, sql, sensor_callback, &sensorId, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "C:SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	
	sqlite3_close(db);

	return sensorId;
}

std::vector<std::vector<std::string>> Sensors::getSensors() {
	
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	
	std::vector<std::vector<std::string> > table;

	rc = sqlite3_open("db/sensors.db", &db);
	if( rc ){
		fprintf(stderr, "C:ERRDB: %s\n", sqlite3_errmsg(db));
	}
	
	std::string sqlStatement = getSqlStatementForSensors();
	const char *sql = sqlStatement.c_str();
	
	rc = sqlite3_exec(db, sql, sensors_callback, &table, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "C:SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	
	sqlite3_close(db);

	return table;
}

std::string Sensors::getSqlStatementForSensorName(int sensor) {
	
	std::ostringstream oss;
	oss << "select name from sensor where id = " << sensor;
	
	return oss.str();
}

std::string Sensors::getSqlStatementForSensorShortName(int sensor) {
	
	std::ostringstream oss;
	oss << "select short_name from sensor where id = " << sensor;
	
	return oss.str();
}

std::string Sensors::getSqlStatementForSensors() {
	
	std::string sql = "select * from sensor";
	
	return sql;
}

std::string Sensors::getSqlStatementForIdByName(std::string name) {
	
	std::ostringstream oss;
	oss << "select id from sensor where name = '" << name << "'";
	
	return oss.str();
}

/* std::vector<std::vector<std::string>> Sensors::getSensorNames() {
	
} */

int Sensors::sensor_callback(void *ptr, int argc, char* argv[], char* cols[]) {
	
	std::string *pstr = static_cast<std::string *>(ptr);
	*pstr = argv[0];
	
	return 0;
}

int Sensors::sensors_callback(void *ptr, int argc, char* argv[], char* cols[]) {
	
	typedef std::vector<std::vector<std::string> > table_type;
	table_type* table = static_cast<table_type*>(ptr);
	std::vector<std::string> row;
	for (int i = 0; i < argc; i++) {
		row.push_back(argv[i] ? argv[i] : "(NULL)");
	}
	table -> push_back(row);
	
	return 0;
}