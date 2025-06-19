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
    std::unique_ptr<SocketManager> socket_manager_ = std::make_unique<SocketManager>(context_);
    try {
        auto callback = callback_.lock();
        if(!callback) throw std::runtime_error("callback of ConnectionEstablisher is dead");
        try {
        co_await socket_manager_->get_connection_async(port);
        } catch(...) {
            callback->cant_open_socket();
            throw;
        }
        auto remote_endpoint = socket_manager_->get_remote_endpoint();
        callback->connected(remote_endpoint.address, remote_endpoint.port);
        FileProcessor handler(std::move(socket_manager_));
        handler.set_callback(static_pointer_cast<IFileTransferCallback>(callback));
        try {
            co_await handler.read_remote_file();
            callback->file_transfered();
        } catch (const std::exception& ex) {
            callback->connection_aborted(remote_endpoint.address, remote_endpoint.port);
            throw;
        }
    } catch (std::exception& ex) {
        Logger::log() << ex.what() << std::endl;
    }
}

