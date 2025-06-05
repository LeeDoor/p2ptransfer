#pragma once
#include "common_types.hpp"
#include "port.hpp"
class NetworkManager {
public:
    NetworkManager()
        : context_(){} 
    int init(Port port);
private:
    net::awaitable<void> listen(Port port);
    net::awaitable<SockPtr> get_connection(Port port);
    net::io_context context_;
};
