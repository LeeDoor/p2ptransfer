#include "connection_handler.hpp"
#include <boost/asio/as_tuple.hpp>
#include <boost/asio/use_awaitable.hpp>
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
    auto send_request = 
        RequestSerializer::serialize_send_request(fs::path(filepath).filename(), fs::file_size(filepath));
    if(!send_request) { 
        Logger::log() << "failed to serialize send_request." << std::endl;
        co_return false;
    }
    boost::system::error_code ec;
    size_t bytes;
    std::tie(ec, bytes) = co_await socket_.async_write_some(net::buffer(*send_request),
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
    read_buffer.reserve(1984);
    std::tie(ec, bytes) = co_await socket_.async_read_some(net::buffer(read_buffer),
                                                           net::as_tuple(net::use_awaitable));
    if(ec) {
        Logger::log() << "failed to read data: " << ec.what() << std::endl;
        co_return false;
    }
    Logger::log() << "response from server: " << read_buffer.substr(0, bytes) << std::endl;
    co_return true;
}
