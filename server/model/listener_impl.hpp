#pragma once

#include "listener.hpp"
class ListenerImpl : public IListener {
public:
    ~ListenerImpl();
    void listen_if_not_already(Port port) override;
private:
    void try_join_context_thread();
    void listen(Port port);
    net::awaitable<void> listen_async(Port port);
    void run_context_thread();
    net::io_context context_;
    std::jthread context_thread_;
    bool is_running_ = false;
};
