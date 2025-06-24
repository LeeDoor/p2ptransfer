#pragma once

#include "address.hpp"
#include "port.hpp"
class IConnectionStatusCallback {
public:
    virtual ~IConnectionStatusCallback() = default;
    virtual void connected(const Address& address, Port port) = 0;
    virtual void cant_open_socket() = 0;
};
