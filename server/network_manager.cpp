#include "network_manager.hpp"
#include "connection_handler.hpp"
#include <iostream>
#include <boost/asio/detached.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
int NetworkManager::init() {
    co_spawn(context_, listen(), net::detached);
    context_.run();
    return 0;
}
net::awaitable<void> NetworkManager::listen() {
    tcpip::socket tcp_socket = co_await get_connection();
    if(!tcp_socket.is_open()) {
        std::cout << "failed to open socket." << std::endl;
        co_return;
    }
    std::cout << "connected from " 
        << tcp_socket.remote_endpoint().address() 
        << ":"
        << tcp_socket.remote_endpoint().port()
        << std::endl;
    ConnectionHandler handler(context_, std::move(tcp_socket));
    co_await handler.handle();
}
net::awaitable<tcpip::socket> NetworkManager::get_connection() {
    tcpip::socket tcp_socket {context_};
    co_await tcp_acceptor_.async_accept(tcp_socket, net::use_awaitable);
    co_return tcp_socket;
}

