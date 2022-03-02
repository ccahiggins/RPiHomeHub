#include "DatabaseController.hpp"

std::string DatabaseController::temps_database = "";
std::string DatabaseController::auth_database = "";
std::string DatabaseController::sensors_database = "";

static volatile bool locked = false;

void DatabaseController::set_temps_database(std::string database) {
	temps_database = database;
}

void DatabaseController::set_auth_database(std::string database) {
	auth_database = database;
}

void DatabaseController::set_sensors_database(std::string database) {
	sensors_database = database;
}

bool DatabaseController::databaseCheck() {
	
	if (temps_database.compare("") == 0 || auth_database.compare("") == 0 || sensors_database.compare("") == 0) {
		return false;
	}
	
	//Check correct columns exist in temps database
	std::string database = temps_database;
	std::string sql = "SELECT COUNT(*) AS CNTREC FROM pragma_table_info('temps') WHERE name='id' or name='temp' or name = 'timestamp' or name = 'voltage';";
	
	std::string result = string_query(database, sql);
	
	if (result.compare("4") != 0) {
		return false;
	}
	
	//Check correct columns exist in auth database
	database = auth_database;
	sql = "SELECT COUNT(*) AS CNTREC FROM pragma_table_info('users') WHERE name='username' or name='password' or name = 'salt' or name = 'session';";
	
	result = string_query(database, sql);
	
	if (result.compare("4") != 0) {
		return false;
	}
	
	//Check correct columns exist in sensors database
	database = sensors_database;
	sql = "SELECT COUNT(*) AS CNTREC FROM pragma_table_info('sensor') WHERE name='id' or name='name' or name = 'short_name' or name = 'thermostat';";
	
	result = string_query(database, sql);
	
	if (result.compare("4") != 0) {
		return false;
	}
	
	return true;
}

// VOLTAGE

std::string DatabaseController::getSqlSensorVoltage() {

	std::stringstream ss;
	ss << "SELECT timestamp, voltage, id ";
	ss << "FROM temps ";
	ss << "WHERE voltage IS NOT NULL ";
	ss << "AND id IN (1,2,3,4,5);";

	return ss.str();	
}

std::string DatabaseController::getSqlSensorVoltage(int days) {
	
	std::stringstream ss;
	ss << "SELECT timestamp, voltage, id ";
	ss << "FROM temps ";
	ss << "WHERE id in (1,2,3,4,5) ";
	ss << "AND timestamp>datetime('now', 'localtime', '-";
	ss << days;
	ss << " days') AND voltage IS NOT NULL;";
	
	return ss.str();
}

std::vector<std::vector<std::string>> DatabaseController::getSensorVoltage() {

	std::string database = temps_database;
	std::string sql = getSqlSensorVoltage();
	std::vector<std::vector<std::string>> data = vector_string_query(database, sql);
	
	return data;	
}

std::vector<std::vector<std::string>> DatabaseController::getSensorVoltage(int days) {
	
	std::string database = temps_database;
	std::string sql = getSqlSensorVoltage(days);
	std::vector<std::vector<std::string>> data = vector_string_query(database, sql);
	
	return data;
}

//VOLTAGE END

//SENSORS

std::string DatabaseController::getSqlStatementForSensorName(int sensor) {
	
	std::ostringstream oss;
	oss << "select name from sensor where id = " << sensor;
	
	return oss.str();
}

std::string DatabaseController::getSqlStatementForSensorShortName(int sensor) {
	
	std::ostringstream oss;
	oss << "select short_name from sensor where id = " << sensor;
	
	return oss.str();
}

std::string DatabaseController::getSqlStatementForSensors() {
	
	std::string sql = "select * from sensor";
	
	return sql;
}

std::string DatabaseController::getSqlStatementForIdByName(std::string &name) {
	
	std::ostringstream oss;
	oss << "select id from sensor where name = '" << name << "'";
	
	return oss.str();
}

std::string DatabaseController::getSensorName(int sensor) {
	
	std::string database = sensors_database;
	std::string sql = getSqlStatementForSensorName(sensor);
	std::string data = string_query(database, sql);
	
	return data;
}

std::string DatabaseController::getSensorShortName(int sensor) {
	
	std::string database = sensors_database;
	std::string sql = getSqlStatementForSensorShortName(sensor);
	std::string data = string_query(database, sql);
	
	return data;
}

std::string DatabaseController::getSensorIdByName(std::string name) {
	
	std::string database = sensors_database;
	std::string sql = getSqlStatementForIdByName(name);
	std::string data = string_query(database, sql);
	
	return data;
}

std::vector<std::vector<std::string>> DatabaseController::getAllSensors() {
	
	std::string database = sensors_database;
	std::string sql = getSqlStatementForSensors();
	std::vector<std::vector<std::string>> data = vector_string_query(database, sql);
	
	return data;	
}

//SENSORS END

//CHART

std::string DatabaseController::getSqlStatementForChartFromDays(std::string &from, std::string &days) {

	std::ostringstream ss;
	ss << "SELECT timestamp, temp, id ";
	ss << "FROM temps ";
	ss << "WHERE id IN (1,2,3,4,5) ";
	ss << "AND timestamp BETWEEN date('" << from << "') AND date('" << from << "','+" << days << " days');";
	
	return ss.str();
}

std::string DatabaseController::getSqlStatementForChartDays(std::string &days) {

	std::ostringstream ss;
	ss << "SELECT timestamp, temp, id ";
	ss << "FROM temps ";
	ss << "WHERE id IN (1,2,3,4,5) ";
	ss << "AND timestamp > datetime('now', 'localtime', '-" << days << " days');";
	
	return ss.str();
}

std::string DatabaseController::getSqlStatementForChartFromTo(std::string &from, std::string &to) {

	std::ostringstream ss;
	ss << "SELECT timestamp, temp, id ";
	ss << "FROM temps ";
	ss << "WHERE id IN (1,2,3,4,5) ";
	ss << "AND timestamp BETWEEN date('" << from << "') ";
	ss << "AND date('" << to << "');";
	
	return ss.str();
}

std::vector<std::vector<std::string>> DatabaseController::getChartDataDays(std::string &days) {
	
	std::string database = temps_database;
	std::string sql = getSqlStatementForChartDays(days);
	std::vector<std::vector<std::string>> data = vector_string_query(database, sql);
	
	return data;
}

std::vector<std::vector<std::string>> DatabaseController::getChartDataFromTo(std::string &from, std::string &to) {
	
	std::string database = temps_database;
	std::string sql = getSqlStatementForChartFromTo(from, to);
	std::vector<std::vector<std::string>> data = vector_string_query(database, sql);
	
	return data;
}

std::vector<std::vector<std::string>> DatabaseController::getChartDataFromDays(std::string &from, std::string &days) {
	
	std::string database = temps_database;
	std::string sql = getSqlStatementForChartFromDays(from, days);
	std::vector<std::vector<std::string>> data = vector_string_query(database, sql);
	
	return data;
}

//CHART END


//AUTH

std::string DatabaseController::getSqlStatementForUserFromUsernamePassword(std::string &username, std::string &password) {
	
	std::string sql = "select username from users where username = '" + username + "' and password = '" + password + "'";
	
	return sql;
}

std::string DatabaseController::getSqlStatementForUserSalt(std::string &username) {
	
	std::string sql = "select salt from users where username = '" + username + "'";
	
	return sql;
}

std::string DatabaseController::getSqlStatementForUsernameFromSession(std::string &session) {
	
	std::string sql = "select username from users where session = '" + session + "'";
	
	return sql;
}

std::string DatabaseController::getSqlStatementForSessionFromUsername(std::string &username) {
	
	std::string sql = "select session from users where username = '" + username + "'";
	
	return sql;
}

std::string DatabaseController::getSqlStatementForSetUserSession(std::string &username, std::string &session) {
	
	std::string sql = "update users set session='" + session + "' where username = '" + username + "'";
	
	return sql;
}


std::string DatabaseController::getUserFromUsernamePassword(std::string &username, std::string &password) {
	
	std::string database = auth_database;
	std::string sql = getSqlStatementForUserFromUsernamePassword(username, password);
	std::string data = string_query(database, sql);
	
	return data;
}

std::string DatabaseController::getUserSalt(std::string &username) {

	std::string database = auth_database;
	std::string sql = getSqlStatementForUserSalt(username);
	std::string data = string_query(database, sql);
	
	return data;	
}

std::string DatabaseController::getUsernameFromSession(std::string &session) {
	
	std::string database = auth_database;
	std::string sql = getSqlStatementForUsernameFromSession(session);
	std::string data = string_query(database, sql);
	
	return data;
}

std::string DatabaseController::getSessionFromUsername(std::string &username) {
	
	std::string database = auth_database;
	std::string sql = getSqlStatementForSessionFromUsername(username);
	std::string data = string_query(database, sql);
	
	return data;
}

std::string DatabaseController::setUserSession(std::string &username, std::string &session) {
	
	std::string database = auth_database;
	std::string sql = getSqlStatementForSetUserSession(username, session);
	std::string data = string_query(database, sql);
	
	return data;
}


//AUTH END

//TEMPS

std::string DatabaseController::getSqlStatementForInsertTempData() {
	
	std::stringstream ss;
	ss << "INSERT INTO temps( id, timestamp, temp, voltage) ";
	ss << "VALUES(?,datetime('now', 'localtime'), ?, ?);";
	 
	return ss.str();
}

void DatabaseController::insertTempData(uint16_t deviceNum, float temp, uint16_t voltage) {
	
	while (locked) {
		std::cout << "L.. " << std::flush;
	}

	locked = true;

	sqlite3 *db = NULL;
	const char *dbPath = temps_database.c_str();
	sqlite3_stmt *stmt = NULL;
	
	int rc = sqlite3_open(dbPath, &db);
	// If rc is not 0, there was an error
	std::cout << "D1:" << rc << std::flush;
	if(rc) {
		std::cout << "S:ERRDB: " << dbPath << "," << rc << "," << sqlite3_errmsg(db) << "  ==  " << std::flush;
		sqlite3_close(db);
		system("/home/pi/RestartHub.sh");
		locked = false;
		return;
	}
	
	std::string sql_str = getSqlStatementForInsertTempData();
	const char *sql = sql_str.c_str();

    int sql_result = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);

	if(!sql_result == SQLITE_OK) {
		fprintf(stderr, "S:XXX: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		system("/home/pi/RestartHub.sh");
		locked = false;
		return;
	}

	sqlite3_bind_int(stmt, 1, deviceNum);
	sqlite3_bind_double(stmt, 2, temp);
	sqlite3_bind_double(stmt, 3,  (voltage * 0.001));
	sqlite3_step(stmt);  // Run SQL INSERT
	sqlite3_reset(stmt); // Clear statement handle for next use
	sqlite3_finalize(stmt);

	rc = sqlite3_close(db);
	int i = 0;
	while(rc != SQLITE_OK) {
		std::cout << "DB BUSY: " << std::flush;
		i++;
		if ( i > 10 ) {
			break;
		}
		sleep(1);
		rc = sqlite3_close(db);
		system("/home/pi/RestartHub.sh");
	}

	locked = false;
}

//TEMPS END

//IFTTT

std::string DatabaseController::getSqlGetTempFromId(std::string &id) {
	
	std::string sql = "select printf(\"%.1f\", temp) as temp from temps where id = " + id + " order by timestamp desc limit 1";
	
	return sql;
}

std::string DatabaseController::getTempFromId(std::string &id) {
	
	std::string database = temps_database;
	std::string sql = getSqlGetTempFromId(id);
	std::string data = string_query(database, sql);
	
	return data;
}

//IFTTT END

//CALLBACKS AND QUERIES

std::string DatabaseController::string_query(std::string &database, std::string &query) {
	
	const char *db_char = database.c_str();
	
	while (locked) {
		std::cout << "L.. " << std::flush;
	}

	locked = true;

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	
	std::string data;
//    std::cout << sqlite3_temp_directory << std::endl;

	rc = sqlite3_open(db_char, &db);
	if( rc ){
		fprintf(stderr, "s:SQL Open Error: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		system("/home/pi/RestartHub.sh");
		locked = false;
		return "";
	}
	
	const char *sql = query.c_str();
	
	rc = sqlite3_exec(db, sql, string_callback, &data, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL Exec error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		sqlite3_close(db);
		system("/home/pi/RestartHub.sh");
	}
	
	sqlite3_close(db);
	locked = false;
	return data;
}

std::vector<std::vector<std::string>> DatabaseController::vector_string_query(std::string &database, std::string &query) {
	
	const char *db_char = database.c_str();
	
	while (locked) {
		std::cout << "L.. " << std::flush;
	}
	
	locked = true;

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	const char *sql;
	std::vector<std::vector<std::string>> table;
	
	rc = sqlite3_open(db_char, &db);
	if( rc ) {
		//fprintf(stderr, "V:ERRDB: %s\n", sqlite3_errmsg(db));
		fprintf(stderr, "v:SQL Open Error: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		system("/home/pi/RestartHub.sh");
		locked = false;
		return table;
	}

	sql = query.c_str();

	rc = sqlite3_exec(db, sql, vector_string_callback, &table, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL Exec error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		sqlite3_close(db);
		system("/home/pi/RestartHub.sh");
	}
   
	sqlite3_close(db);
	
	locked = false;
	return table;
}

int DatabaseController::vector_string_callback(void *ptr, int argc, char* argv[], char* cols[]) {
	
	typedef std::vector<std::vector<std::string> > table_type;
    table_type* table = static_cast<table_type*>(ptr);
    std::vector<std::string> row;
    for (int i = 0; i < argc; i++) {
        row.push_back(argv[i] ? argv[i] : "(NULL)");
	}
    table -> push_back(row);
	
    return 0;
}

int DatabaseController::string_callback(void *ptr, int argc, char* argv[], char* cols[]) {
	
	std::string *pstr = static_cast<std::string *>(ptr);
	*pstr = argv[0];
	
	return 0;
}
