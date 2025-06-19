#pragma once
#include "port.hpp"
#include "common_types.hpp"

class SocketManager {
public:
    net::awaitable<SockPtr> get_connection(Port port);
private:
    net::io_context context_;
};
