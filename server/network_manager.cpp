#include "network_manager.hpp"
#include "connection_handler.hpp"
#include <iostream>
struct ConnectionHandler;
net::awaitable<void> NetworkManager::listen() {
    tcpip::socket tcp_socket = co_await get_connection();
    std::cout << "connected from " 
        << tcp_socket.remote_endpoint().address() 
        << ":"
        << tcp_socket.remote_endpoint().port()
        << std::endl;
    ConnectionHandler handler(context_, std::move(tcp_socket));
    co_await co_spawn(context_, handler.handle(), net::use_awaitable);
}
net::awaitable<tcpip::socket> NetworkManager::get_connection() {
    tcpip::socket tcp_socket {context_};
    co_await tcp_acceptor_.async_accept(tcp_socket, net::use_awaitable);
    co_return tcp_socket;
}

