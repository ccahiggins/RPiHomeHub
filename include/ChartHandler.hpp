#ifndef CHARTHANDLER_HPP
#define CHARTHANDLER_HPP

#include <istream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "CivetServer.h"
#include "ChartCreator.hpp"
#include "ReadHtml.hpp"
#include "AuthHandler.hpp"
#include "boost/format.hpp"


class ChartHandler: public CivetHandler
{
public:

	bool handleGet(CivetServer *server, struct mg_connection *conn);

};

#endif