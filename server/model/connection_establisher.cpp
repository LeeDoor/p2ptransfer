#include "connection_establisher.hpp"
#include "common_types.hpp"

net::awaitable<void> ConnectionEstablisherImpl::establish_connection(Port port) {
    co_await try_establish_connection(port);
    auto remote_endpoint = socket_manager_->get_remote_endpoint();
    callback()->connected(remote_endpoint.address, remote_endpoint.port);
}

net::awaitable<void> ConnectionEstablisherImpl::try_establish_connection(Port port) {
    try {
        co_await socket_manager_->establish_connection_async(port);
    } catch(...) {
        callback()->cant_open_socket();
        throw;
    }
}
