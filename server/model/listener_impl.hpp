#pragma once
#include "file_processor_builder.hpp"
#include "listener.hpp"
#include "socket_manager_builder.hpp"
#include "thread_wrapper.hpp"

namespace general {
namespace server {
namespace model {

class ListenerImpl : public Listener {
public:
    using ThreadWrapperPtr = std::shared_ptr<ThreadWrapper>;
    using SocketManagerBuilderPtr = std::shared_ptr<SocketManagerBuilder>;
    using SocketManagerPtr = std::shared_ptr<SocketManager>;
    using FileProcessorBuilderPtr = std::shared_ptr<FileProcessorBuilder>;
    using ContextPtr = std::shared_ptr<net::io_context>;

    ListenerImpl(ContextPtr context,
                 ThreadWrapperPtr thread_wrapper,
                 SocketManagerBuilderPtr socket_manager_builder,
                 FileProcessorBuilderPtr file_processor_builder);
    void listen_if_not_already(Port port) override;
    void stop() override;

private:
    void spawn_listen_coroutine(Port port);
    net::awaitable<void> listen_async(Port port);
    void spawn_and_run(Port port);
    net::awaitable<SocketManagerPtr> connect_and_listen(Port port);
    net::awaitable<SocketManagerPtr> build_socket_manager(Port port);

    ThreadWrapperPtr thread_wrapper_;
    SocketManagerBuilderPtr socket_manager_builder_;
    FileProcessorBuilderPtr file_processor_builder_;
    ContextPtr context_;
};

}
}
}
