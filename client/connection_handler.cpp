#include "connection_handler.hpp"
#include <boost/asio/as_tuple.hpp>
#include <boost/asio/use_awaitable.hpp>
#include "logger.hpp"

net::awaitable<int> ConnectionHandler::handle(std::string message) {
    boost::system::error_code ec;
    size_t bytes;
    std::tie(ec, bytes) = co_await socket_.async_write_some(net::buffer(message, message.size()), 
                                                  net::as_tuple(net::use_awaitable));
    if(ec) {
        Logger::log() << "failed to write data: " << ec.what() << std::endl;
        co_return 1;
    }
    Logger::log() << "transfered " << bytes << " bytes." << std::endl;
    std::tie(ec, bytes) = co_await socket_.async_read_some(net::buffer(message, message.capacity()),
                                                        net::as_tuple(net::use_awaitable));
    if(ec) {
        Logger::log() << "failed to read data: " << ec.what() << std::endl;
        co_return 2;
    }
    message.resize(bytes);
    Logger::log() << "gathered " << bytes << " bytes: " << message << std::endl;

    socket_.shutdown(tcpip::socket::shutdown_both);
    socket_.close();
    co_return 0;
}
