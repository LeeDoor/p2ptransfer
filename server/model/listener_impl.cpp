#include "listener_impl.hpp"
#include "common_types.hpp"
#include "logger.hpp"

ListenerImpl::ListenerImpl(std::shared_ptr<ModelFactory> model_factory) :
    factory_(model_factory),
    thread_wrapper_(factory_->create_thread_wrapper()){}

void ListenerImpl::listen_if_not_already(Port port) {
    if(!thread_wrapper_->is_running()) { 
        spawn_and_run(port);
    }
}

void ListenerImpl::spawn_and_run(Port port) {
    thread_wrapper_->execute([port, this] {
        spawn_listen_coroutine(port);
        context_.run();
        context_.restart();
    });
}

void ListenerImpl::spawn_listen_coroutine(Port port) {
    net::co_spawn(context_, listen_async(port), net::detached);
}

net::awaitable<void> ListenerImpl::listen_async(Port port) {
    try {
        auto socket_manager = co_await connect_and_listen(port);
        auto file_processor = factory_->create_file_processor(socket_manager, callback());
        co_await file_processor->try_read_file();
    } catch(const std::exception& ex) {
        Logger::log() << ex.what() << std::endl;
    }
}

net::awaitable<std::shared_ptr<SocketManager>> ListenerImpl::connect_and_listen(Port port) {
    try {
        auto socket_builder = factory_->create_socket_builder(context_);
        auto socket_manager = co_await socket_builder->tcp_listening_at(port);
        auto endpoint = socket_manager->get_remote_endpoint();
        callback()->connected(endpoint.address, endpoint.port);
        co_return socket_manager;
    } catch(const std::exception& ex) {
        callback()->cant_open_socket();
        throw;
    }
}
