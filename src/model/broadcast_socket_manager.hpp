#pragma once
#include "address.hpp"
#include "port.hpp"

namespace p2ptransfer {

class BroadcastSocketManager {
public:
    virtual ~BroadcastSocketManager() = default;

    struct BroadcastResponse {
        std::string data;
        Address address;
        Port port;
    };

    virtual net::awaitable<void> broadcast_send(Port port, std::string message) = 0;
    virtual net::awaitable<BroadcastResponse> receive() = 0;
    virtual void stop() = 0;
};

}
