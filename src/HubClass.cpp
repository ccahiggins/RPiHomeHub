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
	load_timer_events();
	timer.start(std::chrono::milliseconds(1000), [&timer]{
		timer.check_timer();
	});
	
	std::ifstream myfile("civet.conf");
	std::vector<std::string> myLines;
	std::copy(std::istream_iterator<std::string>(myfile),
	std::istream_iterator<std::string>(),
	std::back_inserter(myLines));

	
	//timer.loadTimers();
	//CivetServer server(options);
	CivetServer server(myLines);

	server.addHandler(HOME_URI, new HomeHandler());
	server.addHandler(HUB_URI, new HubHandler(boilr, tempSensControl));
	server.addHandler(BOILER_URI, new BoilerHandler(boilr));
	server.addHandler(TIMER_URI, new TimerHandler(timer));
	server.addHandler(TIMER_ADD_URI, new TimerAddHandler(timer, boilr));
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
		tempSensControl.checkSensors();
		usleep(200000);
	}
	std::cout << "Ckosibg" << std::endl;
	return 0;
}

void HubClass::load_timer_events() {
    std::cout << "\033[32mLoading timers\033[0m" << std::endl;

    std::ifstream f;
    f.open("timers.txt");

    std::string x;
    std::vector<std::string> events;

    while (f >> x) {
        events.push_back(x);
    }

    int i = 0;
    if (events.size() > 1) {
    	bool done = false;
    	while (!done) {
    		std::cout << "Timer: " << events[i] << std::endl;
	        if (events[i].compare("BOILER") == 0) {
	            std::shared_ptr<TimerEvent> ev(new BoilerTimerEvent(
	                std::stoi(events[i + 1]),	//hour
	                std::stoi(events[i + 2]),	//minute
	                std::stoi(events[i + 3]),	//one_time
	                std::stoi(events[i + 8]),	//item
	                std::stoi(events[i + 9]),	//duration
	                boilr
	            ));

	            std::cout << "BoilerTimer: " 
	            	<< events[i + 1] <<  ","
	            	<< events[i + 2] <<  ","
	            	<< events[i + 3] <<  ","
	            	<< events[i + 4] <<  ","
	            	<< events[i + 5] <<  ","
	            	<< events[i + 6] <<  ","
	            	<< events[i + 7] <<  ","
	            	<< events[i + 8] <<  ","
	            	<< events[i + 9]
	             	<< std::endl;
	            i += 10;
	            timer.add_event(ev);
	            if (i > events.size() - 2) {
	            	done = true;
	            }
	        } else if (events[i].compare("THERMOSTAT") == 0) {
	            std::shared_ptr<TimerEvent> ev(new ThermostatTimerEvent(
	                std::stoi(events[i + 1]),	//hour
	                std::stoi(events[i + 2]),	//minute
	                std::stoi(events[i + 3]),	//one_time
	                std::stoi(events[i + 8]),	//on_off
	                std::stoi(events[i + 9]),	//room
	                std::stof(events[i + 10]),	//temp
	                thermostat
	            ));
	            std::cout << "ThermostatTimer: " 
	            	<< events[i + 1] <<  ","
	            	<< events[i + 2] <<  ","
	            	<< events[i + 3] <<  ","
	            	<< events[i + 4] <<  ","
	            	<< events[i + 5] <<  ","
	            	<< events[i + 6] <<  ","
	            	<< events[i + 8] <<  ","
	            	<< events[i + 9] <<  ","
	            	<< events[i + 10]
	            	<< std::endl;
	            i += 11;
	            timer.add_event(ev);
	            if (i > events.size() - 2) {
	            	done = true;
	            }
	        }
    	}
    }
    
    f.close();
}