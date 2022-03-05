#ifndef BOILERHANDLER_HPP
#define BOILERHANDLER_HPP

#include "AuthHandler.hpp"
#include "Boiler.hpp"
#include "CivetServer.h"
#include "ReadHtml.hpp"
#include "boost/format.hpp"

#include <string>

class BoilerHandler : public CivetHandler {

public:
    BoilerHandler(Boiler &boiler_);

    bool handleGet(CivetServer *server, struct mg_connection *conn);

private:
    Boiler &boiler;
};

#endif