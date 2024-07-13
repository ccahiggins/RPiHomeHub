#include "ChartCreatorNvd3.hpp"

std::string ChartCreatorNvd3::get_chart_days(std::string &days) {

    std::vector<std::vector<std::string>> data = DatabaseController::get_chart_data_days_epochtime(days);
    std::string tempData = get_chart_data(data);

    return tempData;
}

std::string ChartCreatorNvd3::get_chart_from_days(std::string &from, std::string &days) {

    std::vector<std::vector<std::string>> data = DatabaseController::get_chart_data_from_days_epochtime(from, days);
    std::string tempData = get_chart_data(data);

    return tempData;
}

std::string ChartCreatorNvd3::get_chart_from_to(std::string &from, std::string &to) {

    std::vector<std::vector<std::string>> data = DatabaseController::get_chart_data_from_to_epochtime(from, to);
    std::string tempData = get_chart_data(data);

    return tempData;
}

std::string ChartCreatorNvd3::get_chart_data(std::vector<std::vector<std::string>> &data) {

    // String stream to hold the chart data
    std::ostringstream tempData;

    // Get names of sensors and map to sensor number
    std::map<unsigned int, std::string> m;
    for (int i = 1; i <= 5; i++) {
        m[i] = Sensors::getShortName(i);
    }

    std::map<unsigned int, std::vector<std::vector<std::string>>> woo;

    // Get list of sensors that are in the dataset
    std::vector<int> sensors;
    for (unsigned int i = 0; i < data.size(); i++) {
        if (sensors.empty()) {
            sensors.push_back(std::stoi(data[i][2]));
        } else if (!(find(sensors.begin(), sensors.end(), std::stoi(data[i][2])) != sensors.end())) {
            sensors.push_back(std::stoi(data[i][2]));
        }
        woo[std::stoi(data[i][2])].push_back(data[i]);
    }
    std::cout << std::endl;
    std::sort(sensors.begin(), sensors.end());

    tempData << "[";
    for (auto x = woo.begin(); x != woo.end(); ++x) {
        tempData << "{\"values\":[";
        for (unsigned int i = 0; i < x->second.size(); i++) {
            tempData << "{\"x\":" << x->second[i][0] << ",\"y\":" << x->second[i][1] << "}";
            if (i < x->second.size()) {
                tempData << ",";
            }
        }
        tempData << "],";
        tempData << "\"key\": \"" << m[x->first] << "\",";
        tempData << "\"color\": \"#f9b800\"";
        tempData << "}";
        if (std::next(x) != woo.end()) {
            tempData << ",";
        }
    }
    tempData << "]";

    return tempData.str();
}