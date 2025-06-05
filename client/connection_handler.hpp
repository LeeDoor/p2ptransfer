#pragma once
#include <boost/asio/awaitable.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include "network_headers.hpp"

class ConnectionHandler {
public:
    ConnectionHandler(net::io_context& ctx, SockPtr socket)
        : context_(ctx), socket_(std::move(socket)){}
    net::awaitable<int> handle(std::string filepath);
private:
    template <typename IStream>
    net::awaitable<bool> send_file(IStream& is, size_t filesize);
    net::awaitable<bool> send_request(const std::string& filename, size_t filesize);
    net::awaitable<bool> read_permission();

    net::io_context& context_;
    SockPtr socket_;
};
