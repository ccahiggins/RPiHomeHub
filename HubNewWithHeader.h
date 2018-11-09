#ifndef HUBNEWWITHHEADER_H
#define HUBNEWWITHHEADER_H

#include <iostream>
#include <fstream>

#include "RadioController.h"
#include "TempsensorController.h"
#include "Boiler.h"
#include "Timer.h"

#define DOCUMENT_ROOT "./html"
#define PORT "8081"
#define CERT "html/server.pem"
#define CHART_URI "/chart"
#define CHART2_URI "/chart2"
#define EXIT_URI "/exit"

#define HUB_URI "/hub"
#define HUB_JSON_URI "/jsonhub"
#define BOILER_URI "/boiler"
//#define BOILER_JSON_URI "/jsonboiler"
//#define TIMERS_JSON_URI "/jsontimers"
#define ADDTIMER_URI "/addtimer"
#define DELETETIMER_URI "/deletetimer"
#define TIMER_URI "/timer"
#define ENABLETIMER_URI "/enabletimer"
#define DISABLETIMER_URI "/disabletimer"
//#define POST_URI "/post"
#define VOLTAGE_URI "/voltage"
#define ADDTIMER_URI "/addtimer"
//#define LIGHTSON_URI "/lightson"
//#define LIGHTSOFF_URI "/lightsoff"

#include "AddTimerHandler.h"
#include "EnableTimerHandler.h"
#include "DisableTimerHandler.h"
#include "TimersHandler.h"
#include "HubHandler.h"
#include "BoilerHandler.h"
#include "ChartHandler.h"
#include "DeleteTimerHandler.h"
#include "VoltageHandler.h"
#include "JsonHubHandler.h"

class HubNewWithHeader {

public:
	int main(int argc, char** argv);

private:

	void intHandler();

	void checkTimer();

	static void exiting(void);

	void writeToFile(std::string message);

};
#endif