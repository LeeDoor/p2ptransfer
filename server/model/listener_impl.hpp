#pragma once

#include "listener.hpp"
#include "model_factory.hpp"
class ListenerImpl : public Listener {
public:
    ListenerImpl(std::shared_ptr<ModelFactory> model_factory);
    ~ListenerImpl();
    void listen_if_not_already(Port port) override;
private:
    void try_join_context_thread();
    void listen(Port port);
    net::awaitable<void> listen_async(Port port);
    void run_context_thread();

    std::shared_ptr<ModelFactory> factory_;
    net::io_context context_;
    std::jthread context_thread_;
    bool is_running_ = false;
};
