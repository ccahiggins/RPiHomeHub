#include "ChartCreatorDygraphs.hpp"

std::string ChartCreatorDygraphs::get_chart_days(std::string &days) {

    std::vector<std::vector<std::string>> data = DatabaseController::get_chart_data_days_epochtime(days);
    std::string tempData = get_chart_data(data);

    return tempData;
}

std::string ChartCreatorDygraphs::get_chart_from_days(std::string &from, std::string &days) {

    std::vector<std::vector<std::string>> data = DatabaseController::get_chart_data_from_days_epochtime(from, days);
    std::string tempData = get_chart_data(data);

    return tempData;
}

std::string ChartCreatorDygraphs::get_chart_from_to(std::string &from, std::string &to) {

    std::vector<std::vector<std::string>> data = DatabaseController::get_chart_data_from_to_epochtime(from, to);
    std::string tempData = get_chart_data(data);

    return tempData;
}

std::string ChartCreatorDygraphs::get_chart_data(std::vector<std::vector<std::string>> &data) {

    // String stream to hold the chart data
    std::ostringstream tempData;

    // Get names of sensors and map to sensor number
    std::map<unsigned int, std::string> m;
    for (int i = 1; i <= 5; i++) {
        m[i] = Sensors::getShortName(i);
    }

    // Get list of sensors that are in the dataset
    std::vector<int> sensors;
    for (unsigned int i = 0; i < data.size(); i++) {
        if (sensors.empty()) {
            sensors.push_back(std::stoi(data[i][2]));
        } else if (!(find(sensors.begin(), sensors.end(), std::stoi(data[i][2])) != sensors.end())) {
            sensors.push_back(std::stoi(data[i][2]));
        }
    }
    std::sort(sensors.begin(), sensors.end());

    // Generate the column data with sensor names
    tempData << "Date,";
    for (unsigned int i = 0; i < sensors.size(); i++) {
        tempData << m[sensors[i]];
        if (i < sensors.size() - 1) {
            tempData << ",";
        }
    }
    tempData << std::endl;
    // Generate the row data with date/temp for each sensor
    for (unsigned int i = 0; i < data.size(); i++) {
        for (unsigned int j = 0; j < sensors.size(); j++) {
            if (j == 0) {
                tempData << data[i][0]; // hour
                tempData << ",";
            }

            // Populate the row if the date is for this sensor, otherwise set it null
            if (sensors[j] == std::stoi(data[i][2])) {
                tempData << data[i][1];
            } else {
                tempData << "null";
            }

            if (j < sensors.size() - 1) {
                tempData << ",";
            }
        }
        tempData << std::endl;
    }

    return tempData.str();
}