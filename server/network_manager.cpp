#include "network_manager.hpp"
#include "common_types.hpp"
#include "connection_handler.hpp"
#include "logger.hpp"
int NetworkManager::init(Port port) {
    co_spawn(context_, listen(port), net::detached);
    context_.run();
    return 0;
}
net::awaitable<void> NetworkManager::listen(Port port) {
    SockPtr tcp_socket = co_await get_connection(port);
    if(!tcp_socket) {
        Logger::log() << "failed to open socket." << std::endl;
        co_return;
    }
    Logger::log() << "connected from " 
        << tcp_socket->remote_endpoint().address() 
        << ":"
        << tcp_socket->remote_endpoint().port()
        << std::endl;
    ConnectionHandler handler(context_, std::move(tcp_socket));
    co_await handler.handle();
}
net::awaitable<SockPtr> NetworkManager::get_connection(Port port) {
    tcpip::endpoint endpoint(tcpip::v4(), port);
    try {
        SocketCloser socketCloser = [] (tcpip::socket* socket) {
            ErrorCode ec;
            socket->shutdown(tcpip::socket::shutdown_both, ec);
            socket->close();
            Logger::log() << "socket closed successfully: " << ec.what() << std::endl;
        };
        tcpip::acceptor acceptor(context_, endpoint);
        std::unique_ptr<tcpip::socket, SocketCloser> tcp_socket (new tcpip::socket(context_), socketCloser);
        Logger::log() << "listening for a connection on " << endpoint.address() << ":" << endpoint.port() << std::endl;
        auto [ec] = co_await acceptor.async_accept(*tcp_socket.get(), net::as_tuple(net::use_awaitable));
        if(ec) {
            Logger::log() << "failed to accept connection: " << ec.what() << std::endl;
            co_return nullptr;
        }
        co_return tcp_socket;
    } catch (const std::exception& ex) {
        Logger::log() << "failed while creating acceptor: " << ex.what() << std::endl;
        co_return nullptr;
    }
}

