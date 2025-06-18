#pragma once
#include "common_types.hpp"
#include "listener.hpp"
#include "port.hpp"
class ConnectionEstablisher : public IListener {
public:
    ConnectionEstablisher()
        : context_(){} 
    ~ConnectionEstablisher();
    void listen(Port port) override;
private:
    net::awaitable<void> listen_async(Port port);
    net::awaitable<SockPtr> get_connection(Port port);

    std::thread context_thread_;
    net::io_context context_;
    bool is_running_ = false;
};
