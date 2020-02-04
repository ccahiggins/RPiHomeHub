#include "Boiler.hpp"

Boiler::Boiler(RadioController& radio_) : radio(radio_) {
	std::cout << "BC" << std::endl;
}

bool Boiler::switchPin(int pin, int state, int duration, unsigned char type) {
	RF24NetworkHeader header(03, type);
	RadioController::payload_boiler_control payload;
	//payload.pinNumber = pin;
	payload.pinState=state;
	payload.duration=duration;
	// payload.option = PIN_SET;

	bool ok = radio.sendPayload(header,&payload,sizeof(payload));
	
	return ok;
}

/*
Message types
=============

b = boiler status
h = water control
w = heating control
*/

/* int Boiler::getStatus() {
	//using namespace std;
 	RF24NetworkHeader header(03, 'p');
	RadioController::payload_boiler payload;
	payload.pinNumber = pin;
	payload.pinState = 2;
	payload.duration = 0;

	RadioController::payload_boiler_status returnPayload = radio->sendGetBoilerPayload(header,&payload,sizeof(payload));	
	if (returnPayload.pinNumber != ERRORPAYLOAD) {
		if (returnPayload.duration > 0) {
			return returnPayload.duration;
		}
		if (returnPayload.pinState == OFF){
			return OFF;
		}
		if (returnPayload.pinState == ON){
			return SENDON;
		}
	}
	return ERROR;
} */

bool Boiler::TurnWaterOn() {
	bool ok = switchPin(WATERPIN, PINON, PINNODURATION, 'w');
	return ok;
}

bool Boiler::TurnWaterOn(int duration) {
	bool ok = switchPin(WATERPIN, PINON, duration, 'w');
	return ok;
}

bool Boiler::TurnWaterOff() {
	bool ok = switchPin(WATERPIN, PINOFF, PINNODURATION, 'w');
	return ok;
}

bool Boiler::TurnHeatingOn() {
	bool ok = switchPin(HEATINGPIN, PINON, PINNODURATION, 'h');
	return ok;
}

bool Boiler::TurnHeatingOn(int duration) {
	bool ok = switchPin(HEATINGPIN, PINON, duration, 'h');
	return ok;
}

bool Boiler::TurnHeatingOff() {
	bool ok = switchPin(HEATINGPIN, PINOFF, PINNODURATION, 'h');
	return ok;
}

Boiler::boiler_status Boiler::getBoilerStatus() {

 	RF24NetworkHeader header(03, 'b');
	RadioController::payload_boiler_status payload;
	payload.deviceNum = 0;
	payload.heatingState = 0;
	payload.heatingDuration = 0;
	payload.waterState = 0;
    payload.waterDuration = 0;
	
	Boiler::boiler_status boilerStatus;
	
	RadioController::payload_boiler_status returnPayload = radio.sendGetBoilerPayload(header,&payload,sizeof(payload));	
	if (returnPayload.deviceNum != ERRORPAYLOAD) {
		if (returnPayload.heatingDuration > 0) {
			boilerStatus.heatingStatus = returnPayload.heatingDuration;
		}
		else if (returnPayload.heatingState == OFF) {
			boilerStatus.heatingStatus = returnPayload.heatingState;
		}
		else if (returnPayload.heatingState == ON) {
			boilerStatus.heatingStatus = SENDON;
		}
		
		if (returnPayload.waterDuration > 0) {
			boilerStatus.waterStatus = returnPayload.waterDuration;
		}
		else if (returnPayload.waterState == OFF) {
			boilerStatus.waterStatus = returnPayload.waterState;
		}
		else if (returnPayload.waterState == ON) {
			boilerStatus.waterStatus = SENDON;
		}
	} else {
		boilerStatus.heatingStatus = ERROR;
		boilerStatus.waterStatus = ERROR;
	}
	return boilerStatus;
}

/* int Boiler::WaterStatus() {
	int ok = getStatus(WATERPIN);
	return ok;
}

int Boiler::HeatingStatus() {
	int ok = getStatus(HEATINGPIN);
	return ok;
} */
	
