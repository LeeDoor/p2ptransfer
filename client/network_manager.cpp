#include "network_manager.hpp"
#include "connection_handler.hpp"
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/as_tuple.hpp>
#include <boost/asio/co_spawn.hpp>
#include "network_headers.hpp"
#include <iostream>

int NetworkManager::initialize_connection(Port port, std::string message) {
    co_spawn(context_, connect_and_send(port, std::move(message)), net::detached);
    context_.run();
    return 0;
}
net::awaitable<void> NetworkManager::connect_and_send(Port port, std::string message) {
    std::optional<tcpip::socket> tcp_socket = co_await try_connect(port);
    if(!tcp_socket) {
        std::cout << "failed to open socket." << std::endl;
        co_return;
    }
    ConnectionHandler handler(context_, std::move(*tcp_socket));
    handler.handle();
}
net::awaitable<std::optional<tcpip::socket>> NetworkManager::try_connect(Port port) {
    auto [resolve_ec, endpoint] =
        co_await resolver_.async_resolve("localhost", std::to_string(port), net::as_tuple(net::use_awaitable));
    if(resolve_ec) {
        std::cout << "failed to resolve endpoint: " << "localhost:" << port << std::endl;
        co_return std::nullopt;
    }
    tcpip::socket tcp_socket(context_);
    auto [ec] = co_await tcp_socket.async_connect(endpoint.begin()->endpoint(), net::as_tuple(net::use_awaitable));
    if(ec) {
        std::cout << "failed to connect: " << ec.what() << std::endl;
        co_return std::nullopt;
    }
    co_return tcp_socket;
}
