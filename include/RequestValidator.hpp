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
	std::string getOnOff();
	std::string getTemp();

private:
	RequestType requestType;
	std::string charToString(const char * txt);
	std::string request;
	std::string duration;
	std::string hour;
	std::string minute;
	std::string onetime;
	std::string timerid;
	std::string onoff;
	std::string room;
	std::string temp;
	bool validNum(std::string numTxt, int minnum, int maxnum);
	bool validNum(std::string numTxt);
	bool valid_float(std::string num_text, int min_num, int max_num);
	bool valid_float(std::string num_text);
	bool validateRequest(std::string request);
	std::string pathPart(std::string str, int num);
	bool valid;
	int MIN_THERMOSTAT_TEMP = 0;
	int MAX_THERMOSTAT_TEMP = 100;
	int MIN_ROOM = 1;
	int MAX_ROOM = 5;
};

#endif