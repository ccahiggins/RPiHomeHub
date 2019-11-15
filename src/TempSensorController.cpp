#include "TempSensorController.hpp"

TempSensorController::TempSensorController(RadioController& radio_) : radio(radio_) {
	std::cout << "TC" << std::endl;
}

sqlite3 *db = NULL;
const char *dbPath = "db/sqlTemplog.db";
sqlite3_stmt *stmt = NULL;

void TempSensorController::checkSensors() {
	
	RadioController::payload_temp payload = radio.getTempPayload();
	
	if (payload.deviceNum != 65535) {
		if (payload.deviceNum > 5) {
			std::cout << "Debug-> ID: " << payload.deviceNum << "  ==  " << std::flush;
		} else {
			saveTempData(payload.deviceNum, payload.temp, payload.voltage);
			system("./push");
			printSensorData(payload.deviceNum, payload.temp, payload.voltage);
			notify(payload.deviceNum, payload.temp);
			last_volts[payload.deviceNum] = payload.voltage;
		}
	}
}

void TempSensorController::saveTempData(uint16_t deviceNum, float temp, uint16_t voltage) {
	
	//std::cout << "S:OD=" << std::flush;
	int rc = sqlite3_open(dbPath, &db);
	// If rc is not 0, there was an error
	if(rc) {
		std::cout << "S:ERRDB: " << sqlite3_errmsg(db) << "  ==  " << std::flush;
	}
	
	std::stringstream ss;
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

	//std::cout << "S:CD/" << std::flush;
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
	//std::cout << " " << rc << " " << std::flush;
}

std::vector<int> TempSensorController::lowBattery() {
	float low = 3.0;
	std::vector<int> lowBatts;

	for (std::pair<int, float> element : last_volts) {
		if (element.second <= low) {
			lowBatts.push_back(element.first);
		}
	}

	return lowBatts;
}

void TempSensorController::printSensorData(uint16_t deviceNum, float temp, uint16_t voltage) {
	
	std::stringstream s; 
	s << std::fixed << std::setprecision(1) << temp;

	std::cout << "\033[31mS" << deviceNum << ":\033[36m" << s.str() << "°C,\033[32m";

	printCurrentTime();
	std::cout << ",\033[35m";
	
	float battVolts = voltage * 0.001;
	
	if (battVolts > 0) {	
		std::cout << battVolts << "V\033[0m";
	}
	std::cout << " = " << std::flush;
}

void TempSensorController::printCurrentTime() {

	time_t t = time(0);   // get time now
	struct tm* now = localtime( & t );
	int hour = now->tm_hour;
	int minute = now->tm_min;

	if (hour < 10) {
		std::cout << "0";
	}
	std::cout << hour << ":";
	
	if (minute < 10) {
		std::cout << "0";
	}
	std::cout << minute;
}
