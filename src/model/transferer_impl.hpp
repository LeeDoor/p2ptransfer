#pragma once
#include "file_writer_builder.hpp"
#include "socket_manager_builder.hpp"
#include "thread_wrapper.hpp"
#include "transferer.hpp"

namespace p2ptransfer {
namespace model {

class TransfererImpl : public Transferer {
public:
    using ContextPtr = std::shared_ptr<net::io_context>;
    using ThreadWrapperPtr = std::shared_ptr<ThreadWrapper>;
    using SocketManagerBuilderPtr = std::shared_ptr<SocketManagerBuilder>;
    using SocketManagerPtr = std::shared_ptr<SocketManager>;
    using FileWriterBuilderPtr = std::shared_ptr<FileWriterBuilder>;
    TransfererImpl(
        ContextPtr context,
        ThreadWrapperPtr thread_wrapper,
        SocketManagerBuilderPtr socket_builder,
        FileWriterBuilderPtr file_writer_builder);

    void transfer_file(const Address& address, Port port, const Filename& filename) override;

private:
    net::awaitable<std::shared_ptr<SocketManager>> connect(const Address& address, Port port);
    net::awaitable<void> send_file(std::shared_ptr<SocketManager> socket, const Filename& filename);
    net::awaitable<void> connect_and_send(const Address& address, Port port, const Filename& filename);

    ContextPtr context_;
    ThreadWrapperPtr thread_wrapper_;
    SocketManagerBuilderPtr socket_builder_;
    FileWriterBuilderPtr file_writer_builder_;
};

}
}
