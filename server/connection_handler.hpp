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
    net::awaitable<bool> send_permission(const SendRequest& send_request);
    net::io_context& io_;
    tcpip::socket socket_;
};
