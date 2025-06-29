#pragma once

#include "model_factory.hpp"
#include "socket_manager_mock.hpp"
class SocketManagerMockFactory : public SocketManagerFactory {
public:
    MOCK_METHOD(net::awaitable<std::shared_ptr<SocketManager>>, tcp_listening_at, (Port port), (override));
    MOCK_METHOD(net::awaitable<std::shared_ptr<SocketManager>>, udp_listening_at, (Port port), (override));
    MOCK_METHOD(net::awaitable<std::shared_ptr<SocketManager>>, tcp_connecting_to, (const Address& address, Port port), (override));
    MOCK_METHOD(net::awaitable<std::shared_ptr<SocketManager>>, udp_connecting_to, (const Address& address, Port port), (override));
};

