#pragma once
#include <vector>
#include "network_headers.hpp"
class NetworkManager {
public:
    NetworkManager()
        : context_(), 
        endpoint_(tcpip::v4(), 8080), 
        tcp_acceptor_(context_, endpoint_){}
    net::awaitable<void> listen();
private:
    net::awaitable<tcpip::socket> get_connection();
    net::io_context context_;
    tcpip::endpoint endpoint_;
    tcpip::acceptor tcp_acceptor_;
};
