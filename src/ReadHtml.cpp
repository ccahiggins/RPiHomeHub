#include "ReadHtml.hpp"

std::string ReadHtml::readHtml(char const* fileName) {
	std::string line = "";
	std::ifstream file(fileName);
	
	std::ostringstream oss;
	
	while (getline(file, line)) {
		oss << "\n";
		oss << line;
	}
	file.close();
	
	return oss.str();
}