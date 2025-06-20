#pragma once
#include "callback.hpp"
#include "common_types.hpp"
#include "port.hpp"
#include "remote_interaction_callback.hpp"
#include "socket_manager.hpp"
class ConnectionHandler : public WithCallback<IRemoteInteractionCallback> {
public:
    ConnectionHandler(std::unique_ptr<ISocketManager> socket_manager) :
        socket_manager_(std::move(socket_manager)){}
    net::awaitable<void> handle(Port port);
private:
    net::awaitable<void> try_open_connection(Port port);
    net::awaitable<void> try_read_file();

    std::unique_ptr<ISocketManager> socket_manager_;
};
