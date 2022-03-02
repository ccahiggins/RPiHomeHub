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
	
    std::ifstream hub_cfg;
    hub_cfg.open("hub.cfg");

    std::string hubcfg_line;

	std::string api_key;
	std::string api_url;
	std::string ga_key;

    while (hub_cfg >> hubcfg_line) {
        std::string delimiter = ":";
        std::string cfg1 = hubcfg_line.substr(0, hubcfg_line.find(delimiter));
        std::string cfg2 = hubcfg_line.substr(hubcfg_line.find(delimiter) + 1, hubcfg_line.length());

        if (cfg1.compare("volt_alert") == 0) {
            float volt_alert = std::stof(cfg2);
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

		if (cfg1.compare("temp_db") == 0) {
            DatabaseController::set_temps_database(cfg2);
            std::cout << "Setting temps database to " << cfg2 << std::endl;
        }
		
		if (cfg1.compare("auth_db") == 0) {
            DatabaseController::set_auth_database(cfg2);
            std::cout << "Setting auth database to " << cfg2 << std::endl;
        }
		
		if (cfg1.compare("sensors_db") == 0) {
            DatabaseController::set_sensors_database(cfg2);
            std::cout << "Setting sensors database to " << cfg2 << std::endl;
        }

		if (cfg1.compare("api_key") == 0) {
			api_key = cfg2;
            std::cout << "Setting API Key..." << std::endl;
        }

		if (cfg1.compare("api_url") == 0) {
			api_url = cfg2;
            std::cout << "Setting API URL..." << std::endl;
        }

		if (cfg1.compare("ga_key") == 0) {
			ga_key = cfg2;
            std::cout << "Setting GA Key..." << std::endl;
        }

    }
    hub_cfg.close();
	
	if (DatabaseController::databaseCheck()) {
		std::cout << "Databases looks good" << std::endl;
	} else {
		std::cout << "DATABASE ERROR" << std::endl;
		exit(1);
	}
	
	tempSensControl.attach(&thermostat);
	tempSensControl.attach(&emailer);
	load_timer_events();
	timer.start(std::chrono::milliseconds(1000), []{
		timer.check_timer();
	});
	
	std::ifstream cfg_file("civet.conf");
	std::vector<std::string> config;
	std::copy(std::istream_iterator<std::string>(cfg_file), std::istream_iterator<std::string>(), std::back_inserter(config));

    config.push_back("additional_header");
    config.push_back("X-XSS-Protection: 0\r\nContent-Security-Policy: default-src 'self'; frame-ancestors 'self'; form-action 'self';\r\nX-Frame-Options: DENY");

	CivetServer server(config);

	std::string ip1="192.168.1.101";
	std::string ip2="192.168.1.102";
	std::string ip3="192.168.1.103";
	Sonoff s1(ip1);
	Sonoff s2(ip2);
	Sonoff s3(ip3);
	std::vector<Sonoff> sonoff_list;
	sonoff_list.push_back(s1);
	sonoff_list.push_back(s2);
	sonoff_list.push_back(s3);

	server.addHandler(HUB_URI, new HubHandler(boiler, tempSensControl));
	server.addHandler(HOME_URI, new HomeHandler(api_key, api_url, ga_key));
	server.addHandler(TIMER_URI, new TimerHandler(timer));
	server.addHandler(BOILER_URI, new BoilerHandler(boiler));
	server.addHandler(EMAILER_URI, new EmailerHandler(emailer));
	server.addHandler(TIMER_ADD_URI, new TimerAddHandler(timer, boiler, thermostat));
	server.addHandler(TIMER_ENABLE_URI, new TimerEnableHandler(timer));
	server.addHandler(TIMER_DISABLE_URI, new TimerDisableHandler(timer));
	server.addHandler(TIMER_DELETE_URI, new TimerDeleteHandler(timer));
	server.addHandler(THERMOSTAT_URI, new ThermostatHandler(thermostat));
	server.addHandler(VOLTAGE_URI, new VoltageHandler());
	server.addHandler(SONOFF_URI, new SonoffHandler(sonoff_list));
	server.addHandler(CHART_URI, new ChartHandler());
	server.addHandler(LOGIN_URI, new LoginHandler());
	server.addHandler(IFTTT_URI, new IftttHandler(boiler, thermostat, timer, tempSensControl, sonoff_list));

	while(keepRunning) {
		tempSensControl.checkSensors();
		usleep(200000);
	}
	std::cout << "Closing" << std::endl;
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
