#pragma once
#include "address_gatherer.hpp"
#include "context_wrapper.hpp"
#include "port.hpp"

namespace p2ptransfer {

class SocketManager;
class SocketManagerBuilder;
class ThreadWrapper;

/*!
* \brief Implements \ref AddressGatherer interface.
* 
* Uses \ref ThreadWrapper to detach address gathering execution on other thread.
* Uses \ref SocketManager via UDP connection to get local address.
* Even if connection to router is forbidden, it resolves LAN address.
*/
class AddressGathererImpl : public AddressGatherer {
public:
    using ContextPtr = ContextWrapper;
    using ThreadWrapperPtr = std::shared_ptr<ThreadWrapper>;
    using SocketManagerBuilderPtr = std::shared_ptr<SocketManagerBuilder>;
    using SocketManagerPtr = std::shared_ptr<SocketManager>;

    AddressGathererImpl(
        ContextPtr context,
        ThreadWrapperPtr thread_wrapper,
        SocketManagerBuilderPtr socket_manager_builder);

    void gather_local_address() override;

private:
    void run_gathering();
    net::awaitable<void> gather_async();
    net::awaitable<SocketManagerPtr> build_socket_manager(const Address& address, Port port);

    SocketManagerBuilderPtr socket_builder_;
    ThreadWrapperPtr thread_wrapper_;
    ContextPtr context_;
};

}
