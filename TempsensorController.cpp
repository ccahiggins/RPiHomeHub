#include "TempsensorController.h"

TempsensorController::TempsensorController(RadioController* radio_) {
	std::cout << "TC" << std::endl;
	radio = radio_;
}

float lastVoltage1 = 10;
float lastVoltage2 = 10;
float lastVoltage3 = 10;
float lastVoltage4 = 10;

sqlite3 *db = NULL;
const char *dbPath = "db/sqlTemplog.db";
sqlite3_stmt *stmt = NULL;

void TempsensorController::checkSensors() {
	using namespace std;
	
	RadioController::payload_temp payload = radio->getTempPayload();
	
	if (payload.deviceNum != 65535) {
		if (payload.deviceNum > 5) {
			cout << "Debug-> ID: " << payload.deviceNum << "  ==  " << flush;
		} else {
			saveTempData(payload.deviceNum, payload.temp, payload.voltage);
			printSensorData(payload.deviceNum, payload.temp, payload.voltage);
		}
	}
}

void TempsensorController::saveTempData(uint16_t deviceNum, float temp, uint16_t voltage) {
	
	using namespace std;

	cout << "S:OD=" << flush;
	int rc = sqlite3_open(dbPath, &db);
	// If rc is not 0, there was an error
	if(rc) {
		cout << "S:ERRDB: " << sqlite3_errmsg(db) << "  ==  " << flush;
	}
	
	stringstream ss;
	ss << "INSERT INTO temps( id, timestamp, temp, voltage) ";
	ss << "VALUES(?,datetime('now', 'localtime'), ?, ?);";
	 
	auto x = ss.str();
	const char *sql = x.c_str();
	
	if(!sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL)== SQLITE_OK) {
		fprintf(stderr, "S:XXX: %s\n", sqlite3_errmsg(db));
	}

	sqlite3_bind_int(stmt, 1, deviceNum);
	sqlite3_bind_double(stmt, 2, temp);
	sqlite3_bind_double(stmt, 3,  (voltage * 0.001));
	sqlite3_step(stmt);  // Run SQL INSERT
	sqlite3_reset(stmt); // Clear statement handle for next use
	sqlite3_finalize(stmt);
	cout << "S:CD/" << flush;
	rc = sqlite3_close(db);
	int i = 0;
	while(rc != SQLITE_OK) {
		printf("it is busy\n");
		i++;
		if ( i > 10 ) {
			break;
		}
		sleep(1);
		rc=sqlite3_close(db);
	}
	
	ChartCreator chartCreator;
	chartCreator.writeChartToFile();
	cout << " " << rc << " " << flush;
}

std::vector<int> TempsensorController::lowBattery() {
	float low = 3.2;
	std::vector<int> lowBatts;
	
	if (lastVoltage1 < low) {
		lowBatts.push_back(1);
	}
	if (lastVoltage2 < low) {
		lowBatts.push_back(2);
	}
	if (lastVoltage3 < low) {
		lowBatts.push_back(3);
	}
	if (lastVoltage4 < low) {
		lowBatts.push_back(4);
	}

	return lowBatts;
}

void TempsensorController::printSensorData(uint16_t deviceNum, float temp, uint16_t voltage) {
	using namespace std;
	
	stringstream s; 
	s << fixed <<  setprecision(1) << temp;

	cout << "S" << deviceNum 
	<< ":" << s.str() << "°C,";

	printCurrentTime();
	cout << ",";
	
	float battVolts = voltage * 0.001;
	
	if (battVolts > 0) {
		switch(deviceNum) {
			case 1 : lastVoltage1 = battVolts;
					 break;
			case 2 : lastVoltage2 = battVolts;
					 break;
			case 3 : lastVoltage3 = battVolts;
					 break;
			case 4 : lastVoltage4 = battVolts;
					 break;
		}
		
		cout << battVolts << "V" << " = " << flush;
	} else {
		cout << " = " << flush;
	}
}

void TempsensorController::printCurrentTime() {
	using namespace std;
	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
	int hour=now->tm_hour;
	if (hour < 10) {
		cout << "0" << hour << ":" ;
	} else {
		cout << hour << ":";
	}
	int minute=now->tm_min;
	if (minute < 10) {
		cout << "0" << minute ;
	} else{
		cout << minute;
	}
}
