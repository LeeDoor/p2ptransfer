#include "listener_impl.hpp"
#include "common_types.hpp"
#include "connection_establisher.hpp"
#include "file_processor.hpp"
#include "logger.hpp"
#include "socket_manager_impl.hpp"

ListenerImpl::~ListenerImpl() {
    if(is_running_) {
        context_.stop();
    }
}
void ListenerImpl::listen_if_not_already(Port port) {
    if(is_running_) return;
    try_join_context_thread();
    is_running_ = true;
    listen(port);
    run_context_thread();
    return;
}

void ListenerImpl::try_join_context_thread() {
    if(context_thread_.joinable())
        context_thread_.join();
}

void ListenerImpl::listen(Port port) {
    auto rethrow_functor = [](std::exception_ptr ptr) {
        if(ptr) std::rethrow_exception(ptr);
    };
    net::co_spawn(context_, listen_async(port), rethrow_functor);
}

net::awaitable<void> ListenerImpl::listen_async(Port port) {
    auto socket_manager = std::make_shared<SocketManagerImpl>(context_);
    ConnectionEstablisher establisher(socket_manager);
    establisher.set_callback(callback());
    co_await establisher.establish_connection(port);
    FileProcessor file_processor(socket_manager);
    file_processor.set_callback(callback());
    co_await file_processor.try_read_file();
}

void ListenerImpl::run_context_thread() {
    context_thread_ = std::jthread([this] {
        Logger::catch_log([&]{
            context_.run();
        });
        context_.restart();
        is_running_ = false;
    });
}
