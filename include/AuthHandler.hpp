#ifndef AUTHHANDLER_HPP
#define AUTHHANDLER_HPP

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "DatabaseController.hpp"

#include "CivetServer.h"
#include <boost/algorithm/string.hpp>
#include <openssl/sha.h>
#include <uuid/uuid.h>

class AuthHandler {

public:
    bool authorised(struct mg_connection *conn);
	bool authorised(struct mg_connection *conn, std::string role);
    bool login(struct mg_connection *conn, std::string &session, std::string &uri);
    std::string getUserSession(struct mg_connection *conn, std::string username);
	std::vector<std::string> getRolesForUser(std::string user);
	std::vector<std::string> getRolesForSessionId(std::string session);

private:
    std::string getUserPass(struct mg_connection *conn, std::string &username, std::string &password, std::string &uri);
    void getSession(struct mg_connection *conn, std::string &session);
    bool hasDBSession(std::string session);
    std::string getSalt(std::string username);
    std::string getDBSessionForUser(std::string user);
    bool checkUserPass(std::string username, std::string password);
    std::string sha256(const std::string str);
    std::string createSessionForUser(std::string user);
    std::string uuid();
};

#endif