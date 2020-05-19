#include "HubClass.hpp"

RadioController radioControl;
Boiler boiler(radioControl);
TempSensorController tempSensControl(radioControl);
Timer timer;
Thermostat thermostat(boiler);
Emailer emailer;

int8_t volatile keepRunning = 1;

void HubClass::intHandler() {
	printf("\nStopping...\n");
	keepRunning = 0;
}

int HubClass::startHub(int argc, char** argv)  {

	tempSensControl.attach(&thermostat);
	tempSensControl.attach(&emailer);
	load_timer_events();
	timer.start(std::chrono::milliseconds(1000), []{
		timer.check_timer();
	});

    std::ifstream hub_cfg;
    hub_cfg.open("hub.cfg");

    std::string hubcfg_line;

    while (hub_cfg >> hubcfg_line) {
        std::string delimiter = ":";
        std::string cfg1 = hubcfg_line.substr(0, hubcfg_line.find(delimiter));
        std::string cfg2 = hubcfg_line.substr(hubcfg_line.find(delimiter) + 1, hubcfg_line.length());


        if (cfg1.compare("volt_alert") == 0) {
            int volt_alert = std::stoi(cfg2);
            tempSensControl.set_low_voltage_trigger(volt_alert);
            std::cout << "Setting voltage alert to " << volt_alert << "V" << std::endl;
        }

        if (cfg1.compare("emailer") == 0) {
            if (cfg2.compare("on") == 0) {
                emailer.turn_emailer_on();
                std::cout << "Emailer on" << std::endl;
            } else if (cfg2.compare("off") == 0) {
                emailer.turn_emailer_off();
                std::cout << "Emailer off" << std::endl;
            }
        }

        if (cfg1.compare("temp_alert") == 0) {
            int temp_alert = std::stoi(cfg2);
            emailer.set_trigger_temp(temp_alert);
            std::cout << "Setting temperate alert to " << temp_alert << "C" << std::endl;
        }


    }
    hub_cfg.close();
	
	std::ifstream cfg_file("civet.conf");
	std::vector<std::string> config;
	std::copy(std::istream_iterator<std::string>(cfg_file),
		std::istream_iterator<std::string>(),
		std::back_inserter(config));

	CivetServer server(config);

	server.addHandler(HUB_URI, new HubHandler(boiler, tempSensControl));
	server.addHandler(HOME_URI, new HomeHandler());
	server.addHandler(TIMER_URI, new TimerHandler(timer));
	server.addHandler(BOILER_URI, new BoilerHandler(boiler));
	server.addHandler(EMAILER_URI, new EmailerHandler(emailer));
	server.addHandler(TIMER_ADD_URI, new TimerAddHandler(timer, boiler, thermostat));
	server.addHandler(IFTTT_URI, new IftttHandler(boiler, thermostat, timer, tempSensControl));
	server.addHandler(TIMER_ENABLE_URI, new TimerEnableHandler(timer));
	server.addHandler(TIMER_DISABLE_URI, new TimerDisableHandler(timer));
	server.addHandler(TIMER_DELETE_URI, new TimerDeleteHandler(timer));
	server.addHandler(THERMOSTAT_URI, new ThermostatHandler(thermostat));
	server.addHandler(VOLTAGE_URI, new VoltageHandler());
	server.addHandler(CHART_URI, new ChartHandler());

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

    unsigned int i = 0;
    if (events.size() > 1) {
    	bool done = false;
    	while (!done) {
	        if (events[i].compare("BOILER") == 0) {
	            std::shared_ptr<TimerEvent> ev(new BoilerTimerEvent(
	                std::stoi(events[i + 1]),	//hour
	                std::stoi(events[i + 2]),	//minute
	                std::stoi(events[i + 3]),	//one_time
	                std::stoi(events[i + 8]),	//item
	                std::stoi(events[i + 9]),	//duration
	                boiler
	            ));

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
