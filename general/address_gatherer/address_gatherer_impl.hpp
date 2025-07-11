#pragma once
#include "address_gatherer.hpp"
#include "thread_wrapper.hpp"
#include "socket_manager_builder.hpp"

class AddressGathererImpl : public AddressGatherer {
public:
    AddressGathererImpl(
        std::shared_ptr<net::io_context> context,
        std::shared_ptr<ThreadWrapper> thread_wrapper,
        std::shared_ptr<SocketManagerBuilder> socket_manager_builder);

    void gather_local_address() override;
    void stop() override;
private:
    net::awaitable<void> gather_async();
    net::awaitable<std::shared_ptr<SocketManager>> build_socket_manager(const Address& address, Port port);

    std::shared_ptr<SocketManagerBuilder> socket_builder_;
    std::shared_ptr<ThreadWrapper> thread_wrapper_;
    std::shared_ptr<net::io_context> context_;
};
