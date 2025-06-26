#include "listener_impl.hpp"
#include "common_types.hpp"
#include "logger.hpp"

ListenerImpl::ListenerImpl(std::shared_ptr<ModelFactory> model_factory) :
    factory_(model_factory){}
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
    auto socket_manager = factory_->create_socket_manager(context_);
    auto establisher = factory_->create_connection_establisher(socket_manager, callback());
    co_await establisher->establish_connection(port);
    auto file_processor = factory_->create_file_processor(socket_manager, callback());
    co_await file_processor->try_read_file();
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
