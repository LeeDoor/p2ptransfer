#pragma once
#include "common_types.hpp"
#include "port.hpp"
class NetworkManager {
public:
    NetworkManager()
        : context_(){} 
    ~NetworkManager();
    int init(Port port);
private:
    net::awaitable<void> listen(Port port);
    net::awaitable<SockPtr> get_connection(Port port);
    std::thread context_thread_;
    net::io_context context_;
    bool is_running_ = false;
};
