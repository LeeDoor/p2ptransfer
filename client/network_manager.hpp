#pragma once
#include <string>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "network_headers.hpp"
#include "port.hpp"
#include "address.hpp"

class NetworkManager {  
public:
    NetworkManager(): context_{}, resolver_(context_) {}
    int initialize_connection(Address address, Port port, std::string message);
private:
    net::awaitable<void> connect_and_send(Address address, Port port, std::string message);
    net::awaitable<std::optional<tcpip::socket>> try_connect(Address address, Port port);
    net::io_context context_;
    tcpip::resolver resolver_;
};
