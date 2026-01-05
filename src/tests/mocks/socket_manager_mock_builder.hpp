#pragma once
#include "socket_manager_builder.hpp"

namespace p2ptransfer {
namespace test {

/*!
* \brief Test mock for @ref SocketManagerBuilder class.
*
* Implements @ref SocketManagerBuilder interface to mock it. 
* Does not provide any connection, returning @ref SocketManagerMock instead
* (or any other socket implementation required for testing).
* Base class implementation is protected to make mocks without return value.
* Mock implementation can still throw exceptions in testing reason.
*/    
class SocketManagerMockBuilder : public SocketManagerBuilder {
public:
    SocketManagerMockBuilder(std::shared_ptr<SocketManager> socket_manager) :
        socket_manager_(socket_manager){}
   
    MOCK_METHOD(void, cancel, (), (override));
    MOCK_METHOD(void, mock_tcp_listening_at, (Port port), ());
    MOCK_METHOD(void, mock_udp_listening_at, (Port port), ());
    MOCK_METHOD(void, mock_tcp_connecting_to, (const Address& address, Port port), ());
    MOCK_METHOD(void, mock_udp_connecting_to, (const Address& address, Port port), ());

protected:
    /// Mock method
    net::awaitable<std::shared_ptr<SocketManager>> tcp_listening_at(Port port) override {
        mock_tcp_listening_at(port);
        co_return socket_manager_;
    }
    /// Mock method
    net::awaitable<std::shared_ptr<SocketManager>> udp_listening_at(Port port) override {
        mock_udp_listening_at(port);
        co_return socket_manager_;
    }
    /// Mock method
    net::awaitable<std::shared_ptr<SocketManager>> tcp_connecting_to(const Address& address, Port port) override {
        mock_tcp_connecting_to(address, port);
        co_return socket_manager_;
    }
    /// Mock method
    net::awaitable<std::shared_ptr<SocketManager>> udp_connecting_to(const Address& address, Port port) override {
        mock_udp_connecting_to(address, port);
        co_return socket_manager_;
    }

    std::shared_ptr<SocketManager> socket_manager_;
};

}
}
