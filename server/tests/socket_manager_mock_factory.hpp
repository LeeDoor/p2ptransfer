#pragma once
#include "socket_manager_builder.hpp"
class SocketManagerMockFactory : public SocketManagerFactory {
public:
    SocketManagerMockFactory(std::shared_ptr<SocketManager> socket_manager) :
        socket_manager_(socket_manager){}
   
    MOCK_METHOD(void, mock_tcp_listening_at, (Port port), ());
    MOCK_METHOD(void, mock_udp_listening_at, (Port port), ());
    MOCK_METHOD(void, mock_tcp_connecting_to, (const Address& address, Port port), ());
    MOCK_METHOD(void, mock_udp_connecting_to, (const Address& address, Port port), ());

protected:
    net::awaitable<std::shared_ptr<SocketManager>> tcp_listening_at(Port port) override {
        mock_tcp_listening_at(port);
        co_return socket_manager_;
    }
    net::awaitable<std::shared_ptr<SocketManager>> udp_listening_at(Port port) override {
        mock_udp_listening_at(port);
        co_return socket_manager_;
    }
    net::awaitable<std::shared_ptr<SocketManager>> tcp_connecting_to(const Address& address, Port port) override {
        mock_tcp_connecting_to(address, port);
        co_return socket_manager_;
    }
    net::awaitable<std::shared_ptr<SocketManager>> udp_connecting_to(const Address& address, Port port) override {
        mock_udp_connecting_to(address, port);
        co_return socket_manager_;
    }

    std::shared_ptr<SocketManager> socket_manager_;
};

