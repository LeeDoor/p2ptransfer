#pragma once
#include "socket_manager_builder.hpp"
#include "thread_wrapper.hpp"
#include "transferer.hpp"

namespace p2ptransfer {
namespace model {

class TransfererImpl : public Transferer {
public:
    using ThreadWrapperPtr = std::shared_ptr<ThreadWrapper>;
    using SocketManagerBuilderPtr = std::shared_ptr<SocketManagerBuilder>;
    using SocketManagerPtr = std::shared_ptr<SocketManager>;
    using ContextPtr = std::shared_ptr<net::io_context>;
    TransfererImpl(
        ContextPtr context,
        ThreadWrapperPtr thread_wrapper,
        SocketManagerBuilderPtr socket_builder);

    void transfer_file(const Address& address, Port port, const Filename& filename) override;

private:
    ContextPtr context_;
    ThreadWrapperPtr thread_wrapper_;
    SocketManagerBuilderPtr socket_builder_;
};

}
}
