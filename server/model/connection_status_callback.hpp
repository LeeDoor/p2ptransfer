#pragma once
#include "address.hpp"
#include "port.hpp"

namespace general {
namespace model {

class ConnectionStatusCallback {
public:
    virtual void connected(const Address& address, Port port) = 0;
    virtual void cant_open_socket() = 0;

    virtual ~ConnectionStatusCallback() = default;
};

}
}
