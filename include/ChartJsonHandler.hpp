#ifndef CHARTJSONHANDLER_HPP
#define CHARTJSONHANDLER_HPP

#include <algorithm>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

#include "AuthHandler.hpp"
#include "ChartCreatorJson.hpp"
#include "CivetServer.h"
#include "ReadHtml.hpp"
#include "boost/format.hpp"

class ChartJsonHandler : public CivetHandler {
public:
    bool handleGet(CivetServer *server, struct mg_connection *conn);
};

#endif