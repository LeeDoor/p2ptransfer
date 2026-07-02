#pragma once
#include "listeners_lookup.hpp"

namespace p2ptransfer {

class BroadcastSocketManager;
class ThreadWrapper;

namespace model {

class ListenersLookupImpl : public ListenersLookup {
public:
    using ThreadWrapperPtr = std::shared_ptr<ThreadWrapper>;
    using SocketManagerPtr = std::shared_ptr<BroadcastSocketManager>;

    virtual ~ListenersLookupImpl() = default;
 
    ListenersLookupImpl(
        ContextPtr context,
        SocketManagerPtr socket_manager,
        ThreadWrapperPtr thread_wrapper);

    void start_lookup() override;
    void stop() override;

private:
    static constexpr std::string LOOKUP_MSG = "Stub message";
    static constexpr Port LOOKUP_PORT = 45953;

    void run_lookup();
    net::awaitable<void> lookup_async();

    SocketManagerPtr socket_manager_;
    ThreadWrapperPtr thread_wrapper_;
    ContextPtr context_;
};

}
}
