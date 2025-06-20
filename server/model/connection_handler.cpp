#include "connection_handler.hpp"
#include "common_types.hpp"
#include "file_processor.hpp"

net::awaitable<void> ConnectionHandler::handle(Port port) {
    co_await try_open_connection(port);
    auto remote_endpoint = socket_manager_impl_->get_remote_endpoint();
    callback()->connected(remote_endpoint.address, remote_endpoint.port);
    co_await try_read_file();
}

net::awaitable<void> ConnectionHandler::try_open_connection(Port port) {
    try {
        co_await socket_manager_impl_->open_connection_async(port);
    } catch(...) {
        callback()->cant_open_socket();
        throw;
    }
}
net::awaitable<void> ConnectionHandler::try_read_file() {
    auto remote_endpoint = socket_manager_impl_->get_remote_endpoint();
    FileProcessor handler(std::move(socket_manager_impl_));
    handler.set_callback(static_pointer_cast<IFileTransferCallback>(callback()));
    try {
        co_await handler.read_remote_file();
        callback()->file_transfered();
    } catch (const std::exception& ex) {
        callback()->connection_aborted(remote_endpoint.address, remote_endpoint.port);
        throw;
    }
}
