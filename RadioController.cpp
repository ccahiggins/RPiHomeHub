#include "RadioController.h"

// Header types:
// 't' Temperature sensor
// 'd' Temperature sensor test
// 'p' Boiler

const int COUNT = 5;

volatile bool radioInUse;

RadioController::RadioController():radio(RPI_V2_GPIO_P1_22, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ),network(radio) {
	using namespace std;

	cout << "RC" << endl;

	radio.begin();
	const uint16_t node = 00;

	radio.setChannel(90);
	radio.setDataRate(RF24_250KBPS);
	radio.setAutoAck(true);
	//radio.printDetails();
	delay(5);
	network.begin(node);
	std::cout << "RC" << std::endl;
}

void RadioController::writeToFile(std::string text) {
    using namespace std;
    ofstream myfile;
    myfile.open ("logs/inUseThingy.txt", ios::app);
    myfile << text << endl;
    myfile.close();
}

bool RadioController::sendPayload(RF24NetworkHeader header, const void * payload, uint16_t size) {
	using namespace std;

	while (radioInUse)
	{
		writeToFile("Sleeping1");
		cout << "Sleeping1" << endl;
		usleep(5000);
	}
	radioInUse = true;
	
	writeToFile("SendPayloadInUse");
	
	bool ok = false;
	int counter = 0;
	
	while (!ok && counter < COUNT) {
		ok = network.write(header,payload,size);
		counter++;
	}
	writeToFile("SendPayloadNotInUse");
	usleep(100);
	radioInUse = false;
	
	return ok;
}

/* RadioController::payload_boiler_status RadioController::getBoilerPayload() {
	using namespace std;

	RF24NetworkHeader header;
	payload_boiler payload;
	
	while (radioInUse)
	{
		writeToFile("Sleeping2");
		cout << "Sleeping2" << endl;
		usleep(1000);
	}
	radioInUse = true;
	writeToFile("getBoilerPayloadInUse");
	bool yeah=false;
	
	network.update();
	while (network.available()) {
		network.peek (header);
		
		if (header.type=='p') {
			network.read(header, &payload, sizeof(payload));
			yeah = true;
		}
		network.read(header, &payload, sizeof(payload));
	}
	writeToFile("getBoilerPayloadNotInUse");
	radioInUse = false;
	
	if (yeah) {
		
		return payload;
	}

	payload.deviceNum = 65535;
	return payload;
} */

RadioController::payload_temp RadioController::getTempPayload() {
	using namespace std;

	RF24NetworkHeader header;
	payload_temp payload;

	while (radioInUse)
	{
		writeToFile("Sleeping3");
		cout << "Sleeping3" << endl;
		usleep(5000);
	}
	radioInUse = true;
	writeToFile("getTempPayloadInUse");
	
	bool yeah = false;
	
	network.update();
	
	while (network.available()) {
		network.peek(header);
		uint16_t f1 = 1;
		uint16_t f2 = 2;
		uint16_t f3 = 4;
		if (header.from_node == f1 || header.from_node == f2 || header.from_node == f3) {
			network.read(header, &payload, sizeof(payload));
			yeah = true;
		} else {
			std::cout << "something else: " << "From:" << header.from_node << ", type: " << header.type << std::endl;
			network.read(header, &payload, sizeof(payload));
		}
	}

	writeToFile("getTempPayloadNotInUse");
	usleep(100);
	radioInUse = false;

	if (yeah) {
		return payload;
	}
	
	payload.deviceNum = 65535;
	return payload;
}

RadioController::payload_boiler_status RadioController::sendGetBoilerPayload(RF24NetworkHeader header, const void * payload, uint16_t size) {
	using namespace std;
	
	while (radioInUse)
	{
		writeToFile("Sleeping4");
		cout << "Sleeping4" << endl;
		usleep(5000);
	}
	radioInUse = true;
	writeToFile("sendGetBoilerPayloadInUse");

	bool ok = false;
	int counter = 0;
	cout << "X|" << flush;
	while (!ok && counter < COUNT) {
		writeToFile("while counter thing");
		ok = network.write(header,payload,size);
		counter++;
		cout << "W|" << flush;
	}
	payload_boiler_status returnPayload;

	writeToFile("Network upodate");
	//network.update();
		
	if (ok) {		
		writeToFile("Ok");
		bool got=false;
		while (!got) {
			writeToFile("!got update");
			network.update();
			while (network.available()) {
				writeToFile("network avail");
				got = true;
				network.peek (header);

				if (header.type == 'b') {
					writeToFile("b");
					network.read(header, &returnPayload, sizeof(returnPayload));
					writeToFile("sendGetBoilerPayloadNotInUse");
					usleep(100);
					radioInUse = false;
					return returnPayload;
				}
				writeToFile("This bit");
				network.read(header, &returnPayload, sizeof(returnPayload));
			}
			writeToFile("Before sleep");
			usleep(1000);
		}
	} else {
		writeToFile("Not ok");
		cout << "not ok" << endl;
	}
	writeToFile("sendGetBoilerPayloadNotInUse");
	usleep(100);
	radioInUse = false;
	
	cout << "Return 65535" << endl;

	returnPayload.deviceNum = 65535;
	return returnPayload;
}
