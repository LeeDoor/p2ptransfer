#include "listener_impl.hpp"

#include "logger.hpp"

ListenerImpl::ListenerImpl(std::shared_ptr<net::io_context> context,
                           std::shared_ptr<ThreadWrapper> thread_wrapper,
                           std::shared_ptr<SocketManagerFactory> socket_manager_factory,
                           std::shared_ptr<FileProcessorBuilder> file_processor_factory) :
    thread_wrapper_(thread_wrapper),
    socket_manager_factory_(socket_manager_factory),
    file_processor_factory_(file_processor_factory),
    context_(context)
{}

void ListenerImpl::listen_if_not_already(Port port) {
    if(!thread_wrapper_->is_running()) { 
        spawn_and_run(port);
    }
}

void ListenerImpl::spawn_and_run(Port port) {
    thread_wrapper_->execute([port, this] {
        spawn_listen_coroutine(port);
        context_->run();
        context_->restart();
    });
}

void ListenerImpl::spawn_listen_coroutine(Port port) {
    net::co_spawn(*context_, listen_async(port), net::detached);
}

net::awaitable<void> ListenerImpl::listen_async(Port port) {
    try {
        auto socket_manager = co_await connect_and_listen(port);
        auto file_processor = file_processor_factory_->create_file_processor(callback(), socket_manager);
        co_await file_processor->try_read_file();
    } catch(const std::exception& ex) {
        Logger::log() << ex.what() << std::endl;
    }
}

net::awaitable<std::shared_ptr<SocketManager>> ListenerImpl::connect_and_listen(Port port) {
    try {
        auto socket_manager = co_await socket_manager_factory_->tcp_listening_at(port);
        auto endpoint = socket_manager->get_remote_endpoint();
        callback()->connected(endpoint.address, endpoint.port);
        co_return socket_manager;
    } catch(const std::exception& ex) {
        callback()->cant_open_socket();
        throw;
    }
}

void ListenerImpl::stop() {
    context_->stop();
    thread_wrapper_->join();
}
