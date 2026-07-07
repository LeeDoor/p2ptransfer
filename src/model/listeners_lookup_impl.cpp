#include "listeners_lookup_impl.hpp"
#include "socket_manager_multicast.hpp"
#include "socket_manager_builder.hpp"
#include "thread_wrapper.hpp"
#include "lookup_endpoint.hpp"

namespace p2ptransfer {
namespace model {

ListenersLookupImpl::ListenersLookupImpl(
    ContextPtr context,
    SocketManagerBuilderPtr socket_builder,
    ThreadWrapperPtr thread_wrapper) :
    socket_builder_{ socket_builder },
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
        socket_manager_ = co_await socket_builder_->multicast_bind_to(LOOKUP_ADDRESS, LOOKUP_PORT);
        net::steady_timer timer(*context_);
        timer.expires_at(std::chrono::steady_clock::now() + std::chrono::seconds(2));
        while(true) {
            std::variant<SocketManagerMulticast::MulticastResponse, std::monostate> result = co_await (
                socket_manager_->receive() ||
                timer.async_wait(net::use_awaitable)
            );
            if(result.index() == 0) {
                auto response = std::get<0>(result);
                callback()->responce_received(response .address, std::stoi(response.data));
                break;
            }
        }
    } catch (const std::exception& ex) {
        callback()->failed_to_lookup(ex.what());
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

