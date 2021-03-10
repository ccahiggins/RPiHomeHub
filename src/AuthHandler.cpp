#include "AuthHandler.hpp"


bool AuthHandler::login(struct mg_connection *conn, std::string &session, std::string &uri) {
	std::string username;
	std::string password;
	
	getUserPass(conn, username, password, uri);
	boost::erase_all(username, "'");
	boost::erase_all(username, ";");
	boost::erase_all(username, "\"");
	boost::erase_all(username, "=");
	boost::erase_all(password, "'");
	boost::erase_all(password, ";");
	boost::erase_all(password, "\"");
	boost::erase_all(password, "=");	
	std::string salt = getSalt(username);
	std::string passSalt = sha256(password + salt);
	//string passSalt = password + salt;
	bool loggedIn = checkUserPass(username, passSalt);
	if (loggedIn) {
		std::string sess = getDBSessionForUser(username);
		
		if (sess.empty()) {
			session = createSessionForUser(username);
		} else {
			session = sess;
		}
		return true;
	} else {
		session = "";
		return false;
	}
}

bool AuthHandler::authorised(struct mg_connection *conn) {
	
	std::string session;
	//const struct mg_request_info *req_info = mg_get_request_info(conn);
	//cout << "Loc: " << req_info->local_uri << endl;
	//getUserPass(conn, username, password);
	
	getSession(conn, session);
	//cout << "Auth: Get session" << session << endl;
	return hasDBSession(session);
}

std::string AuthHandler::getUserSession(struct mg_connection *conn, std::string username) {
	
	std::string session;
	getSession(conn, session);
	
	return session;
}

std::string AuthHandler::getUserPass(struct mg_connection *conn, std::string &username, std::string &password, std::string &uri) {

    char post_data[1024], user[sizeof(post_data)], pass[sizeof(post_data)], uri_post[sizeof(post_data)];
    int post_data_len;
	post_data_len = mg_read(conn, post_data, sizeof(post_data));
	mg_get_var(post_data, post_data_len, "username", user, sizeof(user));
	mg_get_var(post_data, post_data_len, "password", pass, sizeof(pass));
	mg_get_var(post_data, post_data_len, "uri", uri_post, sizeof(uri_post));
	
	username = std::string(user);
	password = std::string(pass);
	uri = std::string(uri_post);
	
	//string session = hasDBUserPass(username, password);
	return "kjlkJ";
}



void AuthHandler::getSession(struct mg_connection *conn, std::string &session) {

	const char *cookie = mg_get_header(conn, "Cookie");
    char session_id[38];
    mg_get_cookie(cookie, "session", session_id, sizeof(session_id));
	//cout << "Getcookie: " << aaaaa << endl;
	//cout << "GetSession(): " << session_id << endl;
   //for (i = 0; i < MAX_SESSIONS; i++) {
   //    if (sessions[i].expire != 0 &&
   //        sessions[i].expire > now &&
   //        strcmp(sessions[i].session_id, session_id) == 0) {
   //        break;
   //    }
   //}
   
   session = std::string(session_id);
	
}

std::string AuthHandler::getSalt(std::string username) {

	std::string salt = DatabaseController::getUserSalt(username);
	
	return salt;
}

bool AuthHandler::checkUserPass(std::string username, std::string password)
{

	std::string user = DatabaseController::getUserFromUsernamePassword(username, password);
	
	if (username.compare(user) == 0) {
		return true;
	} else {
		return false;
	}
}

bool AuthHandler::hasDBSession(std::string session)
{

	std::string sesh = DatabaseController::getUsernameFromSession(session);
	
	if (sesh.compare("") != 0) {
		return true;
	} else {
		return false;
	}
}

std::string AuthHandler::createSessionForUser(std::string user) {
	
	std::string session = uuid();
	
	std::string sess = DatabaseController::setUserSession(user, session);
	
	if (!sess.empty()) {
		return session;
	} else {
		return "";
	}
	
}

std::string AuthHandler::getDBSessionForUser(std::string user)
{
	
	std::string session = DatabaseController::getSessionFromUsername(user);
	
	return session;
}


std::string AuthHandler::sha256(const std::string str)
{
	
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

std::string AuthHandler::uuid() {
	
	uuid_t id;
	uuid_generate(id);
	char *rrr = new char[100];
	uuid_unparse(id, rrr);
	std::string uuuuuid = std::string(rrr);
	delete[] rrr;
	rrr = nullptr;

	return uuuuuid;
}