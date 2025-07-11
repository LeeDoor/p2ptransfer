#pragma once
#include "socket_manager.hpp"

class SocketManagerBuilder {
public:
    virtual net::awaitable<std::shared_ptr<SocketManager>> tcp_listening_at(Port port) = 0;
    virtual net::awaitable<std::shared_ptr<SocketManager>> udp_listening_at(Port port) = 0;
    virtual net::awaitable<std::shared_ptr<SocketManager>> tcp_connecting_to(const Address& address, Port port) = 0;
    virtual net::awaitable<std::shared_ptr<SocketManager>> udp_connecting_to(const Address& address, Port port) = 0;
};
