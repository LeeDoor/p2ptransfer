#include "connection_handler.hpp"
#include "common_types.hpp"
#include "file_processor.hpp"

net::awaitable<void> ConnectionHandler::handle(Port port) {
    co_await try_open_connection(port);
    auto remote_endpoint = socket_manager_->get_remote_endpoint();
    callback()->connected(remote_endpoint.address, remote_endpoint.port);
    co_await try_read_file();
}

net::awaitable<void> ConnectionHandler::try_open_connection(Port port) {
    try {
        co_await socket_manager_->open_connection_async(port);
    } catch(...) {
        callback()->cant_open_socket();
        throw;
    }
}
net::awaitable<void> ConnectionHandler::try_read_file() {
    auto remote_endpoint = socket_manager_->get_remote_endpoint();
    FileProcessor file_processor(std::move(socket_manager_));
    file_processor.set_callback(static_pointer_cast<IFileTransferCallback>(callback()));
    try {
        co_await file_processor.read_remote_file();
        callback()->file_transfered();
    } catch (const std::exception& ex) {
        callback()->connection_aborted(remote_endpoint.address, remote_endpoint.port);
        throw;
    }
}
