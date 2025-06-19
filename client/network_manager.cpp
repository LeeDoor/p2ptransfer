#include "network_manager.hpp"
#include "connection_handler.hpp"
#include "common_types.hpp"
#include "logger.hpp"

int ConnectionHandler::initialize_connection(Address address, Port port, std::string filename) {
    co_spawn(context_, connect_and_send(address, port, std::move(filename)), net::detached);
    context_.run();
    return 0;
}
net::awaitable<void> ConnectionHandler::connect_and_send(Address address, Port port, std::string filename) {
    SockPtr tcp_socket = co_await try_connect(address, port);
    if(!tcp_socket) {
        Logger::log() << "failed to open socket." << std::endl;
        co_return;
    }
    FileProcessor handler(context_, std::move(tcp_socket));
    if(co_await handler.handle(std::move(filename))) {
        Logger::log() << "failed to handle connection." << std::endl;
        co_return;
    }
}
net::awaitable<SockPtr> ConnectionHandler::try_connect(Address address, Port port) {
    auto [resolve_ec, endpoint] =
        co_await resolver_.async_resolve(address, std::to_string(port), net::as_tuple(net::use_awaitable));
    if(resolve_ec) {
        Logger::log() << "failed to resolve endpoint: " << address << ":" << port << std::endl;
        co_return nullptr;
    }
    SocketCloser socketCloser = [] (tcpip::socket* s) {
        ErrorCode ec;
        s->shutdown(tcpip::socket::shutdown_both, ec);
        s->close();
    };
    SockPtr tcp_socket(new tcpip::socket(context_), socketCloser);
    auto [ec] = co_await tcp_socket->async_connect(endpoint.begin()->endpoint(), net::as_tuple(net::use_awaitable));
    if(ec) {
        Logger::log() << "failed to connect: " << ec.what() << std::endl;
        co_return nullptr;
    }
    co_return tcp_socket;
}
