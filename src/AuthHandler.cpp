#include "AuthHandler.hpp"


bool AuthHandler::login(struct mg_connection *conn, std::string &session, std::string &uri) {
	using namespace std;
	string username;
	string password;
	
	getUserPass(conn, username, password, uri);
	boost::erase_all(username, "'");
	boost::erase_all(username, ";");
	boost::erase_all(username, "\"");
	boost::erase_all(username, "=");
	boost::erase_all(password, "'");
	boost::erase_all(password, ";");
	boost::erase_all(password, "\"");
	boost::erase_all(password, "=");	
	string salt = getSalt(username);
	string passSalt = sha256(password + salt);
	//string passSalt = password + salt;
	bool loggedIn = checkUserPass(username, passSalt);
	if (loggedIn) {
		string sess = getDBSessionForUser(username);
		
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
	using namespace std;
	//cout << "AuthHandler" << endl;
	
	//string content = "Test POST";

	//string username;
	//string password;
	string session;
	//const struct mg_request_info *req_info = mg_get_request_info(conn);
	//cout << "Loc: " << req_info->local_uri << endl;
	//getUserPass(conn, username, password);
	
	getSession(conn, session);
	//cout << "Auth: Get session" << session << endl;
	return hasDBSession(session);
}

std::string AuthHandler::getUserSession(struct mg_connection *conn, std::string username) {
	using namespace std;
	
	string session;
	getSession(conn, session);
	
	return session;
}

std::string AuthHandler::getUserPass(struct mg_connection *conn, std::string &username, std::string &password, std::string &uri) {
	using namespace std;
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
	using namespace std;
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

std::string AuthHandler::getSalt(std::string username)
{
	using namespace std;
	//cout << "Getting session for " << username << endl;
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	
	string sqlStatement = "select salt from users where username = '" + username + "'";
	
	vector<vector<string> > salts;

	rc = sqlite3_open("db/auth.db", &db);
	//rc = sqlite3_open("/media/ramdisk/sqlTemplog.db", &db);
	if( rc ){
		fprintf(stderr, "C:ERRDB: %s\n", sqlite3_errmsg(db));
		return "";
	}
	
	const char *sql = sqlStatement.c_str();
	
	//int startT=clock();
	rc = sqlite3_exec(db, sql, callback, &salts, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "C:SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	
	sqlite3_close(db);
	
	if (salts.size() > 0) {
		return salts[0][0];
	} else {
		return "";
	}
}

bool AuthHandler::checkUserPass(std::string username, std::string password)
{
	using namespace std;
	//cout << "Getting login for " << username << endl;
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	
	string sqlStatement = "select * from users where username = '" + username + "' and password = '" + password + "'";
	
	vector<vector<string> > users;

	rc = sqlite3_open("db/auth.db", &db);
	//rc = sqlite3_open("/media/ramdisk/sqlTemplog.db", &db);
	if( rc ){
		fprintf(stderr, "C:ERRDB: %s\n", sqlite3_errmsg(db));
		return "";
	}
	
	const char *sql = sqlStatement.c_str();
	
	//int startT=clock();
	rc = sqlite3_exec(db, sql, callback, &users, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "C:SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	
	sqlite3_close(db);
	
	if (users.size() > 0) {
		return true;
	} else {
		return false;
	}
}

bool AuthHandler::hasDBSession(std::string session)
{
	using namespace std;
	//cout << "Getting session for " << session << endl;
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	
	string sqlStatement = "select username from users where session = '" + session + "'";
	
	vector<vector<string> > sessions;

	rc = sqlite3_open("db/auth.db", &db);
	//rc = sqlite3_open("/media/ramdisk/sqlTemplog.db", &db);
	if( rc ){
		fprintf(stderr, "C:ERRDB: %s\n", sqlite3_errmsg(db));
		return "";
	}
	
	const char *sql = sqlStatement.c_str();
	
	//int startT=clock();
	rc = sqlite3_exec(db, sql, callback, &sessions, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "C:SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	
	sqlite3_close(db);
	
	if (sessions.size() > 0) {
		return true;
	} else {
		return false;
	}
}

std::string AuthHandler::createSessionForUser(std::string user) {
	using namespace std;
	
	string session = uuid();
	
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	
	string sqlStatement = "update users set session='" + session + "' where username = '" + user + "'";
	
	vector<vector<string> > sessions;

	rc = sqlite3_open("db/auth.db", &db);
	//rc = sqlite3_open("/media/ramdisk/sqlTemplog.db", &db);
	if( rc ){
		fprintf(stderr, "C:ERRDB: %s\n", sqlite3_errmsg(db));
		return "";
	}
	
	const char *sql = sqlStatement.c_str();
	
	//int startT=clock();
	rc = sqlite3_exec(db, sql, callback, &sessions, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "C:SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	
	sqlite3_close(db);
	//cout << "------" << endl;
	if (sessions.size() > 0) {
		return sessions[0][0];
	} else {
		return "";
	}
	
}

std::string AuthHandler::getDBSessionForUser(std::string user)
{
	using namespace std;
	
	//cout << "Getting session for user " << user << endl;
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	
	string sqlStatement = "select session from users where username = '" + user + "'";
	
	vector<vector<string> > sessions;

	rc = sqlite3_open("db/auth.db", &db);
	//rc = sqlite3_open("/media/ramdisk/sqlTemplog.db", &db);
	if( rc ){
		fprintf(stderr, "C:ERRDB: %s\n", sqlite3_errmsg(db));
		return "";
	}
	
	const char *sql = sqlStatement.c_str();
	
	//int startT=clock();
	rc = sqlite3_exec(db, sql, callback, &sessions, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "C:SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	
	sqlite3_close(db);
	//cout << "------" << endl;
	if (sessions.size() > 0) {
		return sessions[0][0];
	} else {
		return "";
	}
}


std::string AuthHandler::sha256(const std::string str)
{
	using namespace std;
	
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

std::string AuthHandler::uuid() {
	using namespace std;
	
	uuid_t id;
	uuid_generate(id);
	char *rrr = new char[100];
	uuid_unparse(id, rrr);
	string uuuuuid=string(rrr);

	return uuuuuid;
}

int AuthHandler::callback(void *ptr, int argc, char* argv[], char* cols[])
{
	using namespace std;
	//cout << "DB callback" << endl;
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
