#pragma once
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/awaitable.hpp>
#include "network_headers.hpp"
#include "request_structures.hpp"

class ConnectionHandler {
public:
    ConnectionHandler(net::io_context& ctx, tcpip::socket&& socket)
        : io_(ctx), socket_(std::move(socket)){}
    net::awaitable<void> handle();
private:
    net::awaitable<std::optional<SendRequest>> handle_send_request(std::string& buffer);
    net::awaitable<std::optional<std::string>> async_readline(std::string& buffer);
    net::awaitable<std::optional<std::string>> handle_method(std::string& buffer);
    net::awaitable<std::optional<std::string>> handle_filename(std::string& buffer);
    net::awaitable<std::optional<size_t>> handle_filesize(std::string& buffer);

    net::io_context& io_;
    tcpip::socket socket_;
};
