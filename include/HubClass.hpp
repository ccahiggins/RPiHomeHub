#ifndef HUBCLASS_HPP
#define HUBCLASS_HPP

#include <fstream>
#include <iostream>
#include <iterator>

#include "Boiler.hpp"
#include "BoilerHandler.hpp"
#include "ChartHandler.hpp"
#include "DatabaseController.hpp"
#include "Emailer.hpp"
#include "EmailerHandler.hpp"
#include "HomeHandler.hpp"
#include "HubHandler.hpp"
#include "IftttHandler.hpp"
#include "LoginHandler.hpp"
#include "Observer.hpp"
#include "RadioController.hpp"
#include "SonoffHandler.hpp"
#include "TempSensorController.hpp"
#include "Thermostat.hpp"
#include "ThermostatHandler.hpp"
#include "Timer.hpp"
#include "TimerAddHandler.hpp"
#include "TimerDeleteHandler.hpp"
#include "TimerDisableHandler.hpp"
#include "TimerEnableHandler.hpp"
#include "TimerHandler.hpp"
#include "VoltageHandler.hpp"

#define DOCUMENT_ROOT "./html"
#define PORT "8081,443s"
#define CERT "html/server.pem"
#define CHART_URI "/chart"
#define HOME_URI "/new"
#define HUB_URI "/hub"
#define BOILER_URI "/boiler"
#define TIMER_URI "/timer"
#define TIMER_ADD_URI "/timer/add"
#define TIMER_DELETE_URI "/timer/delete"
#define TIMER_ENABLE_URI "/timer/enable"
#define TIMER_DISABLE_URI "/timer/disable"
#define VOLTAGE_URI "/voltage"
#define ADDTIMER_URI "/addtimer"
#define TEST_URI "/test"
#define LOGIN_URI "/login"
#define THERMOSTAT_URI "/thermostat"
#define EMAILER_URI "/emailer"
#define IFTTT_URI "/ifttt"
#define SONOFF_URI "/sonoff"

//#define LIGHTSON_URI "/lightson"
//#define LIGHTSOFF_URI "/lightsoff"

class HubClass {

    int8_t volatile keepRunning = 1;
    int timerCounterThingy = 0;

    void intHandler();
    void load_timer_events();

public:
    int startHub(int argc, char **argv);
};
#endif