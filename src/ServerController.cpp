#include "ServerController.hpp"

const char * options[] = { "document_root", DOCUMENT_ROOT,
		"listening_ports", PORT, "global_auth_file" , "./.htpasswd", 0
};


CivetServer server(options);

ServerController::ServerController(Boiler* boiler_, Timer* timer_) {
	std::cout << "SC" << std::endl;

	server.addHandler(HUB_URI, new HubHandler(boiler_));
	server.addHandler(BOILER_URI, new BoilerHandler(boiler_));
	server.addHandler(BOILER_URI, new TimersHandler(timer_));
}