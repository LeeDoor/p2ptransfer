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

net::awaitable<void> ConnectionHandler::handle() {
#define HANDLE_RETURN socket_.shutdown(tcpip::socket::shutdown_both); co_return
    std::string data;
    auto send_request = co_await handle_send_request(data);
    if(!send_request) {
        Logger::log() << "failed to parse send request. shutting down." << std::endl;
        HANDLE_RETURN;
    }
    Logger::log() 
        << "ready for gathering file " << send_request->filename 
        << " of size " << send_request->filesize << std::endl;
    HANDLE_RETURN;
    co_return;
}

net::awaitable<std::optional<SendRequest>> ConnectionHandler::handle_send_request(std::string& buffer) {
    size_t bytes;
    boost::system::error_code ec;
    std::string request;
    std::tie(ec, bytes) = 
        co_await net::async_read(socket_, net::dynamic_buffer(buffer), 
                                       net::as_tuple(net::use_awaitable));
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
