#include "RadioController.hpp"

// Header types:
// 't' Temperature sensor
// 'd' Temperature sensor test
// 'p' Boiler

const int COUNT = 5;
volatile bool radioInUse;

RadioController::RadioController() : radio(RPI_V2_GPIO_P1_22, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ),network(radio) {

	std::cout << "RC" << std::endl;
    setupRadio();
	std::cout << "RC" << std::endl;
}

void RadioController::writeToFile(std::string text) {

   /* std::ofstream myfile;
    myfile.open ("logs/inUseThingy.txt", std::ios::app);
    myfile << text << std::endl;
    myfile.close();*/
}

void RadioController::setupRadio() {

	radio.begin();
	const uint16_t node = 00;

	radio.setChannel(90);
	radio.setDataRate(RF24_250KBPS);
	radio.setAutoAck(true);
	radio.printDetails();
	delay(5);
	network.begin(node);

}

bool RadioController::sendPayload(RF24NetworkHeader header, const void * payload, uint16_t size) {

	while (radioInUse) {
		writeToFile("Sleeping1");
		std::cout << "|S1|" << std::flush;
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
	usleep(1000);
	radioInUse = false;
	
	return ok;
}

/* RadioController::payload_boiler_status RadioController::getBoilerPayload() {
	//using namespace std;

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

	RF24NetworkHeader header;
	payload_temp payload;

	while (radioInUse) {
		writeToFile("Sleeping3");
		std::cout << "|S3|" << std::flush;
		usleep(5000);
	}
	radioInUse = true;
	writeToFile("getTempPayloadInUse");
	
	bool yeah = false;
	
	network.update();
	usleep(5000);
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
	usleep(1000);
	radioInUse = false;

	if (yeah) {
		return payload;
	}
	
	payload.deviceNum = 65535;
	return payload;
}

RadioController::payload_boiler_status RadioController::sendGetBoilerPayload(RF24NetworkHeader header, const void * payload, uint16_t size) {
	
	while (radioInUse) {
		writeToFile("Sleeping4");
		std::cout << "|S4|" << std::flush;
		usleep(5000);
	}
	radioInUse = true;
	writeToFile("sendGetBoilerPayloadInUse");

	bool ok = false;
	int counter = 0;
	std::cout << "X|" << std::flush;
	while (!ok && counter < COUNT) {
		writeToFile("while counter thing");
		ok = network.write(header,payload,size);
		counter++;
		std::cout << "W|" << std::flush;
		usleep(5000);
	}
	payload_boiler_status returnPayload;

	writeToFile("Network upodate");
	//network.update();
		
	counter = 0;
	if (ok) {		
		writeToFile("Ok");
		bool got=false;
		while (!got && counter < 50) {
			writeToFile("!got update");
			network.update();
			usleep(5000);
			while (network.available()) {
				writeToFile("network avail");
				got = true;
				network.peek (header);

				if (header.type == 'b') {
					writeToFile("b");
					network.read(header, &returnPayload, sizeof(returnPayload));
					writeToFile("sendGetBoilerPayloadNotInUse");
					usleep(5000);
					radioInUse = false;
					return returnPayload;
				}
				writeToFile("This bit");
				network.read(header, &returnPayload, sizeof(returnPayload));
				std::cout << "Header:" << header.type << std::endl;
			}
			counter++;
			writeToFile("Before sleep");
			usleep(5000);
		}
	} else {
		writeToFile("Not ok");
		std::cout << "not ok" << std::endl;
	}
	writeToFile("sendGetBoilerPayloadNotInUse");
	usleep(1000);
	radioInUse = false;
	
	std::cout << "Return 65535" << std::endl;

	returnPayload.deviceNum = 65535;
	return returnPayload;
}
