#pragma once
#include "common_types.hpp"
#include "listener.hpp"
#include "port.hpp"
#include "socket_manager.hpp"
class ConnectionEstablisher : public IListener {
public:
    ~ConnectionEstablisher();
    void listen_if_not_already(Port port) override;
private:
    void try_join_context_thread();
    void run_context_thread();
    net::awaitable<void> listen_async(Port port);

    net::io_context context_;
    std::thread context_thread_;
    bool is_running_ = false;
};
