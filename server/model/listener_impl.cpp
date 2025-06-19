#include "listener_impl.hpp"
#include "common_types.hpp"
#include "connection_handler.hpp"
#include "logger.hpp"

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
    ConnectionHandler handler(context_);
    handler.set_callback(callback());
    co_await handler.handle(port);
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
