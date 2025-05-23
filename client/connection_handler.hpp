#pragma once
#include <boost/asio/awaitable.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include "network_headers.hpp"

class ConnectionHandler {
public:
    ConnectionHandler(net::io_context& ctx, tcpip::socket socket)
        : context_(ctx), socket_(std::move(socket)){}
    net::awaitable<int> handle(std::string filepath);
private:
    net::awaitable<bool> send_request(const std::string& filepath);
    net::awaitable<bool> read_permission();

    net::io_context& context_;
    tcpip::socket socket_;
};
