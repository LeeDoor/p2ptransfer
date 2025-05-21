#pragma once
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/awaitable.hpp>
#include "network_headers.hpp"

class ConnectionHandler {
public:
    ConnectionHandler(net::io_context& ctx, tcpip::socket&& socket)
        : io_(ctx), socket_(std::move(socket)){}
    net::awaitable<void> handle();
private:
    net::io_context& io_;
    tcpip::socket socket_;
};
