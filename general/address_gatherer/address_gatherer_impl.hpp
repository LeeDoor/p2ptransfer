#pragma once
#include "address_gatherer.hpp"
#include "thread_wrapper.hpp"
#include "socket_manager_builder.hpp"

namespace general {

class AddressGathererImpl : public AddressGatherer {
public:
    using ContextPtr = std::shared_ptr<net::io_context>;
    using ThreadWrapperPtr = std::shared_ptr<ThreadWrapper>;
    using SocketManagerBuilderPtr = std::shared_ptr<SocketManagerBuilder>;
    using SocketManagerPtr = std::shared_ptr<SocketManager>;

    AddressGathererImpl(
        ContextPtr context,
        ThreadWrapperPtr thread_wrapper,
        SocketManagerBuilderPtr socket_manager_builder);

    void gather_local_address() override;
    void stop() override;
private:
    net::awaitable<void> gather_async();
    net::awaitable<SocketManagerPtr> build_socket_manager(const Address& address, Port port);

    SocketManagerBuilderPtr socket_builder_;
    ThreadWrapperPtr thread_wrapper_;
    ContextPtr context_;
};

}
