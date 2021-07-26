#include "Sonoff.hpp"

Sonoff::Sonoff(std::string& ip_adress_) : ip_adress(ip_adress_) {};

int Sonoff::turn_on() {
	
	std::string url = "http://" + ip_adress + "/cm?cmnd=Power%20On";
	cpr::Response r = cpr::Get(cpr::Url{url});

	if (r.status_code == 200 && r.text.compare("{\"POWER\":\"ON\"}") == 0) {
		return 1;
	} else {
		return -1;
	}
}

int Sonoff::turn_off() {
	
	std::string url = "http://" + ip_adress + "/cm?cmnd=Power%20off";
	cpr::Response r = cpr::Get(cpr::Url{url});

	if (r.status_code == 200 && r.text.compare("{\"POWER\":\"OFF\"}") == 0) {
		return 0;
	} else {
		return -1;
	}
}

int Sonoff::toggle() {
	
	std::string url = "http://" + ip_adress + "/cm?cmnd=Power%20TOGGLE";
	cpr::Response r = cpr::Get(cpr::Url{url});

	if (r.status_code == 200 && r.text.compare("{\"POWER\":\"OFF\"}") == 0) {
		return 0;
	} else if (r.status_code == 200 && r.text.compare("{\"POWER\":\"ON\"}") == 0) {
		return 1;
	} else {
		return -1;
	}
}

int Sonoff::status() {
	
	std::string url = "http://" + ip_adress + "/cm?cmnd=Power";
	cpr::Response r = cpr::Get(cpr::Url{url});

	if (r.status_code == 200 && r.text.compare("{\"POWER\":\"OFF\"}") == 0) {
		return 0;
	} else if (r.status_code == 200 && r.text.compare("{\"POWER\":\"ON\"}") == 0) {
		return 1;
	} else {
		return -1;
	}
}