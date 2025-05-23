#include "connection_handler.hpp"
#include <boost/asio/as_tuple.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <filesystem>
#include "logger.hpp"
#include "request_serializer.hpp"

net::awaitable<int> ConnectionHandler::handle(std::string filename) {
    auto send_request = 
        RequestSerializer::serialize_send_request(filename, std::filesystem::file_size(filename));
    if(!send_request) { 
        Logger::log() << "failed to serialize send_request." << std::endl;
        co_return 3;
    }
    boost::system::error_code ec;
    size_t bytes;
    std::tie(ec, bytes) = co_await socket_.async_write_some(net::buffer(*send_request),
                                                  net::as_tuple(net::use_awaitable));
    if(ec) {
        Logger::log() << "failed to write data: " << ec.what() << std::endl;
        co_return 2;
    }
    Logger::log() << "sended to server: " << *send_request << std::endl;
    std::string read_buffer;
    read_buffer.reserve(1984);
    std::tie(ec, bytes) = co_await socket_.async_read_some(net::buffer(read_buffer),
                                                        net::as_tuple(net::use_awaitable));
    if(ec) {
        Logger::log() << "failed to read data: " << ec.what() << std::endl;
        co_return 1;
    }
    Logger::log() << "response from server: " << read_buffer.substr(0, bytes) << std::endl;
    socket_.shutdown(tcpip::socket::shutdown_both);
    socket_.close();
    co_return 0;
}
