#ifndef HUBCLASS_HPP
#define HUBCLASS_HPP

#include <iostream>
#include <fstream>
#include <iterator>

#include "RadioController.hpp"
#include "TempSensorController.hpp"
#include "Boiler.hpp"
#include "Timer.hpp"
#include "Thermostat.hpp"

#define DOCUMENT_ROOT "./html"
#define PORT "8081,443s"
#define CERT "html/server.pem"
#define CHART_URI "/chart"
#define CHART2_URI "/chart2"
#define EXIT_URI "/exit"

#define HOME_URI "/new"
#define HUB_URI "/hub"
#define HUB_JSON_URI "/jsonhub"
#define BOILER_URI "/boiler"
#define BOILER_JSON_URI "/jsonboiler"
#define BOILER_JSON_STATUS_URI "/json/boiler/status"
//#define TIMERS_JSON_URI "/jsontimers"
#define TIMER_URI "/timer"
#define TIMER_ADD_URI "/timer/add"
#define TIMER_DELETE_URI "/timer/delete"
#define TIMER_ENABLE_URI "/timer/enable"
#define TIMER_DISABLE_URI "/timer/disable"
//#define POST_URI "/post"
#define VOLTAGE_URI "/voltage"
#define ADDTIMER_URI "/addtimer"
#define TEST_URI "/test"
#define LOGIN_URI "/login"
#define THERMOSTAT_URI "/thermostat"

#define IFTTT_URI "/ifttt"
//#define LIGHTSON_URI "/lightson"
//#define LIGHTSOFF_URI "/lightsoff"

#include "IftttHandler.hpp"
#include "TimerHandler.hpp"
#include "TimerDeleteHandler.hpp"
#include "TimerAddHandler.hpp"
#include "TimerEnableHandler.hpp"
#include "TimerDisableHandler.hpp"
#include "HubHandler.hpp"
#include "BoilerHandler.hpp"
#include "ChartHandler.hpp"
//#include "ChartHandlerTest.hpp"
#include "VoltageHandler.hpp"
//#include "TestHandler.hpp"
#include "LoginHandler.hpp"
#include "HomeHandler.hpp"
//#include "JsonBoilerHandler.hpp"
//#include "JsonBoilerStatusHandler.hpp"
//#include "JsonHubHandler.h"
#include "ThermostatHandler.hpp"

#include "Observer.hpp"

class HubClass {

int8_t volatile keepRunning = 1;
int timerCounterThingy=0;

void intHandler();
void checkTimer();
void writeToFile(std::string message);

public:
  int startHub(int argc, char** argv);

};
#endif