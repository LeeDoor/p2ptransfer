#pragma once
#include "socket_manager_builder.hpp"
#include "socket_manager_impl.hpp"

namespace p2ptransfer {

/*!
* \brief Implementation of @ref SocketManagerBuilder class
*/
class SocketManagerImplBuilder : public SocketManagerBuilder {
public:
    SocketManagerImplBuilder(ContextPtr context) :
        context_(context) {}

    net::awaitable<std::shared_ptr<SocketManager>> tcp_listening_at(Port port) override {
        return listening_at<SocketManagerTcp>(port);
    }
    net::awaitable<std::shared_ptr<SocketManager>> udp_listening_at(Port port) override {
        return listening_at<SocketManagerUdp>(port);
    }
    net::awaitable<std::shared_ptr<SocketManager>> tcp_connecting_to(const Address& address, Port port) override {
        return connecting_to<SocketManagerTcp>(address, port);
    }
    net::awaitable<std::shared_ptr<SocketManager>> udp_connecting_to(const Address& address, Port port) override {
        return connecting_to<SocketManagerUdp>(address, port);
    }

private:
    template<typename SocketType>
    net::awaitable<std::shared_ptr<SocketManager>> listening_at(Port port) {
        typename SocketType::EndpointType endpoint(SocketType::InternetProtocolType::v4(), port);
        typename SocketType::AcceptorType acceptor(*context_, endpoint);
        co_return co_await SocketType::open_for_listening(context_, acceptor);
    }

    template<typename SocketType>
    net::awaitable<std::shared_ptr<SocketManager>> connecting_to(const Address& address, Port port) {
        typename SocketType::EndpointType endpoint(net::ip::make_address(address), port);
        co_return co_await SocketType::open_for_connecting(context_, endpoint);
    }
    
    ContextPtr context_;
};

}
