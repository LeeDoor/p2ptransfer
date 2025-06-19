#include "connection_establisher.hpp"
#include "common_types.hpp"
#include "file_processor.hpp"
#include "logger.hpp"

void ConnectionEstablisher::try_join_context_thread() {
    if(context_thread_.joinable()) {
        context_thread_.join();
    }
}
void ConnectionEstablisher::run_context_thread() {
    context_thread_ = std::thread([this] {
        context_.run();
        context_.restart();
        is_running_ = false;
    });
}
ConnectionEstablisher::~ConnectionEstablisher() {
    if(is_running_) {
        context_.stop();
    }
    try_join_context_thread();
}
void ConnectionEstablisher::listen_if_not_already(Port port) {
    if(is_running_) return;
    try_join_context_thread();
    is_running_ = true;
    co_spawn(context_, listen_async(port), net::detached);
    run_context_thread();
    return;
}
net::awaitable<void> ConnectionEstablisher::listen_async(Port port) {
    SockPtr tcp_socket = co_await socket_manager_->get_connection_async(port);
    auto callback = callback_.lock();
    if(!callback) co_return;
    if(!tcp_socket) {
        Logger::log() << "failed to open socket." << std::endl;
        callback->cant_open_socket();
        co_return;
    }
    Address remote_address = tcp_socket->remote_endpoint().address().to_string();
    Port remote_port = tcp_socket->remote_endpoint().port();
    Logger::log() << "connected from " << remote_address << ":" << remote_port << std::endl;
    callback->connected(remote_address, remote_port);
    FileProcessor handler(context_, std::move(tcp_socket));
    handler.set_callback(static_pointer_cast<IFileTransferCallback>(callback));
    try {
        co_await handler.read_remote_file();
        callback->file_transfered();
    } catch (const std::exception& ex) {
        callback->connection_aborted(remote_address, remote_port);
        throw;
    }
}

