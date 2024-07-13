#include "VoltageHandler.hpp"


bool VoltageHandler::handleGet(CivetServer *server, struct mg_connection *conn) {

	mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	AuthHandler auth = AuthHandler();
	std::string role = "admin";
    if (auth.authorised(conn, role)) {
		std::string param;
		std::string voltageData = "";
		if (CivetServer::getParam(conn, "days", param)) {
			int days = atoi(param.c_str()); 
			voltageData=printVolts(days);
		} else {
			voltageData = printVolts();
		}
		
		std::string html = boost::str(boost::format(ReadHtml::readHtml("html/VoltageHandler/html.html")) % voltageData);
		mg_printf(conn, html.c_str());
	} else {
		const struct mg_request_info *req_info = mg_get_request_info(conn);
		std::string uri = std::string(req_info->local_uri);
		std::string html = ReadHtml::readHtml("html/auth/pleaselogin.html");
		std::string s = boost::str(boost::format(html) % uri  );
		mg_printf(conn, s.c_str());
	}
	
	return true;
}

std::string VoltageHandler::formatVolts(std::vector<std::vector<std::string>> &data) {

	//String for the name the title for each sensor
	std::string sensor1Name = Sensors::getName(1);
	std::string sensor2Name = Sensors::getName(2);
	std::string sensor3Name = Sensors::getName(3);
	std::string sensor4Name = Sensors::getName(4);
	std::string sensor5Name = Sensors::getName(5);
	

	std::string voltsData="";		// String which will contain temp data for graph
	if (data.size() < 1) {		// Return empty string if no values in vector: data
		return voltsData;
	}
	
	std::vector<std::string> sensors;		// Vector of strings for each ID that exists in vector: data 
   
	for (unsigned int i=0; i<data.size(); i++) {	// Get each unique ID from vector: data
		if (sensors.empty())
			sensors.push_back(data[i][2]);
		else if (!(find(sensors.begin(), sensors.end(), data[i][2]) != sensors.end()))
			sensors.push_back(data[i][2]);
	}
   
   std::sort(sensors.begin(), sensors.end());
   
   
	//voltsData.append("var data");
	//voltsData.append("   = google.visualization.arrayToDataTable([\n");

	
	
	//Make titles for graph data
	voltsData.append("['Date', ");
	if (find(sensors.begin(), sensors.end(), "1") != sensors.end())			//Add title if data for sensor exists in Vector: data
	{
		voltsData.append("'");
		voltsData.append(sensor1Name);
		voltsData.append("', ");
	}
	if (find(sensors.begin(), sensors.end(), "2") != sensors.end())			//Add title if data for sensor exists in Vector: data
	{
		voltsData.append("'");
		voltsData.append(sensor2Name);
		voltsData.append("', ");
	}
	if (find(sensors.begin(), sensors.end(), "3") != sensors.end())			//Add title if data for sensor exists in Vector: data
	{
		voltsData.append("'");
		voltsData.append(sensor3Name);
		voltsData.append("', ");
	}
	if (find(sensors.begin(), sensors.end(), "4") != sensors.end())			//Add title if data for sensor exists in Vector: data
	{
		voltsData.append("'");
		voltsData.append(sensor4Name);
		voltsData.append("', ");
	}
	if (find(sensors.begin(), sensors.end(), "5") != sensors.end())			//Add title if data for sensor exists in Vector: data
	{
		voltsData.append("'");
		voltsData.append(sensor5Name);
		voltsData.append("', ");
	}
	voltsData.append("],\n");

	
	//Record last voltage to skip if voltage the same as last
	std::string lastV1 = "";
	std::string lastV2 = "";
	std::string lastV3 = "";
	std::string lastV4 = "";
	std::string lastV5 = "";
	
	for (unsigned int i = 0; i < data.size(); i++) {
		bool skipData = false;					//Skip if voltage unchanged bool
		std::string idString = data[i][2];			//Get ID string from Vector: data
		
		if (!idString.compare("1")) {
			if (!lastV1.compare(data[i][1])) {	//Compare last voltage to current
				skipData=true;
			}
			else {
				lastV1=data[i][1];
			}
		}
		else if (!idString.compare("2")) {

			if (!lastV2.compare(data[i][1])) {	//Compare last voltage to current
				skipData=true;
			}
			else {
				lastV2=data[i][1];
			}
		}
		else if (!idString.compare("3")) {
			if (!lastV3.compare(data[i][1])) {	//Compare last voltage to current
				skipData=true;
			}
			else {
				lastV3=data[i][1];
			}
		}
		else if (!idString.compare("4")) {
			if (!lastV4.compare(data[i][1])) {	//Compare last voltage to current
				skipData=true;
			}
			else {
				lastV4=data[i][1];
			}
		} else if (!idString.compare("5")) {
			if (!lastV5.compare(data[i][1])) {	//Compare last voltage to current
				skipData=true;
			}
			else {
				lastV5=data[i][1];
			}
		}
		
		if (!skipData) {					//Skip if voltage unchanged otherwise add data
			//Add date
			voltsData.append("[new Date(");
			voltsData.append(data[i][0].substr (0, 4)); 	// year
			voltsData.append(",");
			
			std::string month=data[i][0].substr (5, 2);			// Month
			int value = atoi(month.c_str()) - 1;    		// Get month and subtract 1 for graph (Google months are 0-11)
			
			std::string newMonth;          						// string which will contain the changed month
			std::ostringstream convert;   						// stream used for the conversion
		
			convert << value;      							// insert the textual representation of 'month-1' in the characters in the stream

			newMonth = convert.str(); 
				
			voltsData.append(newMonth); 					// month - 1
			voltsData.append(",");
			voltsData.append(data[i][0].substr (8, 2));		// day
			voltsData.append(",");
			voltsData.append(data[i][0].substr (11, 2));	// hour
			voltsData.append(",");
			voltsData.append(data[i][0].substr (14, 2));	// minute
			voltsData.append("),");
			
			//Add voltage data and nulls depending on how many sensors there are
			for (unsigned int j=0; j<sensors.size(); j++)
			{
				if (!sensors[j].compare(data[i][2]))
				{
					voltsData.append(data[i][1]);
					voltsData.append(",");
				}
				else 
					voltsData.append("null, ");
			}
			voltsData.append("],\n");
		}
	}
	
	return voltsData;
}

std::string VoltageHandler::printVolts(int days) {


	std::vector<std::vector<std::string>> data = DatabaseController::getSensorVoltage(days);
	std::string voltsData = formatVolts(data);
	
	return voltsData;
}

std::string VoltageHandler::printVolts() {

	std::vector<std::vector<std::string>> data = DatabaseController::getSensorVoltage();
	std::string voltsData = formatVolts(data);
	
	return voltsData;
}
