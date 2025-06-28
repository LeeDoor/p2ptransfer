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

void ListenerImpl::spawn_listen_coroutine(Port port) {
    auto rethrow_functor = [](std::exception_ptr ptr) {
        if(ptr) std::rethrow_exception(ptr);
    };
    net::co_spawn(context_, listen_async(port), rethrow_functor);
}

net::awaitable<void> ListenerImpl::listen_async(Port port) {
    auto socket_manager = co_await build_socket_manager(port);
    auto establisher = factory_->create_connection_establisher(socket_manager, callback());
    co_await establisher->establish_connection(port);
    auto file_processor = factory_->create_file_processor(socket_manager, callback());
    co_await file_processor->try_read_file();
}

void ListenerImpl::spawn_and_run(Port port) {
    thread_wrapper_->execute([=, this] {
        Logger::catch_log([&]{
            spawn_listen_coroutine(port);
            context_.run();
        });
        context_.restart();
    });
}

net::awaitable<std::shared_ptr<SocketManager>> ListenerImpl::build_socket_manager(Port port) {
    auto socket_builder = factory_->create_socket_manager(context_);
    co_return co_await socket_builder->tcp_listening_at(port);
}
