#pragma once
#include "callback.hpp"
#include "common_types.hpp"
#include "port.hpp"
#include "remote_interaction_callback.hpp"
#include "socket_manager_impl.hpp"
class ConnectionHandler : public WithCallback<IRemoteInteractionCallback> {
public:
    ConnectionHandler(net::io_context& context) :
        socket_manager_impl_(std::make_unique<SocketManagerImpl>(context)){}
    net::awaitable<void> handle(Port port);
private:
    net::awaitable<void> try_open_connection(Port port);
    net::awaitable<void> try_read_file();

    std::unique_ptr<SocketManagerImpl> socket_manager_impl_;
};
