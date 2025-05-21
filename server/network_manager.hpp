#pragma once
#define BOOST_ASIO_NO_DEPRECATED
#include "network_headers.hpp"
#include "port.hpp"
#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
class NetworkManager {
public:
    NetworkManager()
        : context_(), endpoint_(), tcp_acceptor_(context_){} 
    int init(Port port);
private:
    net::awaitable<void> listen();
    net::awaitable<std::optional<tcpip::socket>> get_connection();
    net::io_context context_;
    tcpip::endpoint endpoint_;
    tcpip::acceptor tcp_acceptor_;
};
