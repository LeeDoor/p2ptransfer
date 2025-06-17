#pragma once
#include "common_types.hpp"
#include "model_callback.hpp"
#include "request_structures.hpp"
class Presenter;
class ConnectionHandler {
public:
    ConnectionHandler(net::io_context& ctx, SockPtr socket, std::weak_ptr<IModelCallback> callback)
    : io_(ctx), socket_(std::move(socket)), callback_(callback)
    {}
    net::awaitable<int> handle();
private:
    net::awaitable<std::optional<SendRequest>> handle_send_request(std::string& buffer);
    template<typename OStream>
    net::awaitable<bool> handle_file(OStream& os, const SendRequest& send_request);
    net::awaitable<bool> send_permission(const SendRequest& send_request);
    net::io_context& io_;
    SockPtr socket_;
    std::weak_ptr<IModelCallback> callback_;
};
