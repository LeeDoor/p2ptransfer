#pragma once
#include "common_types.hpp"
#include "listener.hpp"
#include "port.hpp"
class ConnectionEstablisher : public IListener {
public:
    ConnectionEstablisher()
        : context_(){} 
    ~ConnectionEstablisher();
    void listen_if_not_already(Port port) override;
private:
    void try_join_context_thread();
    void run_context_thread();
    net::awaitable<void> listen_async(Port port);
    net::awaitable<SockPtr> get_connection(Port port);

    std::thread context_thread_;
    net::io_context context_;
    bool is_running_ = false;
};
