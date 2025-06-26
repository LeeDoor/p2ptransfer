#pragma once

#include "connection_establisher.hpp"
class ConnectionEstablisherMock : public ConnectionEstablisher {
public:
    MOCK_METHOD(net::awaitable<void>, establish_connection, (Port port), (override));
};
