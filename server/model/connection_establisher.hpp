#pragma once

#include "port.hpp"
class ConnectionEstablisher {
public:
    virtual net::awaitable<void> establish_connection(Port port) = 0;
};
