#ifndef CHARTHANDLER_HPP
#define CHARTHANDLER_HPP

#include <algorithm>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

#include "AuthHandler.hpp"
#include "ChartCreator.hpp"
#include "ChartCreatorDygraphs.hpp"
#include "ChartCreatorGoogleArray.hpp"
#include "ChartCreatorJson.hpp"
#include "ChartCreatorNvd3.hpp"
#include "CivetServer.h"
#include "ReadHtml.hpp"
#include "boost/format.hpp"

class ChartHandler : public CivetHandler {
public:
    bool handleGet(CivetServer *server, struct mg_connection *conn);
};

#endif