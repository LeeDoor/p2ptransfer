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
        context_(context), 
        acceptor_tcp_(*context_),
        acceptor_udp_(*context_)
    {}

    void cancel() override {
        ErrorCode ec;
        ec = acceptor_tcp_.cancel(ec);   
        ec = acceptor_udp_.cancel(ec);
    }

    net::awaitable<std::shared_ptr<SocketManager>> tcp_listening_at(Port port) override {
        using SocketType = SocketManagerTcp;
        return listening_at<SocketType>(prepare_acceptor<SocketType>(acceptor_tcp_, port));
    }

    net::awaitable<std::shared_ptr<SocketManager>> udp_listening_at(Port port) override {
        using SocketType = SocketManagerUdp;
        return listening_at<SocketManagerUdp>(prepare_acceptor<SocketType>(acceptor_udp_, port));
    }

    net::awaitable<std::shared_ptr<SocketManager>> tcp_connecting_to(const Address& address, Port port) override {
        return connecting_to<SocketManagerTcp>(address, port);
    }

    net::awaitable<std::shared_ptr<SocketManager>> udp_connecting_to(const Address& address, Port port) override {
        return connecting_to<SocketManagerUdp>(address, port);
    }

private:
    template<typename SocketType>
    net::awaitable<std::shared_ptr<SocketManager>> listening_at(SocketType::AcceptorType& acceptor) {
        co_return co_await SocketType::open_for_listening(context_, acceptor);
    }

    template<typename SocketType>
    SocketType::AcceptorType& prepare_acceptor(SocketType::AcceptorType& acceptor, Port port) {
        typename SocketType::EndpointType endpoint(SocketType::InternetProtocolType::v4(), port);
        if(acceptor.is_open()) acceptor.close();
        acceptor.open(endpoint.protocol());
        acceptor.bind(endpoint);
        acceptor.listen();
        return acceptor;
    }

    template<typename SocketType>
    net::awaitable<std::shared_ptr<SocketManager>> connecting_to(const Address& address, Port port) {
        typename SocketType::EndpointType endpoint(net::ip::make_address(address), port);
        co_return co_await SocketType::open_for_connecting(context_, endpoint);
    }
    
    ContextPtr context_;
    SocketManagerTcp::AcceptorType acceptor_tcp_;
    SocketManagerUdp::AcceptorType acceptor_udp_;
};

}
