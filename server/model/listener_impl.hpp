#pragma once
#include "file_processor_builder.hpp"
#include "listener.hpp"
#include "socket_manager_builder.hpp"
#include "thread_wrapper.hpp"

class ListenerImpl : public Listener {
public:
    ListenerImpl(std::shared_ptr<net::io_context> context,
                 std::shared_ptr<ThreadWrapper> thread_wrapper,
                 std::shared_ptr<SocketManagerFactory> socket_manager_factory,
                 std::shared_ptr<FileProcessorBuilder> file_processor_factory);
    void listen_if_not_already(Port port) override;
    void stop() override;

private:
    void spawn_listen_coroutine(Port port);
    net::awaitable<void> listen_async(Port port);
    void spawn_and_run(Port port);
    net::awaitable<std::shared_ptr<SocketManager>> connect_and_listen(Port port);
    net::awaitable<std::shared_ptr<SocketManager>> build_socket_manager(Port port);

    std::shared_ptr<ThreadWrapper> thread_wrapper_;
    std::shared_ptr<SocketManagerFactory> socket_manager_factory_;
    std::shared_ptr<FileProcessorBuilder> file_processor_factory_;
    std::shared_ptr<net::io_context> context_;
};
