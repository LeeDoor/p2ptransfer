#pragma once
#include "stdlib.hpp"
#include "common_types.hpp"
#include "port.hpp"
#include "address.hpp"

class ConnectionEstablisherImpl {  
public:
    ConnectionEstablisherImpl(): context_{}, resolver_(context_) {}
    int initialize_connection(Address address, Port port, std::string message);
private:
    net::awaitable<void> connect_and_send(Address address, Port port, std::string message);
    net::awaitable<SockPtr> try_connect(Address address, Port port);
    net::io_context context_;
    tcpip::resolver resolver_;
};
