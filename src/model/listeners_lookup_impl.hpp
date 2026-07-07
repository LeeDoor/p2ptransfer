#pragma once
#include "listeners_lookup.hpp"

namespace p2ptransfer {

class SocketManagerMulticast;
class SocketManagerBuilder;
class ThreadWrapper;

namespace model {

class ListenersLookupImpl : public ListenersLookup {
public:
    using ThreadWrapperPtr = std::shared_ptr<ThreadWrapper>;
    using SocketManagerPtr = std::shared_ptr<SocketManagerMulticast>;
    using SocketManagerBuilderPtr = std::shared_ptr<SocketManagerBuilder>;

    virtual ~ListenersLookupImpl() = default;
 
    ListenersLookupImpl(
        ContextPtr context,
        SocketManagerBuilderPtr socket_builder,
        ThreadWrapperPtr thread_wrapper);

    void start_lookup() override;
    void stop() override;

private:
    void run_lookup();
    net::awaitable<void> lookup_async();

    SocketManagerBuilderPtr socket_builder_;
    SocketManagerPtr socket_manager_;
    ThreadWrapperPtr thread_wrapper_;
    ContextPtr context_;
};

}
}
