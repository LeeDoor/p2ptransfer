#pragma once
#include "listener.hpp"
#include "model_factory.hpp"
#include "thread_wrapper.hpp"

class ListenerImpl : public Listener {
public:
    ListenerImpl(std::shared_ptr<ModelFactory> model_factory);
    void listen_if_not_already(Port port) override;

private:
    void spawn_listen_coroutine(Port port);
    net::awaitable<void> listen_async(Port port);
    void spawn_and_run(Port port);
    net::awaitable<std::shared_ptr<SocketManager>> connect_and_listen(Port port);
    net::awaitable<std::shared_ptr<SocketManager>> build_socket_manager(Port port);

    std::shared_ptr<ModelFactory> factory_;
    std::shared_ptr<ThreadWrapper> thread_wrapper_;
    net::io_context context_;
};
