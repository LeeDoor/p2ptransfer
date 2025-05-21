#include <iostream>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include "connection_handler.hpp"
#include "network_headers.hpp"
net::awaitable<void> ConnectionHandler::handle() {
    std::string data(4096, '\0');
    size_t transferred = co_await socket_.async_read_some(net::buffer(data, 4096), net::use_awaitable);
    data.resize(transferred);
    std::cout << "received " << transferred << " bytes: " << data << std::endl;
    co_await net::async_write(socket_, net::buffer(data, transferred), net::use_awaitable);
    socket_.shutdown(tcpip::socket::shutdown_both);
    socket_.close();
    std::cout << "closed socket" << std::endl;
    co_return;
}

