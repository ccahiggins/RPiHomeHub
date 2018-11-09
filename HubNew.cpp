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
#define BOILER_JSON_URI "/jsonboiler"
#define BOILER_JSON_STATUS_URI "/json/boiler/status"
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
//#include "ChartHandler.h"
//#include "ChartHandlerTest.h"
#include "DeleteTimerHandler.h"
#include "VoltageHandler.h"
#include "JsonBoilerHandler.h"
#include "JsonBoilerStatusHandler.h"
//#include "JsonHubHandler.h"


RadioController radioControl;
Boiler boilr(&radioControl);
TempsensorController tempsensControl(&radioControl);
Timer timer;

int8_t volatile keepRunning = 1;

void intHandler() {
	printf("\nStopping...\n");
	keepRunning = 0;
}

const char * options[] = {
	"document_root", DOCUMENT_ROOT,
	"listening_ports", PORT,
	"global_auth_file" , "./.htpasswd",
	0
};

void checkTimer();

void exiting(void) {
	using namespace std;
	cout << "exiting" << endl;
}

void writeToFile(std::string message) {
	using namespace std;
	
	ofstream myfile;
	myfile.open ("timer.txt", std::ios_base::app);
	
	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
	int hour=now->tm_hour;
	if (hour < 10)
		myfile << "0" << hour << ":" ;
	else
		myfile << hour << ":";
	int minute=now->tm_min;
	if (minute < 10)
		myfile << "0" << minute << ":" ;
	else
		myfile << minute << ":";
	int second = now->tm_sec;
	if (second < 10)
		myfile << "0" << second << ": ";
	else
		myfile << second << ": ";

	myfile << message << endl;
	myfile.close();
}

int main(int argc, char** argv)  {
	
	using namespace std;
	atexit(exiting);
	timer.loadTimers();
	CivetServer server(options);
	server.addHandler(HUB_URI, new HubHandler(&boilr, &tempsensControl));
	//server.addHandler(HUB_URI, new HubHandler(&boilr));
	server.addHandler(BOILER_URI, new BoilerHandler(&boilr));
	server.addHandler(TIMER_URI, new TimersHandler(&timer));
	server.addHandler(ADDTIMER_URI, new AddTimerHandler(&timer));
	server.addHandler(ENABLETIMER_URI, new EnableTimerHandler(&timer));
	server.addHandler(DISABLETIMER_URI, new DisableTimerHandler(&timer));
	server.addHandler(DELETETIMER_URI, new DeleteTimerHandler(&timer));
	//server.addHandler(CHART_URI, new ChartHandlerTest());
	server.addHandler(VOLTAGE_URI, new VoltageHandler());
	server.addHandler(BOILER_JSON_URI, new JsonBoilerHandler(&boilr));
	//server.addHandler(HUB_JSON_URI, new JsonHubHandler(&boilr));
	server.addHandler(BOILER_JSON_STATUS_URI, new JsonBoilerStatusHandler(&boilr));

	while(keepRunning) {
		checkTimer();
		tempsensControl.checkSensors();
		usleep(200000);
	}
	cout << "Ckosibg" << endl;
	return 0;
}

int timerCounterThingy=0;

void checkTimer() {
	if (timerCounterThingy >= 10) {
		Timer::TimerEvent* event;
			event = timer.checkTimer(1);
		if (event != NULL) {
			Timer::TimerEvent event1 = *event;
			timer.getNextTimerEvent();
			//cout << "received this event:" <<  event1.startHour << ":" << event1.startMinute << ", duration:" << event1.duration << ", boilerItem:" << event1.boilerItem << endl;
			/* cout << event << endl;
			cout << "Hour: " << event1.startHour << ", "
				 << "Minute: " << event1.startMinute << ", "
				 << "Duration: " << event1.duration  
				 << endl; */
			if(event1.boilerItem == 0) {
				//writeToFile("Checking timer: 0");
				boilr.TurnWaterOn(event1.duration);
			} else if(event1.boilerItem == 1) {
				//writeToFile("Checking timer: 1");
				boilr.TurnHeatingOn(event1.duration);
			} else {
				//writeToFile("Checking timer: X");
			}
		} else {
			//writeToFile("Checking timer: NULL");
		}
		timerCounterThingy = 0;
	} else {
		timerCounterThingy += 1;
	}
}
