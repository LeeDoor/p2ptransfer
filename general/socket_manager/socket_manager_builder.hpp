#pragma once
#include "socket_manager.hpp"

namespace general {

/*!
* \brief socket builder for immediate connection
*
* Using socket builder prevents class user from accessing
* socket before connection. Takes over the responsibility
* to ensure the returned socket connection is established.
* \throws std::runtime_error if socket opening or connection 
* establishing failed.
*/
class SocketManagerBuilder {
public:
    virtual net::awaitable<std::shared_ptr<SocketManager>> tcp_listening_at(Port port) = 0;
    virtual net::awaitable<std::shared_ptr<SocketManager>> udp_listening_at(Port port) = 0;
    virtual net::awaitable<std::shared_ptr<SocketManager>> tcp_connecting_to(const Address& address, Port port) = 0;
    virtual net::awaitable<std::shared_ptr<SocketManager>> udp_connecting_to(const Address& address, Port port) = 0;

    virtual ~SocketManagerBuilder() = default;
};

}
