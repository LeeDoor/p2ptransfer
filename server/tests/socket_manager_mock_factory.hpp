#pragma once

#include "model_factory.hpp"
#include "socket_manager_mock.hpp"
class SocketManagerMockFactory : public SocketManagerFactory {
public:
    SocketManagerMockFactory() :
        SocketManagerMockFactory(std::make_shared<SocketManagerMock>()) {}
    SocketManagerMockFactory(std::shared_ptr<SocketManager> sm) :
        socket_manager_(sm) {}
    net::awaitable<std::shared_ptr<SocketManager>> tcp_listening_at(Port port) override {
        co_return socket_manager_;
    }
    net::awaitable<std::shared_ptr<SocketManager>> udp_listening_at(Port port) override {
        co_return socket_manager_;
    }
    net::awaitable<std::shared_ptr<SocketManager>> tcp_connecting_to(const Address& address, Port port) override {
        co_return socket_manager_;
    }
    net::awaitable<std::shared_ptr<SocketManager>> udp_connecting_to(const Address& address, Port port) override {
        co_return socket_manager_;
    }

private:
    std::shared_ptr<SocketManager> socket_manager_;
};

