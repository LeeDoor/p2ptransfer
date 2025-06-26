#pragma once
#include "callback.hpp"
#include "common_types.hpp"
#include "port.hpp"
#include "socket_manager.hpp"
#include "connection_status_callback.hpp"

class ConnectionEstablisherImpl : public WithCallback<ConnectionStatusCallback> {
public:
    ConnectionEstablisherImpl(std::shared_ptr<SocketManager> socket_manager) :
        socket_manager_(socket_manager){}
    net::awaitable<void> establish_connection(Port port);

private:
    net::awaitable<void> try_establish_connection(Port port);
    net::awaitable<void> try_read_file();

    std::shared_ptr<SocketManager> socket_manager_;
};
