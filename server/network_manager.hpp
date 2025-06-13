#pragma once
#include "common_types.hpp"
#include "model_network_manager.hpp"
#include "port.hpp"
class NetworkManager : public IModelNetworkManager {
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
