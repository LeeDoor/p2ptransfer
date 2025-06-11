#pragma once
#include "common_types.hpp"
#include "request_structures.hpp"
class Presenter;
class ConnectionHandler {
public:
    ConnectionHandler(net::io_context& ctx, SockPtr socket, std::weak_ptr<Presenter> presenter)
    : io_(ctx), socket_(std::move(socket)), presenter_(presenter)
    {}
    net::awaitable<int> handle();
private:
    net::awaitable<std::optional<SendRequest>> handle_send_request(std::string& buffer);
    template<typename OStream>
    net::awaitable<bool> handle_file(OStream& os, const SendRequest& send_request);
    net::awaitable<bool> send_permission(const SendRequest& send_request);
    net::io_context& io_;
    SockPtr socket_;
    std::weak_ptr<Presenter> presenter_;
};
