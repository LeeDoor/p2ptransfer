#pragma once
#include "socket_manager_builder.hpp"
#include "socket_manager_impl.hpp"

namespace general {

/*!
* \brief Implementation of @ref SocketManagerBuilder class
*/
class SocketManagerImplBuilder : public SocketManagerBuilder {
public:
    SocketManagerImplBuilder(std::shared_ptr<net::io_context> context) :
        context_(context) {}
    net::awaitable<std::shared_ptr<SocketManager>> tcp_listening_at(Port port) override {
        co_return co_await SocketManagerTcp::open_for_listening(*context_, port);
    }
    net::awaitable<std::shared_ptr<SocketManager>> udp_listening_at(Port port) override {
        co_return co_await SocketManagerUdp::open_for_listening(*context_, port);
    }
    net::awaitable<std::shared_ptr<SocketManager>> tcp_connecting_to(const Address& address, Port port) override {
        co_return co_await SocketManagerTcp::open_for_connecting(*context_, address, port);
    }
    net::awaitable<std::shared_ptr<SocketManager>> udp_connecting_to(const Address& address, Port port) override {
        co_return co_await SocketManagerUdp::open_for_connecting(*context_, address, port);
    }

private:
    std::shared_ptr<net::io_context> context_;
};

}
