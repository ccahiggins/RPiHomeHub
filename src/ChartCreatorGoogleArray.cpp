#include "ChartCreatorGoogleArray.hpp"

std::string ChartCreatorGoogleArray::get_chart_days(std::string &days) {
	
	std::vector<std::vector<std::string>> data = DatabaseController::getChartDataDays(days);
	std::string tempData = get_chart_data(data);
	
	return tempData;
}

std::string ChartCreatorGoogleArray::get_chart_from_days(std::string &from, std::string &days) {

	std::vector<std::vector<std::string>> data = DatabaseController::getChartDataFromDays(from, days);
	std::string tempData = get_chart_data(data);
	
	return tempData;
}

std::string ChartCreatorGoogleArray::get_chart_from_to(std::string &from, std::string &to) {

	std::vector<std::vector<std::string>> data = DatabaseController::getChartDataFromTo(from, to);
	std::string tempData = get_chart_data(data);
	
	return tempData;
}

std::string ChartCreatorGoogleArray::get_chart_data(std::vector<std::vector<std::string>> &data)
{
    // String stream to hold the chart data    
    std::ostringstream tempData;
    
    std::map<unsigned int, std::string> m;
    for (int i=1; i <= 5; i++) {
        m[i] = Sensors::getShortName(i);
    }
	
	std::vector<int> sensors;
	
	if (data.size() < 1) {
		return tempData.str();
	}

	for (unsigned int i=0; i < data.size(); i++)
	{
		if (sensors.empty())
			sensors.push_back(std::stoi(data[i][2]));
		else if (!(find(sensors.begin(), sensors.end(), std::stoi(data[i][2])) != sensors.end()))
			sensors.push_back(std::stoi(data[i][2]));
	}

	std::sort(sensors.begin(), sensors.end());

	//Make titles for graph data
	tempData << "[[\"Date\",";

	for (unsigned int i = 0; i < sensors.size(); i++) {
        tempData << "\"" << m[sensors[i]] << "\",";
	}

	tempData << "\"Zero\"],";
	
	
	for (unsigned int i = 0; i < data.size(); i++) {
		//Add date
		tempData << "[new Date(";
		tempData << data[i][0].substr (0, 4); 	  // year
		tempData << ",";
		
		std::string month = data[i][0].substr (5, 2);  // Month
		int value = atoi(month.c_str()) - 1;	  // Get month and subtract 1 for graph (Google months are 0-11)
		
		tempData << value; 						  // month - 1
		tempData << ",";
		tempData << data[i][0].substr (8, 2);	  // day
		tempData << ",";
		tempData << data[i][0].substr (11, 2);	  // hour
		tempData << ",";
		tempData << data[i][0].substr (14, 2);	  // minute
		tempData << "),";
		
		//Add voltage data and nulls depending on how many sensors there are
		for (unsigned int j = 0; j < sensors.size(); j++) {
			if (sensors[j] == std::stoi(data[i][2])) {
				tempData << data[i][1];
				tempData << ",";
			}
			else {
				tempData << "null,";
			}
		}
		if (i == 0) {
				tempData << "0],";
		} else if ( i == data.size() - 1) {
            tempData << "0]";
        } else {
		    tempData << "null],";
		}
	}
	tempData << "]";
	
    return tempData.str();
}