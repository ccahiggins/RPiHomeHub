#ifndef CHARTHANDLER_HPP
#define CHARTHANDLER_HPP

#include <istream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <ctime>

#include "CivetServer.h"
#include "ChartCreator.hpp"
#include "ReadHtml.hpp"


class ChartHandler: public CivetHandler
{
public:

	bool handleGet(CivetServer *server, struct mg_connection *conn);

};

#endif