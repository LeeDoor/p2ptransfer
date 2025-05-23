#include "connection_handler.hpp"
#include <boost/asio/as_tuple.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/write.hpp>
#include <filesystem>
#include "logger.hpp"
#include "request_serializer.hpp"

net::awaitable<int> ConnectionHandler::handle(std::string filepath) {
    #define HANDLE_RETURN(ec) socket_.shutdown(tcpip::socket::shutdown_both); co_return ec
    if(! co_await send_request(filepath)) {
        Logger::log() << "failed to send send_request." << std::endl;
        HANDLE_RETURN(1);
    }
    co_await read_permission();
    HANDLE_RETURN(0);
}
net::awaitable<bool> ConnectionHandler::send_request(const std::string& filepath) {
    namespace fs = std::filesystem;
    auto send_request_opt = 
        RequestSerializer::serialize_send_request(fs::path(filepath).filename(), fs::file_size(filepath));
    if(!send_request_opt) { 
        Logger::log() << "failed to serialize send_request." << std::endl;
        co_return false;
    }
    std::string send_request(std::move(*send_request_opt));
    boost::system::error_code ec;
    size_t bytes;
    std::tie(ec, bytes) = co_await net::async_write(socket_, net::buffer(send_request),
                                                  net::as_tuple(net::use_awaitable));
    if(ec) {
        Logger::log() << "failed to write data: " << ec.what() << std::endl;
        co_return false;
    }
    co_return true;
}
net::awaitable<bool> ConnectionHandler::read_permission() {
    boost::system::error_code ec;
    size_t bytes;
    std::string read_buffer;
    std::tie(ec, bytes) = co_await net::async_read_until(socket_, net::dynamic_buffer(read_buffer),
                                                         "\n\n", net::as_tuple(net::use_awaitable));
    if(ec && ec != net::error::eof) {
        Logger::log() << "failed to read data: " << ec.what() << std::endl;
        co_return false;
    }
    Logger::log() << bytes << " response from server: " << read_buffer.substr(0, bytes);
    co_return true;
}
