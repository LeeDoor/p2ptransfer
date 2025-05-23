#include <boost/asio/co_spawn.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/as_tuple.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/read_until.hpp>
#include "connection_handler.hpp"
#include "network_headers.hpp"
#include "logger.hpp"
#include "request_deserializer.hpp"
#include "request_serializer.hpp"

net::awaitable<void> ConnectionHandler::handle() {
#define HANDLE_RETURN socket_.shutdown(tcpip::socket::shutdown_both); co_return
    std::string data;
    auto send_request = co_await handle_send_request(data);
    if(!send_request) {
        Logger::log() << "failed to perform send request. shutting down." << std::endl;
        HANDLE_RETURN;
    }
    if(!co_await send_permission(*send_request)) {
        Logger::log() << "failed to perform send permission. shutting down." << std::endl;
        HANDLE_RETURN;
    }
    HANDLE_RETURN;
}

net::awaitable<bool> ConnectionHandler::send_permission(const SendRequest& send_request) {
    auto send_permission = RequestSerializer::serialize_send_permission(send_request.filename);
    if(!send_permission) {
        Logger::log() << "failed to serialize send permission." << std::endl;
        co_return false;
    }
    boost::system::error_code ec;
    size_t bytes;
    std::tie(ec, bytes) =
        co_await net::async_write(socket_, net::buffer(*send_permission), net::as_tuple(net::use_awaitable));
    if(ec) {
        Logger::log() << "failed to write send permission." << std::endl;
        co_return false;
    }
    co_return true;
}
net::awaitable<std::optional<SendRequest>> ConnectionHandler::handle_send_request(std::string& buffer) {
    size_t bytes;
    boost::system::error_code ec;
    std::string request;
    std::tie(ec, bytes) = 
        co_await net::async_read_until(socket_, net::dynamic_buffer(buffer), 
                                       "\n\n", net::as_tuple(net::use_awaitable));
    if(ec && ec != boost::asio::error::eof) {
        Logger::log() << "failed to read line: " << ec.what() << std::endl;
        co_return std::nullopt;
    }
    request = buffer.substr(0, bytes);
    buffer.erase(0, bytes);
    RequestDeserializer deserializer;
    if(auto send_request = deserializer.deserialize_send_request(request)) {
        co_return *send_request;
    }
    co_return std::nullopt;
}
