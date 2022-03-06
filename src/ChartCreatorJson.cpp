#include "ChartCreatorJson.hpp"


std::string ChartCreatorJson::get_chart_days(std::string &days) {
	
	std::vector<std::vector<std::string>> data = DatabaseController::getChartDataDays(days);
	std::string tempData = get_chart_json(data);
	
	return tempData;
}

std::string ChartCreatorJson::get_chart_from_days(std::string &from, std::string &days) {

	std::vector<std::vector<std::string>> data = DatabaseController::getChartDataFromDays(from, days);
	std::string tempData = get_chart_json(data);
	
	return tempData;
}

std::string ChartCreatorJson::get_chart_from_to(std::string &from, std::string &to) {

	std::vector<std::vector<std::string>> data = DatabaseController::getChartDataFromTo(from, to);
	std::string tempData = get_chart_json(data);
	
	return tempData;
}

std::string ChartCreatorJson::get_chart_json(std::vector<std::vector<std::string>> &data)
{
    // String stream to hold the chart data    
    std::ostringstream tempData;


    // Get names of sensors and map to sensor number
    std::map<unsigned int, std::string> m;
    for (int i=1; i <= 5; i++) {
        m[i] = Sensors::getShortName(i);
    }

    // Get list of sensors that are in the dataset
    std::vector<int> sensors;
	for (unsigned int i=0; i < data.size(); i++) {
		if (sensors.empty()) {
			sensors.push_back(std::stoi(data[i][2]));
		} else if (!(find(sensors.begin(), sensors.end(), std::stoi(data[i][2])) != sensors.end())) {
			sensors.push_back(std::stoi(data[i][2]));
		}
	}
	std::sort(sensors.begin(), sensors.end());

    // Generate the column data with sensor names
    tempData << "{\"cols\":[{\"id\":0,\"label\":\"Date\",\"type\":\"date\"},";
	for (unsigned int i = 0; i < sensors.size(); i++) {
        tempData << "{\"id\":" << (i + 1) << ",\"label\":\"" << m[sensors[i]] << "\",\"type\":\"number\"}";
        if (i < sensors.size() -1 ) {
            tempData << ",";
        }
	}

    // Generate the row data with date/temp for each sensor
    tempData << "],\"rows\":[";    
    for (unsigned int i=0; i < data.size(); i++) {
        tempData << "{\"c\":[";
        for (unsigned int j = 0; j < sensors.size(); j++) {
            if (j == 0) {
                tempData << "{\"v\":\"Date(";
                tempData << data[i][0].substr (0, 4);
                tempData << ",";
                tempData << std::stoi(data[i][0].substr (5, 2)) - 1;
                tempData << ",";
                tempData << data[i][0].substr (8, 2);	  // day
                tempData << ",";
                tempData << data[i][0].substr (11, 2);	  // hour
                tempData << ",";
                tempData << data[i][0].substr (14, 2);	  // minute
                tempData << ")";

                tempData << "\",\"f\":null},";
            }
            
            // Populate the row if the date is for this sensor, otherwise set it null
            if (sensors[j] == std::stoi(data[i][2])) {
                tempData << "{\"v\":" << data[i][1] << ",\"f\":null}";
            } else {
                tempData << "{\"v\":null,\"f\":null}";
            }

            if (j < sensors.size() -1 ) {
                tempData << ",";
            }

        }
        tempData << "]}";
        if (i < data.size() -1 ) {
            tempData << ",";
        }
    }
    tempData << "]}";

    return tempData.str();
}