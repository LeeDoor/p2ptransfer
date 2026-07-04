#include "listeners_lookup_impl.hpp"
#include "broadcast_socket_manager.hpp"
#include "thread_wrapper.hpp"

namespace p2ptransfer {
namespace model {

ListenersLookupImpl::ListenersLookupImpl(
    ContextPtr context,
    SocketManagerPtr socket_manager,
    ThreadWrapperPtr thread_wrapper) :
    socket_manager_{ socket_manager },
    thread_wrapper_{ thread_wrapper },
    context_{ context }
{}

void ListenersLookupImpl::start_lookup() {
    if(thread_wrapper_->is_running()) 
        throw std::logic_error("starting listeners lookup twice");
    run_lookup();
}

void ListenersLookupImpl::run_lookup() {
    net::co_spawn(*context_, lookup_async(), net::detached);
    thread_wrapper_->execute([this] {
        context_->run();
        context_->restart();
    });
}

net::awaitable<void> ListenersLookupImpl::lookup_async() {
    try {
        while(true) {
         //    auto result = co_await socket_manager_->receive();
            // callback()->responce_received(result.address, result.port);
        co_await socket_manager_->broadcast_send(LOOKUP_PORT, LOOKUP_MSG);
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    } catch (const std::exception& ex) {
        /* Ignore error */
    }
    socket_manager_ = nullptr;
}

void ListenersLookupImpl::stop() {
    context_->stop();
    if(socket_manager_) {
        socket_manager_->stop();
    }
}

}
}

