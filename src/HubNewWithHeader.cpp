#include "HubNewWithHeader.hpp"

	RadioController radioControl;
	Boiler boilr(&radioControl);
	TempsensorController tempsensControl(&radioControl);
	Timer timer;

int8_t volatile keepRunning = 1;

void HubNewWithHeader::intHandler() {
	printf("\nStopping...\n");
	keepRunning = 0;
}

const char * options[] = {
	"document_root", DOCUMENT_ROOT,
	"listening_ports", PORT,
	"global_auth_file" , "./.htpasswd",
	0
};

void HubNewWithHeader::exiting(void) {
	using namespace std;
	cout << "exiting" << endl;
}

void HubNewWithHeader::writeToFile(std::string message) {
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

int HubNewWithHeader::main(int argc, char** argv)  {
	
	using namespace std;
	atexit(exiting);
	timer.loadTimers();
	CivetServer server(options);
	server.addHandler(HUB_URI, new HubHandler(&boilr));
	server.addHandler(BOILER_URI, new BoilerHandler(&boilr));
	server.addHandler(TIMER_URI, new TimersHandler(&timer));
	server.addHandler(ADDTIMER_URI, new AddTimerHandler(&timer));
	server.addHandler(ENABLETIMER_URI, new EnableTimerHandler(&timer));
	server.addHandler(DISABLETIMER_URI, new DisableTimerHandler(&timer));
	server.addHandler(DELETETIMER_URI, new DeleteTimerHandler(&timer));
	server.addHandler(CHART_URI, new ChartHandler());
	server.addHandler(VOLTAGE_URI, new VoltageHandler());
	server.addHandler(HUB_JSON_URI, new JsonHubHandler(&boilr));
	
	while(keepRunning) {
		checkTimer();
		tempsensControl.checkSensors();
		usleep(100000);
	}
	cout << "Ckosibg" << endl;
	return 0;
}

int timerCounterThingy=0;

void HubNewWithHeader::checkTimer() {
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
