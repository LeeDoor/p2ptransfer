#pragma once
#include "address.hpp"
#include "common_types.hpp"
#include "port.hpp"
class Presenter;
class NetworkManager {
public:
    NetworkManager()
        : context_(){} 
    ~NetworkManager();
    int init(Port port);
    void set_presenter(std::shared_ptr<Presenter> presenter);
    Address get_local_address();
private:
    net::awaitable<void> listen(Port port);
    net::awaitable<SockPtr> get_connection(Port port);

    std::thread context_thread_;
    net::io_context context_;
    bool is_running_ = false;
    std::weak_ptr<Presenter> presenter_;
};
