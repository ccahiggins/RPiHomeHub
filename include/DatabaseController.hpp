#ifndef DATABASECONTROLLER_HPP
#define DATABASECONTROLLER_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>

#include <unistd.h>
#include <sqlite3.h>

class DatabaseController {

public:
    DatabaseController();
	
	static bool databaseCheck();
	
	static void set_temps_database(std::string database);
	static void set_auth_database(std::string database);
	static void set_sensors_database(std::string database);
	
	static std::vector<std::vector<std::string>> getSensorVoltage();
	static std::vector<std::vector<std::string>> getSensorVoltage(int days);
	
	static std::string getSensorName(int sensor);
	static std::string getSensorShortName(int sensor);
	static std::string getSensorIdByName(std::string name);
	static std::vector<std::vector<std::string>> getAllSensors();
	
	static std::vector<std::vector<std::string>> getChartDataDays(std::string &days);
	static std::vector<std::vector<std::string>> getChartDataFromTo(std::string &from, std::string &to);
	static std::vector<std::vector<std::string>> getChartDataFromDays(std::string &from, std::string &days);
	
	static std::string getUserFromUsernamePassword(std::string &username, std::string &password);
	static std::string getUserSalt(std::string &username);
	static std::string getUsernameFromSession(std::string &session);
	static std::string getSessionFromUsername(std::string &username);
	static std::string setUserSession(std::string &username, std::string &session);
	
	static std::string getTempFromId(std::string &id);
	
	static void insertTempData(uint16_t deviceNum, float temp, uint16_t voltage);
	
private:

	static std::string temps_database;
	static std::string auth_database;
	static std::string sensors_database;

	static std::string getSqlSensorVoltage();
	static std::string getSqlSensorVoltage(int days);
	
	static std::string getSqlStatementForSensorName(int sensor);
	static std::string getSqlStatementForSensorShortName(int sensor);
	static std::string getSqlStatementForIdByName(std::string &name);
	static std::string getSqlStatementForSensors();
	
	static std::string getSqlStatementForChartDays(std::string &days);
	static std::string getSqlStatementForChartFromTo(std::string &from, std::string &to);
	static std::string getSqlStatementForChartFromDays(std::string &from, std::string &days);
	
	static std::string getSqlStatementForUserFromUsernamePassword(std::string &username, std::string &password);
	static std::string getSqlStatementForUserSalt(std::string &username);
	static std::string getSqlStatementForUsernameFromSession(std::string &session);
	static std::string getSqlStatementForSessionFromUsername(std::string &username);
	static std::string getSqlStatementForSetUserSession(std::string &username, std::string &session);
	
	static std::string getSqlStatementForInsertTempData();
	
	static std::string getSqlGetTempFromId(std::string &id);
	
    	
	static std::string string_query(std::string &database, std::string &query);
	static std::vector<std::vector<std::string>> vector_string_query(std::string &database, std::string &query);
	

    static int string_callback(void *ptr, int argc, char* argv[], char* cols[]);
    static int vector_string_callback(void *ptr, int argc, char* argv[], char* cols[]);
};

#endif