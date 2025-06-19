#include "socket_manager.hpp"

SockPtr SocketManager::get_connection_sync(Port port) {
    SockPtr sock_;
    co_spawn(context_, get_connection_async(port), [&](std::exception_ptr a, SockPtr sock){
        sock_ = std::move(sock);
    });
    context_.run();
    return sock_;
}
net::awaitable<SockPtr> SocketManager::get_connection_async(Port port) {
    tcpip::endpoint endpoint(tcpip::v4(), port);
    try {
        SocketCloser socketCloser = [] (tcpip::socket* socket) {
            ErrorCode ec;
            socket->shutdown(tcpip::socket::shutdown_both, ec);
            socket->close();
        };
        tcpip::acceptor acceptor(context_, endpoint);
        SockPtr tcp_socket (new tcpip::socket(context_), socketCloser);
        auto [ec] = co_await acceptor.async_accept(*tcp_socket.get(), net::as_tuple(net::use_awaitable));
        if(ec) {
            co_return nullptr;
        }
        co_return tcp_socket;
    } catch (const std::exception& ex) {
        co_return nullptr;
    }
}
net::awaitable<SockPtr> SocketManager::connect_async(Address address, Port port) {
    tcpip::resolver resolver_(context_);
    auto [resolve_ec, endpoint] =
        co_await resolver_.async_resolve(address, std::to_string(port), net::as_tuple(net::use_awaitable));
    if(resolve_ec) {
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
        co_return nullptr;
    }
    co_return tcp_socket;
}
SockPtr SocketManager::connect_sync(Address address, Port port) {
    SockPtr sock_;
    co_spawn(context_, connect_async(address, port), [&](std::exception_ptr a, SockPtr sock){
        sock_ = std::move(sock);
    });
    context_.run();
    return sock_;
}
