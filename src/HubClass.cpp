#include "HubClass.hpp"

RadioController radioControl;
Boiler boilr(radioControl);
TempSensorController tempSensControl(radioControl);
Timer timer;
Thermostat thermostat(boilr);
Emailer emailer;

int8_t volatile keepRunning = 1;

void HubClass::intHandler() {
	printf("\nStopping...\n");
	keepRunning = 0;
}

void HubClass::writeToFile(std::string message) {
	
	std::ofstream myfile;
	myfile.open ("timer.txt", std::ios_base::app);
	
	time_t t = time(0);   // get time now
	struct tm * now = localtime( &t );
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

	myfile << message << std::endl;
	myfile.close();
}

int HubClass::startHub(int argc, char** argv)  {

	tempSensControl.attach(&thermostat);
	tempSensControl.attach(&emailer);
	
	std::ifstream myfile("civet.conf");
	std::vector<std::string> myLines;
	std::copy(std::istream_iterator<std::string>(myfile),
	std::istream_iterator<std::string>(),
	std::back_inserter(myLines));

	
	timer.loadTimers();
	//CivetServer server(options);
	CivetServer server(myLines);

	server.addHandler(HOME_URI, new HomeHandler());
	server.addHandler(HUB_URI, new HubHandler(boilr, tempSensControl));
	server.addHandler(BOILER_URI, new BoilerHandler(boilr));
	server.addHandler(TIMER_URI, new TimerHandler(timer));
	server.addHandler(TIMER_ADD_URI, new TimerAddHandler(timer));
	server.addHandler(TIMER_ENABLE_URI, new TimerEnableHandler(timer));
	server.addHandler(TIMER_DISABLE_URI, new TimerDisableHandler(timer));
	server.addHandler(TIMER_DELETE_URI, new TimerDeleteHandler(timer));
	server.addHandler(CHART_URI, new ChartHandler());
	server.addHandler(VOLTAGE_URI, new VoltageHandler());
	server.addHandler(THERMOSTAT_URI, new ThermostatHandler(thermostat));
	server.addHandler(EMAILER_URI, new EmailerHandler(emailer));
	server.addHandler(IFTTT_URI, new IftttHandler(boilr, timer, tempSensControl));
	server.addHandler(LOGIN_URI, new LoginHandler());

	while(keepRunning) {
		checkTimer();
		tempSensControl.checkSensors();
		usleep(200000);
	}
	std::cout << "Ckosibg" << std::endl;
	return 0;
}

int timerCounterThingy=0;

void HubClass::checkTimer() {
	if (timerCounterThingy >= 5) {
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
				writeToFile("Checking timer: 0");
				boilr.TurnWaterOn(event1.duration);
			} else if(event1.boilerItem == 1) {
				writeToFile("Checking timer: 1");
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
