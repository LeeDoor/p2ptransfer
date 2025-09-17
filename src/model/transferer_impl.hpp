#pragma once
#include "context_wrapper.hpp"
#include "transferer.hpp"

namespace p2ptransfer {

class ThreadWrapper;
class SocketManagerBuilder;
class SocketManager;

namespace model {

class FileWriterBuilder;

class TransfererImpl : public Transferer {
public:
    using ContextPtr = ContextWrapper;
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
    /*! Has non-reference parameters to copy incoming objects and avoid SIGSEGV */
    net::awaitable<void> connect_and_send(Address address, Port port, Filename filename);

    ContextPtr context_;
    ThreadWrapperPtr thread_wrapper_;
    SocketManagerBuilderPtr socket_builder_;
    FileWriterBuilderPtr file_writer_builder_;
};

}
}
