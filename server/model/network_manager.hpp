#pragma once
#include "common_types.hpp"
#include "view_callback.hpp"
#include "port.hpp"
#include "callback.hpp"
#include "connection_status_callback.hpp"
class NetworkManager : public IViewCallback, public WithCallback<std::weak_ptr<IConnectionStatusCallback>> {
public:
    NetworkManager()
        : context_(){} 
    ~NetworkManager();
    void listen(Port port) override;
private:
    net::awaitable<void> listen_async(Port port);
    net::awaitable<SockPtr> get_connection(Port port);

    std::thread context_thread_;
    net::io_context context_;
    bool is_running_ = false;
};
