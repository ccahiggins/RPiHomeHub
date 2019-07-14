#ifndef AUTHHANDLER_HPP
#define AUTHHANDLER_HPP

#include <string>
#include <sstream>
#include <iomanip>

#include "CivetServer.h"
#include <sqlite3.h>
#include <openssl/sha.h>
#include <uuid/uuid.h>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <cstring>

class AuthHandler
{
	
public:
	bool authorised(struct mg_connection *conn);
	bool login(struct mg_connection *conn, std::string &session, std::string &uri);
	std::string getUserSession(struct mg_connection *conn, std::string username);
	
private:
	std::string getUserPass(struct mg_connection *conn, std::string &username, std::string &password, std::string &uri);
	void getSession(struct mg_connection *conn, std::string &session);
	bool hasDBSession(std::string session);
	std::string getSalt(std::string username);
	std::string getDBSessionForUser(std::string user);
	static int callback(void *ptr, int argc, char* argv[], char* cols[]);
	bool checkUserPass(std::string username, std::string password);
	std::string sha256(const std::string str);
	std::string createSessionForUser(std::string user);
	std::string uuid();
};

#endif