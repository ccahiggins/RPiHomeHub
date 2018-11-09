#ifndef SERVERCONTROLLER_H
#define SERVERCONTROLLER_H
#include <unistd.h>

#include "CivetServer.h"

#define DOCUMENT_ROOT "."
#define PORT "8081"
#define HUB_URI "/hub"
//#define HUB_JSON_URI "/jsonhub"
#define BOILER_URI "/boiler"
//#define BOILER_JSON_URI "/jsonboiler"
//#define TIMERS_JSON_URI "/jsontimers"
//#define ADDTIMER_URI "/addtimer"
//#define DELETETIMER_URI "/deletetimer"
//#define TIMER_URI "/timer"
//#define ENABLETIMER_URI "/enabletimer"
//#define DISABLETIMER_URI "/disabletimer"
//#define POST_URI "/post"
//#define VOLTAGE_URI "/voltage"
//#define TIMERADDTIMER_URI "/addtimey"
//#define LIGHTSON_URI "/lightson"
//#define LIGHTSOFF_URI "/lightsoff"

#define CHART_URI "/chart"
#define CHART2_URI "/chart2"
#define EXIT_URI "/exit"

#include "TestHandler.h"

#include "HubHandler.h"
//#include "AddTimerHandler.h"
//#include "JsonHubHandler.h"
#include "BoilerHandler.h"
//#include "JsonBoilerHandler.h"
//#include "ChartHandler.h"
//#include "Chart2Handler.h"
//#include "JsonTimersHandler.h"
//#include "DeleteTimerHandler.h"
#include "TimersHandler.h"
//#include "EnableTimerHandler.h"
//#include "DisableTimerHandler.h"
//#include "PostHandler.h"
//#include "VoltageHandler.h"
//#include "TimerAddTimer.h"

class ServerController
{

public:
	ServerController(Boiler* boiler_);
};

#endif