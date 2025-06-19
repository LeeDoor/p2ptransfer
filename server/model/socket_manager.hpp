#pragma once
#include "address.hpp"
#include "port.hpp"
#include "common_types.hpp"

class SocketManager {
public:
    net::awaitable<SockPtr> get_connection_async(Port port);
    SockPtr get_connection_sync(Port port);

    net::awaitable<SockPtr> connect_async(Address address, Port port);
    SockPtr connect_sync(Address address, Port port);
private:
    net::io_context context_;
};
