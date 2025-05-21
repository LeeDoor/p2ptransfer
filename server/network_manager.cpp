#include "network_manager.hpp"
#include "network_headers.hpp"
#include "connection_handler.hpp"
#include <boost/asio/as_tuple.hpp>
#include <iostream>
#include <boost/asio/detached.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
int NetworkManager::init(Port port) {
    endpoint_.port(port);
    boost::system::error_code ec{};
    tcp_acceptor_.bind(endpoint_, ec);
    if(ec) {
        std::cout << "failed to open endpoint: " << ec.what() << std::endl;
        return 1;
    }
    co_spawn(context_, listen(), net::detached);
    context_.run();
    return 0;
}
net::awaitable<void> NetworkManager::listen() {
    auto tcp_socket = co_await get_connection();
    if(!tcp_socket) {
        std::cout << "failed to open socket." << std::endl;
        co_return;
    }
    std::cout << "connected from " 
        << tcp_socket->remote_endpoint().address() 
        << ":"
        << tcp_socket->remote_endpoint().port()
        << std::endl;
    ConnectionHandler handler(context_, std::move(*tcp_socket));
    co_await handler.handle();
}
net::awaitable<std::optional<tcpip::socket>> NetworkManager::get_connection() {
    tcpip::socket tcp_socket {context_};
    auto [ec] = co_await tcp_acceptor_.async_accept(tcp_socket, net::as_tuple(net::use_awaitable));
    if(ec) {
        std::cout << "failed to accept connection: " << ec.what() << std::endl;
        co_return std::nullopt;
    }
    co_return tcp_socket;
}

