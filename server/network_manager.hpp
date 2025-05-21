#pragma once
#include "network_headers.hpp"
#include <boost/asio/awaitable.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
class NetworkManager {
public:
    NetworkManager()
        : context_(), 
        endpoint_(tcpip::v4(), 8080), 
        tcp_acceptor_(context_, endpoint_){}
    int init();
private:
    net::awaitable<void> listen();
    net::awaitable<tcpip::socket> get_connection();
    net::io_context context_;
    tcpip::endpoint endpoint_;
    tcpip::acceptor tcp_acceptor_;
};
