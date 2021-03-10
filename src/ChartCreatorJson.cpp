#include "ChartCreatorJson.hpp"


std::string ChartCreatorJson::getChartDays(std::string &days) {
	
	std::vector<std::vector<std::string>> data = DatabaseController::getChartDataDays(days);
	std::string tempData = getTempGraph(data);
	
	return tempData;
}

std::string ChartCreatorJson::getChartFromDays(std::string &from, std::string &days) {

	std::vector<std::vector<std::string>> data = DatabaseController::getChartDataFromDays(from, days);
	std::string tempData = getTempGraph(data);
	
	return tempData;
}

std::string ChartCreatorJson::getChartFromTo(std::string &from, std::string &to) {

	std::vector<std::vector<std::string>> data = DatabaseController::getChartDataFromTo(from, to);
	std::string tempData = getTempGraph(data);
	
	return tempData;
}


std::string ChartCreatorJson::getTempGraph(std::vector<std::vector<std::string>> &data)
{
	using namespace std;
	int numGraphs = 1;
	string somestuff = "";

	somestuff.append(ReadHtml::readHtml("html/ChartCreatorJson/graph1.html"));
	//somestuff.append("=========");

	string tempData = formatGraphData(data);
	somestuff.append(tempData);

	somestuff.append(ReadHtml::readHtml("html/ChartCreatorJson/graph2.html"));


	//Do this to make an example graph
	//If there is no data returned form database
	if (numGraphs < 1) {
		string emptyGraph = "";
		emptyGraph.append(ReadHtml::readHtml("html/ChartCreatorJson/emptyGraph.html"));
		return emptyGraph;
	}
	return somestuff;

}

std::string ChartCreatorJson::formatGraphData(std::vector<std::vector<std::string>> &data)
{
	using namespace std;
	
	vector<vector<string>> d;
	
	vector<string> q;
	vector<string> w;
	vector<string> e;
	vector<string> r;
	
	d.push_back(q);
	d.push_back(w);
	d.push_back(e);
	d.push_back(r);
	
	for (unsigned int i = 0; i < data.size(); i++) {
		//Add date
		
		/* tempData << data[i][0].substr (0, 4); 	  // year
		tempData << ",";
		
		string month = data[i][0].substr (5, 2);  // Month
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
			if (!sensors[j].compare(data[i][2])) {
				tempData << data[i][1];
				tempData << ",";
			}
			else {
				tempData << "null, ";
			}
		}
		tempData << "],\n"; */
		
		if (data[i][2].compare("1") == 0)  {
			string line = "{\'t\':\'";
			line.append(data[i][0]);
			line.append("\',\'y\':");
			line.append(data[i][1]);
			line.append("},");
			
			d[0].push_back(line);
		} else if (data[i][2].compare("2") == 0)  {
			string line = "{\'t\':\'";
			line.append(data[i][0]);
			line.append("\',\'y\':");
			line.append(data[i][1]);
			line.append("},");
			
			d[1].push_back(line);
		} else if (data[i][2].compare("3") == 0)  {
			string line = "{\'t\':\'";
			line.append(data[i][0]);
			line.append("\',\'y\':");
			line.append(data[i][1]);
			line.append("},");
			
			d[2].push_back(line);
		} else if (data[i][2].compare("4") == 0)  {
			string line = "{\'t\':\'";
			line.append(data[i][0]);
			line.append("\',\'y\':");
			line.append(data[i][1]);
			line.append("},");
			
			d[3].push_back(line);
		}
	}
	
	string datasets = "[";
	for (unsigned int i = 0; i < d.size(); i++) {
		string a = "{data:[";
		for (unsigned int j = 0; j < d[i].size(); j++) {
			a.append(d[i][j]);
		}
		a.append("]},");
		datasets.append(a);
	}
	datasets.append("];");
	
	return datasets;
}	
















/* std::string ChartCreatorJson::formatGraphData(std::vector<std::vector<std::string> > &data)
{
	using namespace std;
	
	string sensor1Name = "Bed";
	string sensor2Name = "Living";
	string sensor3Name = "Kids";
	string sensor4Name = "Outside";
	
	vector<string> sensors;
	
	ostringstream tempData;
	if (data.size() < 1) {
		return tempData.str();
	}

	tempData << "var data";
	tempData << " = google.visualization.arrayToDataTable([\n";

	for (unsigned int i=0; i < data.size(); i++)
	{
		if (sensors.empty())
			sensors.push_back(data[i][2]);
		else if (!(find(sensors.begin(), sensors.end(), data[i][2]) != sensors.end()))
			sensors.push_back(data[i][2]);
	}

	sort(sensors.begin(), sensors.end());

	//Make titles for graph data
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
	tempData << "],\n";
	
	
	for (unsigned int i = 0; i < data.size(); i++) {
		//Add date
		tempData << "[new Date(";
		tempData << data[i][0].substr (0, 4); 	  // year
		tempData << ",";
		
		string month = data[i][0].substr (5, 2);  // Month
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
			if (!sensors[j].compare(data[i][2])) {
				tempData << data[i][1];
				tempData << ",";
			}
			else {
				tempData << "null, ";
			}
		}
		tempData << "],\n";
	}
	tempData << "]);\n";
	
	return tempData.str();
}	 */