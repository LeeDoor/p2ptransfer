#include "network_manager.hpp"
#include "network_headers.hpp"
#include "connection_handler.hpp"
#include <boost/asio/as_tuple.hpp>
#include "logger.hpp"
#include <boost/asio/detached.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/ip/address.hpp>
int NetworkManager::init(Port port) {
    co_spawn(context_, listen(port), net::detached);
    context_.run();
    return 0;
}
net::awaitable<void> NetworkManager::listen(Port port) {
    auto tcp_socket = co_await get_connection(port);
    if(!tcp_socket) {
        Logger::log() << "failed to open socket." << std::endl;
        co_return;
    }
    Logger::log() << "connected from " 
        << tcp_socket->remote_endpoint().address() 
        << ":"
        << tcp_socket->remote_endpoint().port()
        << std::endl;
    ConnectionHandler handler(context_, std::move(*tcp_socket));
    co_await handler.handle();
}
net::awaitable<std::optional<tcpip::socket>> NetworkManager::get_connection(Port port) {
    tcpip::endpoint endpoint(tcpip::v4(), port);
    try {
        tcpip::acceptor acceptor(context_, endpoint);
        tcpip::socket tcp_socket {context_};
        Logger::log() << "listening for a connection on " << endpoint.address() << ":" << endpoint.port() << std::endl;
        auto [ec] = co_await acceptor.async_accept(tcp_socket, net::as_tuple(net::use_awaitable));
        if(ec) {
            Logger::log() << "failed to accept connection: " << ec.what() << std::endl;
            co_return std::nullopt;
        }
        co_return tcp_socket;
    } catch (const std::exception& ex) {
        Logger::log() << "failed while creating acceptor: " << ex.what() << std::endl;
        co_return std::nullopt;
    }
}

