#include "socket_manager.hpp"

net::awaitable<SockPtr> SocketManager::get_connection(Port port) {
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
