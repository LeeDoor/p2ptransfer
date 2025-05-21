#pragma once
#include "network_headers.hpp"
#include "port.hpp"
#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
class NetworkManager {
public:
    NetworkManager()
        : context_(){} 
    int init(Port port);
private:
    net::awaitable<void> listen(Port port);
    net::awaitable<std::optional<tcpip::socket>> get_connection(Port port);
    net::io_context context_;
};
