#pragma once
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include "network_headers.hpp"

class ConnectionHandler {
public:
    ConnectionHandler(net::io_context& ctx, tcpip::socket socket)
        : context_(ctx), socket_(std::move(socket)){}
    int handle();
private:
    net::io_context& context_;
    tcpip::socket socket_;
};
