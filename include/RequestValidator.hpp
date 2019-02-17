#ifndef REQUESTVALIDATOR_HPP
#define REQUESTVALIDATOR_HPP
#
#include <unistd.h>
#include <iostream>
#include "RequestType.hpp"


class RequestValidator {

public:
	RequestValidator(const char* _request);
	RequestType getType();
	bool isValid();
	std::string getDuration();
	std::string getRoom();
	std::string getHour();
	std::string getMinute();
	std::string getTimerId();
	std::string getOneTime();

private:
	RequestType requestType;
	std::string charToString(const char * txt);
	std::string request;
	std::string duration;
	std::string hour;
	std::string minute;
	std::string onetime;
	std::string timerid;
	std::string room;
	bool validNum(std::string numTxt, int minnum, int maxnum);
	bool validNum(std::string numTxt);
	bool validateRequest(std::string request);
	std::string pathPart(std::string str, int num);
	bool valid;
};

#endif