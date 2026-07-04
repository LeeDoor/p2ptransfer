#pragma once
#include "address.hpp"
#include "port.hpp"

namespace p2ptransfer {

class SocketManagerMulticast {
public:
    virtual ~SocketManagerMulticast() = default;

    struct MulticastResponse {
        std::string data;
        Address address;
        Port port;
    };

    virtual net::awaitable<void> send(std::string message) = 0;
    virtual net::awaitable<MulticastResponse> receive() = 0;
    virtual void stop() = 0;
};

}
