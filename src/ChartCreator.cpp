#include "ChartCreator.hpp"

std::string ChartCreator::getChartDays(std::string &days) {

    std::vector<std::vector<std::string>> data = DatabaseController::getChartDataDays(days);
    std::string tempData = getTempGraph(data);

    return tempData;
}

std::string ChartCreator::getChartFromDays(std::string &from, std::string &days) {

    std::vector<std::vector<std::string>> data = DatabaseController::getChartDataFromDays(from, days);
    std::string tempData = getTempGraph(data);

    return tempData;
}

std::string ChartCreator::getChartFromTo(std::string &from, std::string &to) {

    std::vector<std::vector<std::string>> data = DatabaseController::getChartDataFromTo(from, to);
    std::string tempData = getTempGraph(data);

    return tempData;
}

void ChartCreator::writeChartToFile() {

    std::string days = "1";

    std::vector<std::vector<std::string>> data = DatabaseController::getChartDataDays(days);
    std::string tempData = getTempGraph(data);

    std::ofstream myfile;
    myfile.open("chartdata");
    myfile << tempData;
    myfile.close();
}

std::string ChartCreator::getTempGraph(std::vector<std::vector<std::string>> &data) {

    int numGraphs = 1;
    std::string somestuff = "";

    somestuff.append(ReadHtml::readHtml("html/ChartCreator/graph1.html"));
    // somestuff.append("=========");

    std::string tempData = formatGraphData(data);
    somestuff.append(tempData);

    somestuff.append(ReadHtml::readHtml("html/ChartCreator/graph2.html"));

    // Do this to make an example graph
    // If there is no data returned form database
    if (numGraphs < 1) {
        std::string emptyGraph = "";
        emptyGraph.append(ReadHtml::readHtml("html/ChartCreator/emptyGraph.html"));
        return emptyGraph;
    }
    return somestuff;
}

std::string ChartCreator::formatGraphData(std::vector<std::vector<std::string>> &data) {

    std::string sensor1Name = Sensors::getShortName(1);
    std::string sensor2Name = Sensors::getShortName(2);
    std::string sensor3Name = Sensors::getShortName(3);
    std::string sensor4Name = Sensors::getShortName(4);
    std::string sensor5Name = Sensors::getShortName(5);

    std::vector<std::string> sensors;

    std::ostringstream tempData;
    if (data.size() < 1) {
        return tempData.str();
    }

    tempData << "var data";
    tempData << " = google.visualization.arrayToDataTable([\n";

    for (unsigned int i = 0; i < data.size(); i++) {
        if (sensors.empty())
            sensors.push_back(data[i][2]);
        else if (!(find(sensors.begin(), sensors.end(), data[i][2]) != sensors.end()))
            sensors.push_back(data[i][2]);
    }

    std::sort(sensors.begin(), sensors.end());

    // Make titles for graph data
    tempData << "['Date', ";
    if (find(sensors.begin(), sensors.end(), "1") != sensors.end()) {
        tempData << "'";
        tempData << sensor1Name;
        tempData << "', ";
    }
    if (find(sensors.begin(), sensors.end(), "2") != sensors.end()) {
        tempData << "'";
        tempData << sensor2Name;
        tempData << "', ";
    }
    if (find(sensors.begin(), sensors.end(), "3") != sensors.end()) {
        tempData << "'";
        tempData << sensor3Name;
        tempData << "', ";
    }
    if (find(sensors.begin(), sensors.end(), "4") != sensors.end()) {
        tempData << "'";
        tempData << sensor4Name;
        tempData << "', ";
    }
    if (find(sensors.begin(), sensors.end(), "5") != sensors.end()) {
        tempData << "'";
        tempData << sensor5Name;
        tempData << "', ";
    }
    tempData << "'Zero'],\n";

    for (unsigned int i = 0; i < data.size(); i++) {
        // Add date
        tempData << "[new Date(";
        tempData << data[i][0].substr(0, 4); // year
        tempData << ",";

        std::string month = data[i][0].substr(5, 2); // Month
        int value = atoi(month.c_str()) - 1;         // Get month and subtract 1 for graph (Google months are 0-11)

        tempData << value; // month - 1
        tempData << ",";
        tempData << data[i][0].substr(8, 2); // day
        tempData << ",";
        tempData << data[i][0].substr(11, 2); // hour
        tempData << ",";
        tempData << data[i][0].substr(14, 2); // minute
        tempData << "),";

        // Add voltage data and nulls depending on how many sensors there are
        for (unsigned int j = 0; j < sensors.size(); j++) {
            if (!sensors[j].compare(data[i][2])) {
                tempData << data[i][1];
                tempData << ",";
            } else {
                tempData << "null, ";
            }
        }
        if (i == 0 || i == data.size() - 1) {
            tempData << "0],\n";
        } else {
            tempData << "null],\n";
        }
    }
    tempData << "]);\n";

    return tempData.str();
}